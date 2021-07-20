/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bx/platform.h>

#include "Render/RenderInternal.h"
#include <Render/embedded_shader.h>
#include <bx/file.h>
#include <bx/mutex.h>

#include "Render/Topology.h"
#include "version.h"

#include "Render/Callback.hpp"
#include "Render/StaticVariable.h"

#if BX_PLATFORM_OSX || BX_PLATFORM_IOS
#include <objc/message.h>
#endif // BX_PLATFORM_OSX

BX_ERROR_RESULT(BGFX_ERROR_TEXTURE_VALIDATION, BX_MAKEFOURCC('b', 'g', 0, 1));

namespace bgfx
{
#define BGFX_API_THREAD_MAGIC UINT32_C(0x78666762)

#if BGFX_CONFIG_MULTITHREADED
#define BGFX_CHECK_API_THREAD()                                                                                        \
    BX_ASSERT(NULL != s_ctx, "Library is not initialized yet.");                                                       \
    BX_ASSERT(BGFX_API_THREAD_MAGIC == s_threadIndex, "Must be called from main thread.")
#define BGFX_CHECK_RENDER_THREAD()                                                                                     \
    BX_ASSERT((NULL != s_ctx && s_ctx->m_singleThreaded) || ~BGFX_API_THREAD_MAGIC == s_threadIndex,                   \
              "Must be called from render thread.")
#else
#define BGFX_CHECK_API_THREAD()
#define BGFX_CHECK_RENDER_THREAD()
#endif // BGFX_CONFIG_MULTITHREADED

#define BGFX_CHECK_CAPS(_caps, _msg)                                                                                   \
    BX_ASSERT(0 != (g_caps.supported & (_caps)),                                                                       \
              _msg " Use bgfx::getCaps to check " #_caps " backend renderer capabilities.");

#if BGFX_CONFIG_USE_TINYSTL
void *TinyStlAllocator::static_allocate(size_t _bytes)
{
    return BX_ALLOC(g_allocator, _bytes);
}

void TinyStlAllocator::static_deallocate(void *_ptr, size_t /*_bytes*/)
{
    if (NULL != _ptr)
    {
        BX_FREE(g_allocator, _ptr);
    }
}
#endif // BGFX_CONFIG_USE_TINYSTL

static CallbackStub *s_callbackStub = NULL;
static AllocatorStub *s_allocatorStub = NULL;
static bool s_graphicsDebuggerPresent = false;

CallbackI *g_callback = NULL;
bx::AllocatorI *g_allocator = NULL;

Caps g_caps;

#if BGFX_CONFIG_MULTITHREADED && !defined(BX_THREAD_LOCAL)
class ThreadData
{
    BX_CLASS(ThreadData, NO_COPY, NO_ASSIGNMENT);

  public:
    ThreadData(uintptr_t _rhs)
    {
        union {
            uintptr_t ui;
            void *ptr;
        } cast = {_rhs};
        m_tls.set(cast.ptr);
    }

    operator uintptr_t() const
    {
        union {
            uintptr_t ui;
            void *ptr;
        } cast;
        cast.ptr = m_tls.get();
        return cast.ui;
    }

    uintptr_t operator=(uintptr_t _rhs)
    {
        union {
            uintptr_t ui;
            void *ptr;
        } cast = {_rhs};
        m_tls.set(cast.ptr);
        return _rhs;
    }

    bool operator==(uintptr_t _rhs) const
    {
        uintptr_t lhs = *this;
        return lhs == _rhs;
    }

  private:
    bx::TlsData m_tls;
};

static ThreadData s_threadIndex(0);
#elif !BGFX_CONFIG_MULTITHREADED
static uint32_t s_threadIndex(0);
#else
static BX_THREAD_LOCAL uint32_t s_threadIndex(0);
#endif

static Context *s_ctx = NULL;
static bool s_renderFrameCalled = false;
InternalData g_internalData;
PlatformData g_platformData;
bool g_platformDataChangedSinceReset = false;


extern const EmbeddedShader s_embeddedShaders[];

static const uint32_t numCharsPerBatch = 1024;
static const uint32_t numBatchVertices = numCharsPerBatch * 4;
static const uint32_t numBatchIndices = numCharsPerBatch * 6;

void blit(RendererContextI *_renderCtx, TextVideoMemBlitter &_blitter, const TextVideoMem &_mem)
{
    struct Vertex
    {
        float m_x;
        float m_y;
        float m_z;
        uint32_t m_fg;
        uint32_t m_bg;
        float m_u;
        float m_v;
    };

    uint32_t yy = 0;
    uint32_t xx = 0;

    const float texelWidth = 1.0f / 2048.0f;
    const float texelWidthHalf = RendererType::Direct3D9 == g_caps.rendererType ? 0.0f : texelWidth * 0.5f;
    const float texelHeight = 1.0f / 24.0f;
    const float texelHeightHalf = RendererType::Direct3D9 == g_caps.rendererType ? texelHeight * 0.5f : 0.0f;
    const float utop = (_mem.m_small ? 0.0f : 8.0f) * texelHeight + texelHeightHalf;
    const float ubottom = (_mem.m_small ? 8.0f : 24.0f) * texelHeight + texelHeightHalf;
    const float fontHeight = (_mem.m_small ? 8.0f : 16.0f);

    _renderCtx->blitSetup(_blitter);

    const uint32_t *palette =
        0 != (s_ctx->m_init.resolution.reset & BGFX_RESET_SRGB_BACKBUFFER) ? paletteLinear : paletteSrgb;

    for (; yy < _mem.m_height;)
    {
        Vertex *vertex = (Vertex *)_blitter.m_vb->data;
        uint16_t *indices = (uint16_t *)_blitter.m_ib->data;
        uint32_t startVertex = 0;
        uint32_t numIndices = 0;

        for (; yy < _mem.m_height && numIndices < numBatchIndices; ++yy)
        {
            xx = xx < _mem.m_width ? xx : 0;
            const TextVideoMem::MemSlot *line = &_mem.m_mem[yy * _mem.m_width + xx];

            for (; xx < _mem.m_width && numIndices < numBatchIndices; ++xx)
            {
                uint32_t ch = line->character;
                const uint8_t attr = line->attribute;

                if (ch > 0xff)
                {
                    ch = 0;
                }

                if (0 != (ch | attr) && (' ' != ch || 0 != (attr & 0xf0)))
                {
                    const uint32_t fg = palette[attr & 0xf];
                    const uint32_t bg = palette[(attr >> 4) & 0xf];

                    Vertex vert[4] = {
                        {(xx)*8.0f, (yy)*fontHeight, 0.0f, fg, bg, (ch)*8.0f * texelWidth - texelWidthHalf, utop},
                        {(xx + 1) * 8.0f, (yy)*fontHeight, 0.0f, fg, bg, (ch + 1) * 8.0f * texelWidth - texelWidthHalf,
                         utop},
                        {(xx + 1) * 8.0f, (yy + 1) * fontHeight, 0.0f, fg, bg,
                         (ch + 1) * 8.0f * texelWidth - texelWidthHalf, ubottom},
                        {(xx)*8.0f, (yy + 1) * fontHeight, 0.0f, fg, bg, (ch)*8.0f * texelWidth - texelWidthHalf,
                         ubottom},
                    };

                    bx::memCopy(vertex, vert, sizeof(vert));
                    vertex += 4;

                    indices[0] = uint16_t(startVertex + 0);
                    indices[1] = uint16_t(startVertex + 1);
                    indices[2] = uint16_t(startVertex + 2);
                    indices[3] = uint16_t(startVertex + 2);
                    indices[4] = uint16_t(startVertex + 3);
                    indices[5] = uint16_t(startVertex + 0);

                    startVertex += 4;
                    indices += 6;

                    numIndices += 6;
                }

                line++;
            }

            if (numIndices >= numBatchIndices)
            {
                break;
            }
        }

        _renderCtx->blitRender(_blitter, numIndices);
    }
}

void srtToMatrix4_x1(void *_dst, const void *_src)
{
    Matrix4 *mtx = reinterpret_cast<Matrix4 *>(_dst);
    const Srt *srt = reinterpret_cast<const Srt *>(_src);

    const float rx = srt->rotate[0];
    const float ry = srt->rotate[1];
    const float rz = srt->rotate[2];
    const float rw = srt->rotate[3];

    const float xx2 = 2.0f * rx * rx;
    const float yy2 = 2.0f * ry * ry;
    const float zz2 = 2.0f * rz * rz;
    const float yx2 = 2.0f * ry * rx;
    const float yz2 = 2.0f * ry * rz;
    const float yw2 = 2.0f * ry * rw;
    const float wz2 = 2.0f * rw * rz;
    const float wx2 = 2.0f * rw * rx;
    const float xz2 = 2.0f * rx * rz;

    const float sx = srt->scale[0];
    const float sy = srt->scale[1];
    const float sz = srt->scale[2];

    mtx->un.val[0] = (1.0f - yy2 - zz2) * sx;
    mtx->un.val[1] = (yx2 + wz2) * sx;
    mtx->un.val[2] = (xz2 - yw2) * sx;
    mtx->un.val[3] = 0.0f;

    mtx->un.val[4] = (yx2 - wz2) * sy;
    mtx->un.val[5] = (1.0f - xx2 - zz2) * sy;
    mtx->un.val[6] = (yz2 + wx2) * sy;
    mtx->un.val[7] = 0.0f;

    mtx->un.val[8] = (xz2 + yw2) * sz;
    mtx->un.val[9] = (yz2 - wx2) * sz;
    mtx->un.val[10] = (1.0f - xx2 - yy2) * sz;
    mtx->un.val[11] = 0.0f;

    const float tx = srt->translate[0];
    const float ty = srt->translate[1];
    const float tz = srt->translate[2];

    mtx->un.val[12] = tx;
    mtx->un.val[13] = ty;
    mtx->un.val[14] = tz;
    mtx->un.val[15] = 1.0f;
}

void transpose(void *_dst, uint32_t _dstStride, const void *_src, uint32_t _srcStride = sizeof(bx::simd128_t))
{
    uint8_t *dst = reinterpret_cast<uint8_t *>(_dst);
    const uint8_t *src = reinterpret_cast<const uint8_t *>(_src);

    using namespace bx;

    const simd128_t r0 = simd_ld<simd128_t>(src);
    src += _srcStride;

    const simd128_t r1 = simd_ld<simd128_t>(src);
    src += _srcStride;

    const simd128_t r2 = simd_ld<simd128_t>(src);
    src += _srcStride;

    const simd128_t r3 = simd_ld<simd128_t>(src);

    const simd128_t aibj = simd_shuf_xAyB(r0, r2);     // aibj
    const simd128_t emfn = simd_shuf_xAyB(r1, r3);     // emfn
    const simd128_t ckdl = simd_shuf_zCwD(r0, r2);     // ckdl
    const simd128_t gohp = simd_shuf_zCwD(r1, r3);     // gohp
    const simd128_t aeim = simd_shuf_xAyB(aibj, emfn); // aeim
    const simd128_t bfjn = simd_shuf_zCwD(aibj, emfn); // bfjn
    const simd128_t cgko = simd_shuf_xAyB(ckdl, gohp); // cgko
    const simd128_t dhlp = simd_shuf_zCwD(ckdl, gohp); // dhlp

    simd_st(dst, aeim);
    dst += _dstStride;

    simd_st(dst, bfjn);
    dst += _dstStride;

    simd_st(dst, cgko);
    dst += _dstStride;

    simd_st(dst, dhlp);
}

void srtToMatrix4_x4_Ref(void *_dst, const void *_src)
{
    uint8_t *dst = reinterpret_cast<uint8_t *>(_dst);
    const uint8_t *src = reinterpret_cast<const uint8_t *>(_src);

    srtToMatrix4_x1(dst + 0 * sizeof(Matrix4), src + 0 * sizeof(Srt));
    srtToMatrix4_x1(dst + 1 * sizeof(Matrix4), src + 1 * sizeof(Srt));
    srtToMatrix4_x1(dst + 2 * sizeof(Matrix4), src + 2 * sizeof(Srt));
    srtToMatrix4_x1(dst + 3 * sizeof(Matrix4), src + 3 * sizeof(Srt));
}

void srtToMatrix4_x4_Simd(void *_dst, const void *_src)
{
    using namespace bx;

    simd128_t *dst = reinterpret_cast<simd128_t *>(_dst);
    const simd128_t *src = reinterpret_cast<const simd128_t *>(_src);

    simd128_t rotate[4];
    simd128_t translate[4];
    simd128_t scale[4];

    transpose(rotate, sizeof(simd128_t), src + 0, sizeof(Srt));
    transpose(translate, sizeof(simd128_t), src + 1, sizeof(Srt));
    transpose(scale, sizeof(simd128_t), src + 2, sizeof(Srt));

    const simd128_t rx = simd_ld<simd128_t>(rotate + 0);
    const simd128_t ry = simd_ld<simd128_t>(rotate + 1);
    const simd128_t rz = simd_ld<simd128_t>(rotate + 2);
    const simd128_t rw = simd_ld<simd128_t>(rotate + 3);

    const simd128_t tx = simd_ld<simd128_t>(translate + 0);
    const simd128_t ty = simd_ld<simd128_t>(translate + 1);
    const simd128_t tz = simd_ld<simd128_t>(translate + 2);

    const simd128_t sx = simd_ld<simd128_t>(scale + 0);
    const simd128_t sy = simd_ld<simd128_t>(scale + 1);
    const simd128_t sz = simd_ld<simd128_t>(scale + 2);

    const simd128_t zero = simd_splat(0.0f);
    const simd128_t one = simd_splat(1.0f);
    const simd128_t two = simd_splat(2.0f);

    const simd128_t xx = simd_mul(rx, rx);
    const simd128_t xx2 = simd_mul(two, xx);
    const simd128_t yy = simd_mul(ry, ry);
    const simd128_t yy2 = simd_mul(two, yy);
    const simd128_t zz = simd_mul(rz, rz);
    const simd128_t zz2 = simd_mul(two, zz);
    const simd128_t yx = simd_mul(ry, rx);
    const simd128_t yx2 = simd_mul(two, yx);
    const simd128_t yz = simd_mul(ry, rz);
    const simd128_t yz2 = simd_mul(two, yz);
    const simd128_t yw = simd_mul(ry, rw);
    const simd128_t yw2 = simd_mul(two, yw);
    const simd128_t wz = simd_mul(rw, rz);
    const simd128_t wz2 = simd_mul(two, wz);
    const simd128_t wx = simd_mul(rw, rx);
    const simd128_t wx2 = simd_mul(two, wx);
    const simd128_t xz = simd_mul(rx, rz);
    const simd128_t xz2 = simd_mul(two, xz);
    const simd128_t t0x = simd_sub(one, yy2);
    const simd128_t r0x = simd_sub(t0x, zz2);
    const simd128_t r0y = simd_add(yx2, wz2);
    const simd128_t r0z = simd_sub(xz2, yw2);
    const simd128_t r1x = simd_sub(yx2, wz2);
    const simd128_t omxx2 = simd_sub(one, xx2);
    const simd128_t r1y = simd_sub(omxx2, zz2);
    const simd128_t r1z = simd_add(yz2, wx2);
    const simd128_t r2x = simd_add(xz2, yw2);
    const simd128_t r2y = simd_sub(yz2, wx2);
    const simd128_t r2z = simd_sub(omxx2, yy2);

    simd128_t tmp[4];
    tmp[0] = simd_mul(r0x, sx);
    tmp[1] = simd_mul(r0y, sx);
    tmp[2] = simd_mul(r0z, sx);
    tmp[3] = zero;
    transpose(dst + 0, sizeof(Matrix4), tmp);

    tmp[0] = simd_mul(r1x, sy);
    tmp[1] = simd_mul(r1y, sy);
    tmp[2] = simd_mul(r1z, sy);
    tmp[3] = zero;
    transpose(dst + 1, sizeof(Matrix4), tmp);

    tmp[0] = simd_mul(r2x, sz);
    tmp[1] = simd_mul(r2y, sz);
    tmp[2] = simd_mul(r2z, sz);
    tmp[3] = zero;
    transpose(dst + 2, sizeof(Matrix4), tmp);

    tmp[0] = tx;
    tmp[1] = ty;
    tmp[2] = tz;
    tmp[3] = one;
    transpose(dst + 3, sizeof(Matrix4), tmp);
}

void srtToMatrix4(void *_dst, const void *_src, uint32_t _num)
{
    uint8_t *dst = reinterpret_cast<uint8_t *>(_dst);
    const uint8_t *src = reinterpret_cast<const uint8_t *>(_src);

    if (!bx::isAligned(src, 16))
    {
        for (uint32_t ii = 0, num = _num / 4; ii < num; ++ii)
        {
            srtToMatrix4_x4_Ref(dst, src);
            src += 4 * sizeof(Srt);
            dst += 4 * sizeof(Matrix4);
        }
    }
    else
    {
        for (uint32_t ii = 0, num = _num / 4; ii < num; ++ii)
        {
            srtToMatrix4_x4_Simd(dst, src);
            src += 4 * sizeof(Srt);
            dst += 4 * sizeof(Matrix4);
        }
    }

    for (uint32_t ii = 0, num = _num & 3; ii < num; ++ii)
    {
        srtToMatrix4_x1(dst, src);
        src += sizeof(Srt);
        dst += sizeof(Matrix4);
    }
}

const uint32_t g_uniformTypeSize[UniformType::Count + 1] = {
    sizeof(int32_t), 0, 4 * sizeof(float), 3 * 3 * sizeof(float), 4 * 4 * sizeof(float), 1,
};

struct CapsFlags
{
    uint64_t m_flag;
    const char *m_str;
};

static const CapsFlags s_capsFlags[] = {
#define CAPS_FLAGS(_x)                                                                                                 \
    {                                                                                                                  \
        _x, #_x                                                                                                        \
    }
    CAPS_FLAGS(BGFX_CAPS_ALPHA_TO_COVERAGE),
    CAPS_FLAGS(BGFX_CAPS_BLEND_INDEPENDENT),
    CAPS_FLAGS(BGFX_CAPS_COMPUTE),
    CAPS_FLAGS(BGFX_CAPS_CONSERVATIVE_RASTER),
    CAPS_FLAGS(BGFX_CAPS_DRAW_INDIRECT),
    CAPS_FLAGS(BGFX_CAPS_FRAGMENT_DEPTH),
    CAPS_FLAGS(BGFX_CAPS_FRAGMENT_ORDERING),
    CAPS_FLAGS(BGFX_CAPS_GRAPHICS_DEBUGGER),
    CAPS_FLAGS(BGFX_CAPS_HDR10),
    CAPS_FLAGS(BGFX_CAPS_HIDPI),
    CAPS_FLAGS(BGFX_CAPS_INDEX32),
    CAPS_FLAGS(BGFX_CAPS_INSTANCING),
    CAPS_FLAGS(BGFX_CAPS_OCCLUSION_QUERY),
    CAPS_FLAGS(BGFX_CAPS_RENDERER_MULTITHREADED),
    CAPS_FLAGS(BGFX_CAPS_SWAP_CHAIN),
    CAPS_FLAGS(BGFX_CAPS_TEXTURE_2D_ARRAY),
    CAPS_FLAGS(BGFX_CAPS_TEXTURE_3D),
    CAPS_FLAGS(BGFX_CAPS_TEXTURE_BLIT),
    CAPS_FLAGS(BGFX_CAPS_TEXTURE_COMPARE_ALL),
    CAPS_FLAGS(BGFX_CAPS_TEXTURE_COMPARE_LEQUAL),
    CAPS_FLAGS(BGFX_CAPS_TEXTURE_CUBE_ARRAY),
    CAPS_FLAGS(BGFX_CAPS_TEXTURE_DIRECT_ACCESS),
    CAPS_FLAGS(BGFX_CAPS_TEXTURE_READ_BACK),
    CAPS_FLAGS(BGFX_CAPS_VERTEX_ATTRIB_HALF),
    CAPS_FLAGS(BGFX_CAPS_VERTEX_ATTRIB_UINT10),
    CAPS_FLAGS(BGFX_CAPS_VERTEX_ID),
#undef CAPS_FLAGS
};


// static const char *s_topologyName[] = {
//     "Triangles", "TriStrip", "Lines", "LineStrip", "Points",
// };
// BX_STATIC_ASSERT(Topology::Count == BX_COUNTOF(s_topologyName));

static TextureFormat::Enum s_emulatedFormats[] = {
    TextureFormat::BC1,     TextureFormat::BC2,     TextureFormat::BC3,     TextureFormat::BC4,
    TextureFormat::BC5,     TextureFormat::ETC1,    TextureFormat::ETC2,    TextureFormat::ETC2A,
    TextureFormat::ETC2A1,  TextureFormat::PTC12,   TextureFormat::PTC14,   TextureFormat::PTC12A,
    TextureFormat::PTC14A,  TextureFormat::PTC22,   TextureFormat::PTC24,   TextureFormat::ATC,
    TextureFormat::ATCE,    TextureFormat::ATCI,    TextureFormat::ASTC4x4, TextureFormat::ASTC5x5,
    TextureFormat::ASTC6x6, TextureFormat::ASTC8x5, TextureFormat::ASTC8x6, TextureFormat::ASTC10x5,
    TextureFormat::BGRA8, // GL doesn't support BGRA8 without extensions.
    TextureFormat::RGBA8, // D3D9 doesn't support RGBA8
};

} // namespace bgfx

#if BX_PLATFORM_WINDOWS
extern "C"
{
    // When laptop setup has integrated and discrete GPU, following driver workarounds will
    // select discrete GPU:

    // Reference(s):
    // -
    // https://web.archive.org/web/20180722051003/https://docs.nvidia.com/gameworks/content/technologies/desktop/optimus.htm
    //
    __declspec(dllexport) uint32_t NvOptimusEnablement = UINT32_C(1);

    // Reference(s):
    // - https://web.archive.org/web/20180722051032/https://gpuopen.com/amdpowerxpressrequesthighperformance/
    //
    __declspec(dllexport) uint32_t AmdPowerXpressRequestHighPerformance = UINT32_C(1);
}
#endif // BX_PLATFORM_WINDOWS

#define BGFX_TEXTURE_FORMAT_BIMG(_fmt)                                                                                 \
    BX_STATIC_ASSERT(uint32_t(bgfx::TextureFormat::_fmt) == uint32_t(bimg::TextureFormat::_fmt))

BGFX_TEXTURE_FORMAT_BIMG(BC1);
BGFX_TEXTURE_FORMAT_BIMG(BC2);
BGFX_TEXTURE_FORMAT_BIMG(BC3);
BGFX_TEXTURE_FORMAT_BIMG(BC4);
BGFX_TEXTURE_FORMAT_BIMG(BC5);
BGFX_TEXTURE_FORMAT_BIMG(BC6H);
BGFX_TEXTURE_FORMAT_BIMG(BC7);
BGFX_TEXTURE_FORMAT_BIMG(ETC1);
BGFX_TEXTURE_FORMAT_BIMG(ETC2);
BGFX_TEXTURE_FORMAT_BIMG(ETC2A);
BGFX_TEXTURE_FORMAT_BIMG(ETC2A1);
BGFX_TEXTURE_FORMAT_BIMG(PTC12);
BGFX_TEXTURE_FORMAT_BIMG(PTC14);
BGFX_TEXTURE_FORMAT_BIMG(PTC12A);
BGFX_TEXTURE_FORMAT_BIMG(PTC14A);
BGFX_TEXTURE_FORMAT_BIMG(PTC22);
BGFX_TEXTURE_FORMAT_BIMG(PTC24);
BGFX_TEXTURE_FORMAT_BIMG(ATC);
BGFX_TEXTURE_FORMAT_BIMG(ATCE);
BGFX_TEXTURE_FORMAT_BIMG(ATCI);
BGFX_TEXTURE_FORMAT_BIMG(ASTC4x4);
BGFX_TEXTURE_FORMAT_BIMG(ASTC5x5);
BGFX_TEXTURE_FORMAT_BIMG(ASTC6x6);
BGFX_TEXTURE_FORMAT_BIMG(ASTC8x5);
BGFX_TEXTURE_FORMAT_BIMG(ASTC8x6);
BGFX_TEXTURE_FORMAT_BIMG(ASTC10x5);
BGFX_TEXTURE_FORMAT_BIMG(Unknown);
BGFX_TEXTURE_FORMAT_BIMG(R1);
BGFX_TEXTURE_FORMAT_BIMG(A8);
BGFX_TEXTURE_FORMAT_BIMG(R8);
BGFX_TEXTURE_FORMAT_BIMG(R8I);
BGFX_TEXTURE_FORMAT_BIMG(R8U);
BGFX_TEXTURE_FORMAT_BIMG(R8S);
BGFX_TEXTURE_FORMAT_BIMG(R16);
BGFX_TEXTURE_FORMAT_BIMG(R16I);
BGFX_TEXTURE_FORMAT_BIMG(R16U);
BGFX_TEXTURE_FORMAT_BIMG(R16F);
BGFX_TEXTURE_FORMAT_BIMG(R16S);
BGFX_TEXTURE_FORMAT_BIMG(R32I);
BGFX_TEXTURE_FORMAT_BIMG(R32U);
BGFX_TEXTURE_FORMAT_BIMG(R32F);
BGFX_TEXTURE_FORMAT_BIMG(RG8);
BGFX_TEXTURE_FORMAT_BIMG(RG8I);
BGFX_TEXTURE_FORMAT_BIMG(RG8U);
BGFX_TEXTURE_FORMAT_BIMG(RG8S);
BGFX_TEXTURE_FORMAT_BIMG(RG16);
BGFX_TEXTURE_FORMAT_BIMG(RG16I);
BGFX_TEXTURE_FORMAT_BIMG(RG16U);
BGFX_TEXTURE_FORMAT_BIMG(RG16F);
BGFX_TEXTURE_FORMAT_BIMG(RG16S);
BGFX_TEXTURE_FORMAT_BIMG(RG32I);
BGFX_TEXTURE_FORMAT_BIMG(RG32U);
BGFX_TEXTURE_FORMAT_BIMG(RG32F);
BGFX_TEXTURE_FORMAT_BIMG(RGB8);
BGFX_TEXTURE_FORMAT_BIMG(RGB8I);
BGFX_TEXTURE_FORMAT_BIMG(RGB8U);
BGFX_TEXTURE_FORMAT_BIMG(RGB8S);
BGFX_TEXTURE_FORMAT_BIMG(RGB9E5F);
BGFX_TEXTURE_FORMAT_BIMG(BGRA8);
BGFX_TEXTURE_FORMAT_BIMG(RGBA8);
BGFX_TEXTURE_FORMAT_BIMG(RGBA8I);
BGFX_TEXTURE_FORMAT_BIMG(RGBA8U);
BGFX_TEXTURE_FORMAT_BIMG(RGBA8S);
BGFX_TEXTURE_FORMAT_BIMG(RGBA16);
BGFX_TEXTURE_FORMAT_BIMG(RGBA16I);
BGFX_TEXTURE_FORMAT_BIMG(RGBA16U);
BGFX_TEXTURE_FORMAT_BIMG(RGBA16F);
BGFX_TEXTURE_FORMAT_BIMG(RGBA16S);
BGFX_TEXTURE_FORMAT_BIMG(RGBA32I);
BGFX_TEXTURE_FORMAT_BIMG(RGBA32U);
BGFX_TEXTURE_FORMAT_BIMG(RGBA32F);
BGFX_TEXTURE_FORMAT_BIMG(R5G6B5);
BGFX_TEXTURE_FORMAT_BIMG(RGBA4);
BGFX_TEXTURE_FORMAT_BIMG(RGB5A1);
BGFX_TEXTURE_FORMAT_BIMG(RGB10A2);
BGFX_TEXTURE_FORMAT_BIMG(RG11B10F);
BGFX_TEXTURE_FORMAT_BIMG(UnknownDepth);
BGFX_TEXTURE_FORMAT_BIMG(D16);
BGFX_TEXTURE_FORMAT_BIMG(D24);
BGFX_TEXTURE_FORMAT_BIMG(D24S8);
BGFX_TEXTURE_FORMAT_BIMG(D32);
BGFX_TEXTURE_FORMAT_BIMG(D16F);
BGFX_TEXTURE_FORMAT_BIMG(D24F);
BGFX_TEXTURE_FORMAT_BIMG(D32F);
BGFX_TEXTURE_FORMAT_BIMG(D0S8);
BGFX_TEXTURE_FORMAT_BIMG(Count);

#undef BGFX_TEXTURE_FORMAT_BIMG

// #include <bgfx/c99/bgfx.h>

#define FLAGS_MASK_TEST(_flags, _mask) ((_flags) == ((_flags) & (_mask)))

BX_STATIC_ASSERT(FLAGS_MASK_TEST(0 | BGFX_SAMPLER_INTERNAL_DEFAULT | BGFX_SAMPLER_INTERNAL_SHARED,
                                 BGFX_SAMPLER_RESERVED_MASK));

BX_STATIC_ASSERT(FLAGS_MASK_TEST(0 | BGFX_RESET_INTERNAL_FORCE, BGFX_RESET_RESERVED_MASK));

BX_STATIC_ASSERT(FLAGS_MASK_TEST(0 | BGFX_STATE_INTERNAL_SCISSOR | BGFX_STATE_INTERNAL_OCCLUSION_QUERY,
                                 BGFX_STATE_RESERVED_MASK));

BX_STATIC_ASSERT(FLAGS_MASK_TEST(0 | BGFX_SUBMIT_INTERNAL_OCCLUSION_VISIBLE, BGFX_SUBMIT_RESERVED_MASK));

BX_STATIC_ASSERT((0 | BGFX_STATE_ALPHA_REF_MASK | BGFX_STATE_BLEND_ALPHA_TO_COVERAGE | BGFX_STATE_BLEND_EQUATION_MASK |
                  BGFX_STATE_BLEND_INDEPENDENT | BGFX_STATE_BLEND_MASK | BGFX_STATE_CONSERVATIVE_RASTER |
                  BGFX_STATE_CULL_MASK | BGFX_STATE_DEPTH_TEST_MASK | BGFX_STATE_FRONT_CCW | BGFX_STATE_LINEAA |
                  BGFX_STATE_MSAA | BGFX_STATE_POINT_SIZE_MASK | BGFX_STATE_PT_MASK | BGFX_STATE_RESERVED_MASK |
                  BGFX_STATE_WRITE_MASK) ==
                 (0 ^ BGFX_STATE_ALPHA_REF_MASK ^ BGFX_STATE_BLEND_ALPHA_TO_COVERAGE ^ BGFX_STATE_BLEND_EQUATION_MASK ^
                  BGFX_STATE_BLEND_INDEPENDENT ^ BGFX_STATE_BLEND_MASK ^ BGFX_STATE_CONSERVATIVE_RASTER ^
                  BGFX_STATE_CULL_MASK ^ BGFX_STATE_DEPTH_TEST_MASK ^ BGFX_STATE_FRONT_CCW ^ BGFX_STATE_LINEAA ^
                  BGFX_STATE_MSAA ^ BGFX_STATE_POINT_SIZE_MASK ^ BGFX_STATE_PT_MASK ^ BGFX_STATE_RESERVED_MASK ^
                  BGFX_STATE_WRITE_MASK));

BX_STATIC_ASSERT(FLAGS_MASK_TEST(BGFX_CAPS_TEXTURE_COMPARE_LEQUAL, BGFX_CAPS_TEXTURE_COMPARE_ALL));

BX_STATIC_ASSERT((0 | BGFX_CAPS_ALPHA_TO_COVERAGE | BGFX_CAPS_BLEND_INDEPENDENT | BGFX_CAPS_COMPUTE |
                  BGFX_CAPS_CONSERVATIVE_RASTER | BGFX_CAPS_DRAW_INDIRECT | BGFX_CAPS_FRAGMENT_DEPTH |
                  BGFX_CAPS_FRAGMENT_ORDERING | BGFX_CAPS_GRAPHICS_DEBUGGER | BGFX_CAPS_HDR10 | BGFX_CAPS_HIDPI |
                  BGFX_CAPS_INDEX32 | BGFX_CAPS_INSTANCING | BGFX_CAPS_OCCLUSION_QUERY |
                  BGFX_CAPS_RENDERER_MULTITHREADED | BGFX_CAPS_SWAP_CHAIN | BGFX_CAPS_TEXTURE_2D_ARRAY |
                  BGFX_CAPS_TEXTURE_3D | BGFX_CAPS_TEXTURE_BLIT | BGFX_CAPS_TEXTURE_CUBE_ARRAY |
                  BGFX_CAPS_TEXTURE_DIRECT_ACCESS | BGFX_CAPS_TEXTURE_READ_BACK | BGFX_CAPS_VERTEX_ATTRIB_HALF |
                  BGFX_CAPS_VERTEX_ATTRIB_UINT10 | BGFX_CAPS_VERTEX_ID) ==
                 (0 ^ BGFX_CAPS_ALPHA_TO_COVERAGE ^ BGFX_CAPS_BLEND_INDEPENDENT ^ BGFX_CAPS_COMPUTE ^
                  BGFX_CAPS_CONSERVATIVE_RASTER ^ BGFX_CAPS_DRAW_INDIRECT ^ BGFX_CAPS_FRAGMENT_DEPTH ^
                  BGFX_CAPS_FRAGMENT_ORDERING ^ BGFX_CAPS_GRAPHICS_DEBUGGER ^ BGFX_CAPS_HDR10 ^ BGFX_CAPS_HIDPI ^
                  BGFX_CAPS_INDEX32 ^ BGFX_CAPS_INSTANCING ^ BGFX_CAPS_OCCLUSION_QUERY ^
                  BGFX_CAPS_RENDERER_MULTITHREADED ^ BGFX_CAPS_SWAP_CHAIN ^ BGFX_CAPS_TEXTURE_2D_ARRAY ^
                  BGFX_CAPS_TEXTURE_3D ^ BGFX_CAPS_TEXTURE_BLIT ^ BGFX_CAPS_TEXTURE_CUBE_ARRAY ^
                  BGFX_CAPS_TEXTURE_DIRECT_ACCESS ^ BGFX_CAPS_TEXTURE_READ_BACK ^ BGFX_CAPS_VERTEX_ATTRIB_HALF ^
                  BGFX_CAPS_VERTEX_ATTRIB_UINT10 ^ BGFX_CAPS_VERTEX_ID));

#undef FLAGS_MASK_TEST
