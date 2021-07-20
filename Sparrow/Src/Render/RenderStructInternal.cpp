
#include "RenderStructInternal.h"
#include "StaticVariable.h"

namespace bgfx
{

void charsetFillTexture(const uint8_t *_charset, uint8_t *_rgba, uint32_t _height, uint32_t _pitch, uint32_t _bpp)
{
    for (uint32_t ii = 0; ii < 256; ++ii)
    {
        uint8_t *pix = &_rgba[ii * 8 * _bpp];
        for (uint32_t yy = 0; yy < _height; ++yy)
        {
            for (uint32_t xx = 0; xx < 8; ++xx)
            {
                uint8_t bit = 1 << (7 - xx);
                bx::memSet(&pix[xx * _bpp], _charset[ii * _height + yy] & bit ? 255 : 0, _bpp);
            }

            pix += _pitch;
        }
    }
}

void ClearQuad::init()
{
    BGFX_CHECK_API_THREAD();

    if (RendererType::Noop != g_caps.rendererType)
    {
        m_layout.begin().add(Attrib::Position, 2, AttribType::Float).end();

        ShaderHandle vsh = createEmbeddedShader(s_embeddedShaders, g_caps.rendererType, "vs_clear");
        BX_ASSERT(isValid(vsh), "Failed to create clear quad embedded vertex shader \"vs_clear\"");

        for (uint32_t ii = 0, num = g_caps.limits.maxFBAttachments; ii < num; ++ii)
        {
            char name[32];
            bx::snprintf(name, BX_COUNTOF(name), "fs_clear%d", ii);
            ShaderHandle fsh = createEmbeddedShader(s_embeddedShaders, g_caps.rendererType, name);
            BX_ASSERT(isValid(fsh), "Failed to create clear quad embedded fragment shader \"%s\"", name);

            m_program[ii] = createProgram(vsh, fsh);
            BX_ASSERT(isValid(m_program[ii]), "Failed to create clear quad program.");
            destroy(fsh);
        }

        destroy(vsh);

        struct Vertex
        {
            float m_x;
            float m_y;
        };

        const uint16_t stride = m_layout.m_stride;
        const bgfx::Memory *mem = bgfx::alloc(4 * stride);
        Vertex *vertex = (Vertex *)mem->data;
        BX_ASSERT(stride == sizeof(Vertex), "Stride/Vertex mismatch (stride %d, sizeof(Vertex) %d)", stride,
                  sizeof(Vertex));

        vertex->m_x = -1.0f;
        vertex->m_y = -1.0f;
        vertex++;
        vertex->m_x = 1.0f;
        vertex->m_y = -1.0f;
        vertex++;
        vertex->m_x = -1.0f;
        vertex->m_y = 1.0f;
        vertex++;
        vertex->m_x = 1.0f;
        vertex->m_y = 1.0f;

        m_vb = s_ctx->createVertexBuffer(mem, m_layout, 0);
    }
}

void ClearQuad::shutdown()
{
    BGFX_CHECK_API_THREAD();

    if (RendererType::Noop != g_caps.rendererType)
    {
        for (uint32_t ii = 0, num = g_caps.limits.maxFBAttachments; ii < num; ++ii)
        {
            if (isValid(m_program[ii]))
            {
                destroy(m_program[ii]);
                m_program[ii].idx = kInvalidHandle;
            }
        }

        s_ctx->destroyVertexBuffer(m_vb);
    }
}
void TextVideoMemBlitter::init()
{
    BGFX_CHECK_API_THREAD();
    m_layout.begin()
        .add(Attrib::Position, 3, AttribType::Float)
        .add(Attrib::Color0, 4, AttribType::Uint8, true)
        .add(Attrib::Color1, 4, AttribType::Uint8, true)
        .add(Attrib::TexCoord0, 2, AttribType::Float)
        .end();

    uint16_t width = 2048;
    uint16_t height = 24;
    uint8_t bpp = 1;
    uint32_t pitch = width * bpp;

    const Memory *mem;

    mem = alloc(pitch * height);
    uint8_t *rgba = mem->data;
    charsetFillTexture(vga8x8, rgba, 8, pitch, bpp);
    charsetFillTexture(vga8x16, &rgba[8 * pitch], 16, pitch, bpp);
    m_texture = createTexture2D(width, height, false, 1, TextureFormat::R8,
                                BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT |
                                    BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
                                mem);

    ShaderHandle vsh = createEmbeddedShader(s_embeddedShaders, g_caps.rendererType, "vs_debugfont");
    ShaderHandle fsh = createEmbeddedShader(s_embeddedShaders, g_caps.rendererType, "fs_debugfont");

    BX_ASSERT(isValid(vsh) && isValid(fsh), "Failed to create embedded blit shaders");

    m_program = createProgram(vsh, fsh, true);

    m_vb = s_ctx->createTransientVertexBuffer(numBatchVertices * m_layout.m_stride, &m_layout);
    m_ib = s_ctx->createTransientIndexBuffer(numBatchIndices * 2);
}

void TextVideoMemBlitter::shutdown()
{
    BGFX_CHECK_API_THREAD();

    if (isValid(m_program))
    {
        destroy(m_program);
    }

    destroy(m_texture);
    s_ctx->destroyTransientVertexBuffer(m_vb);
    s_ctx->destroyTransientIndexBuffer(m_ib);
}

static uint8_t parseAttrTo(char *&_ptr, char _to, uint8_t _default)
{
    const bx::StringView str = bx::strFind(_ptr, _to);
    if (!str.isEmpty() && 3 > str.getPtr() - _ptr)
    {
        char tmp[4];

        int32_t len = int32_t(str.getPtr() - _ptr);
        bx::strCopy(tmp, sizeof(tmp), _ptr, len);

        uint32_t attr;
        bx::fromString(&attr, tmp);

        _ptr += len + 1;
        return uint8_t(attr);
    }

    return _default;
}

static uint8_t parseAttr(char *&_ptr, uint8_t _default)
{
    char *ptr = _ptr;
    if (*ptr++ != '[')
    {
        return _default;
    }

    if (0 == bx::strCmp(ptr, "0m", 2))
    {
        _ptr = ptr + 2;
        return _default;
    }

    uint8_t fg = parseAttrTo(ptr, ';', _default & 0xf);
    uint8_t bg = parseAttrTo(ptr, 'm', _default >> 4);

    uint8_t attr = (bg << 4) | fg;
    _ptr = ptr;
    return attr;
}

void TextVideoMem::printfVargs(uint16_t _x, uint16_t _y, uint8_t _attr, const char *_format, va_list _argList)
{
    if (_x < m_width && _y < m_height)
    {
        va_list argListCopy;
        va_copy(argListCopy, _argList);
        uint32_t num = bx::vsnprintf(NULL, 0, _format, argListCopy) + 1;
        char *temp = (char *)alloca(num);
        va_copy(argListCopy, _argList);
        num = bx::vsnprintf(temp, num, _format, argListCopy);

        uint8_t attr = _attr;
        MemSlot *mem = &m_mem[_y * m_width + _x];
        for (uint32_t ii = 0, xx = _x; ii < num && xx < m_width; ++ii)
        {
            char ch = temp[ii];
            if (BX_UNLIKELY(ch == '\x1b'))
            {
                char *ptr = &temp[ii + 1];
                attr = parseAttr(ptr, _attr);
                ii += uint32_t(ptr - &temp[ii + 1]);
            }
            else
            {
                mem->character = ch;
                mem->attribute = attr;
                ++mem;
                ++xx;
            }
        }
    }
}

} // namespace bgfx