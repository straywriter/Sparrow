#pragma once

#include "Render/RenderConfig.h"
#include "Render/Render.h"

namespace bgfx
{

extern InternalData g_internalData;
extern PlatformData g_platformData;
extern bool g_platformDataChangedSinceReset;

extern const uint32_t g_uniformTypeSize[UniformType::Count + 1];
extern CallbackI *g_callback;
extern bx::AllocatorI *g_allocator;
extern Caps g_caps;

#if BGFX_CONFIG_MAX_DRAW_CALLS < (64 << 10)
typedef uint16_t RenderItemCount;
#else
typedef uint32_t RenderItemCount;
#endif // BGFX_CONFIG_MAX_DRAW_CALLS < (64<<10)

struct Condition
{
    enum Enum
    {
        LessEqual,
        GreaterEqual,
    };
};

bool windowsVersionIs(Condition::Enum _op, uint32_t _version);

constexpr bool isShaderType(uint32_t _magic, char _type)
{
    return uint32_t(_type) == (_magic & BX_MAKEFOURCC(0xff, 0, 0, 0));
}

inline bool isShaderBin(uint32_t _magic)
{
    return BX_MAKEFOURCC(0, 'S', 'H', 0) == (_magic & BX_MAKEFOURCC(0, 0xff, 0xff, 0)) &&
           (isShaderType(_magic, 'C') || isShaderType(_magic, 'F') || isShaderType(_magic, 'V'));
}

inline bool isShaderVerLess(uint32_t _magic, uint8_t _version)
{
    return (_magic & BX_MAKEFOURCC(0, 0, 0, 0xff)) < BX_MAKEFOURCC(0, 0, 0, _version);
}

const char *getShaderTypeName(uint32_t _magic);

//----------------------

typedef bx::StringT<&g_allocator> String;

struct ProfilerScope
{
    ProfilerScope(const char *_name, uint32_t _abgr, const char *_filePath, uint16_t _line)
    {
        g_callback->profilerBeginLiteral(_name, _abgr, _filePath, _line);
    }

    ~ProfilerScope()
    {
        g_callback->profilerEnd();
    }
};

void setGraphicsDebuggerPresent(bool _present);
bool isGraphicsDebuggerPresent();
void release(const Memory *_mem);
const char *getAttribName(Attrib::Enum _attr);
const char *getAttribNameShort(Attrib::Enum _attr);
void getTextureSizeFromRatio(BackbufferRatio::Enum _ratio, uint16_t &_width, uint16_t &_height);
TextureFormat::Enum getViableTextureFormat(const bimg::ImageContainer &_imageContainer);
const char *getName(TextureFormat::Enum _fmt);
const char *getName(UniformHandle _handle);
const char *getName(ShaderHandle _handle);
const char *getName(Topology::Enum _topology);

template <typename Ty> inline void release(Ty)
{
}

template <> inline void release(Memory *_mem)
{
    release((const Memory *)_mem);
}

inline uint32_t castfu(float _value)
{
    union {
        float fl;
        uint32_t ui;
    } un;
    un.fl = _value;
    return un.ui;
}

inline uint64_t packStencil(uint32_t _fstencil, uint32_t _bstencil)
{
    return (uint64_t(_bstencil) << 32) | uint64_t(_fstencil);
}

inline uint32_t unpackStencil(uint8_t _0or1, uint64_t _stencil)
{
    return uint32_t((_stencil >> (32 * _0or1)));
}

inline bool needBorderColor(uint64_t _flags)
{
    return BGFX_SAMPLER_U_BORDER == (_flags & BGFX_SAMPLER_U_BORDER) ||
           BGFX_SAMPLER_V_BORDER == (_flags & BGFX_SAMPLER_V_BORDER) ||
           BGFX_SAMPLER_W_BORDER == (_flags & BGFX_SAMPLER_W_BORDER);
}

inline uint8_t calcNumMips(bool _hasMips, uint16_t _width, uint16_t _height, uint16_t _depth = 1)
{
    if (_hasMips)
    {
        const uint32_t max = bx::max(_width, _height, _depth);
        const uint32_t num = 1 + uint32_t(bx::log2<int32_t>(max));

        return uint8_t(num);
    }

    return 1;
}

/// Dump vertex layout into debug output.
void dump(const VertexLayout &_layout);

struct RendererContextI;

extern void blit(RendererContextI *_renderCtx, TextVideoMemBlitter &_blitter, const TextVideoMem &_mem);

inline void blit(RendererContextI *_renderCtx, TextVideoMemBlitter &_blitter, const TextVideoMem *_mem)
{
    blit(_renderCtx, _blitter, *_mem);
}

const char *getUniformTypeName(UniformType::Enum _enum);
UniformType::Enum nameToUniformTypeEnum(const char *_name);
const char *getPredefinedUniformName(PredefinedUniform::Enum _enum);
PredefinedUniform::Enum nameToPredefinedUniformEnum(const char *_name);

//
constexpr uint8_t kSortKeyViewNumBits = 10;
constexpr uint8_t kSortKeyViewBitShift = 64 - kSortKeyViewNumBits;
constexpr uint64_t kSortKeyViewMask = uint64_t(BGFX_CONFIG_MAX_VIEWS - 1) << kSortKeyViewBitShift;

constexpr uint8_t kSortKeyDrawBitShift = kSortKeyViewBitShift - 1;
constexpr uint64_t kSortKeyDrawBit = uint64_t(1) << kSortKeyDrawBitShift;

//
constexpr uint8_t kSortKeyDrawTypeNumBits = 2;
constexpr uint8_t kSortKeyDrawTypeBitShift = kSortKeyDrawBitShift - kSortKeyDrawTypeNumBits;
constexpr uint64_t kSortKeyDrawTypeMask = uint64_t(3) << kSortKeyDrawTypeBitShift;

constexpr uint64_t kSortKeyDrawTypeProgram = uint64_t(0) << kSortKeyDrawTypeBitShift;
constexpr uint64_t kSortKeyDrawTypeDepth = uint64_t(1) << kSortKeyDrawTypeBitShift;
constexpr uint64_t kSortKeyDrawTypeSequence = uint64_t(2) << kSortKeyDrawTypeBitShift;

//
constexpr uint8_t kSortKeyTransNumBits = 2;

constexpr uint8_t kSortKeyDraw0BlendShift = kSortKeyDrawTypeBitShift - kSortKeyTransNumBits;
constexpr uint64_t kSortKeyDraw0BlendMask = uint64_t(0x3) << kSortKeyDraw0BlendShift;

constexpr uint8_t kSortKeyDraw0ProgramShift = kSortKeyDraw0BlendShift - BGFX_CONFIG_SORT_KEY_NUM_BITS_PROGRAM;
constexpr uint64_t kSortKeyDraw0ProgramMask = uint64_t(BGFX_CONFIG_MAX_PROGRAMS - 1) << kSortKeyDraw0ProgramShift;

constexpr uint8_t kSortKeyDraw0DepthShift = kSortKeyDraw0ProgramShift - BGFX_CONFIG_SORT_KEY_NUM_BITS_DEPTH;
constexpr uint64_t kSortKeyDraw0DepthMask = ((uint64_t(1) << BGFX_CONFIG_SORT_KEY_NUM_BITS_DEPTH) - 1)
                                            << kSortKeyDraw0DepthShift;

//
constexpr uint8_t kSortKeyDraw1DepthShift = kSortKeyDrawTypeBitShift - BGFX_CONFIG_SORT_KEY_NUM_BITS_DEPTH;
constexpr uint64_t kSortKeyDraw1DepthMask = ((uint64_t(1) << BGFX_CONFIG_SORT_KEY_NUM_BITS_DEPTH) - 1)
                                            << kSortKeyDraw1DepthShift;

constexpr uint8_t kSortKeyDraw1BlendShift = kSortKeyDraw1DepthShift - kSortKeyTransNumBits;
constexpr uint64_t kSortKeyDraw1BlendMask = uint64_t(0x3) << kSortKeyDraw1BlendShift;

constexpr uint8_t kSortKeyDraw1ProgramShift = kSortKeyDraw1BlendShift - BGFX_CONFIG_SORT_KEY_NUM_BITS_PROGRAM;
constexpr uint64_t kSortKeyDraw1ProgramMask = uint64_t(BGFX_CONFIG_MAX_PROGRAMS - 1) << kSortKeyDraw1ProgramShift;

//
constexpr uint8_t kSortKeyDraw2SeqShift = kSortKeyDrawTypeBitShift - BGFX_CONFIG_SORT_KEY_NUM_BITS_SEQ;
constexpr uint64_t kSortKeyDraw2SeqMask = ((uint64_t(1) << BGFX_CONFIG_SORT_KEY_NUM_BITS_SEQ) - 1)
                                          << kSortKeyDraw2SeqShift;

constexpr uint8_t kSortKeyDraw2BlendShift = kSortKeyDraw2SeqShift - kSortKeyTransNumBits;
constexpr uint64_t kSortKeyDraw2BlendMask = uint64_t(0x3) << kSortKeyDraw2BlendShift;

constexpr uint8_t kSortKeyDraw2ProgramShift = kSortKeyDraw2BlendShift - BGFX_CONFIG_SORT_KEY_NUM_BITS_PROGRAM;
constexpr uint64_t kSortKeyDraw2ProgramMask = uint64_t(BGFX_CONFIG_MAX_PROGRAMS - 1) << kSortKeyDraw2ProgramShift;

//
constexpr uint8_t kSortKeyComputeSeqShift = kSortKeyDrawBitShift - BGFX_CONFIG_SORT_KEY_NUM_BITS_SEQ;
constexpr uint64_t kSortKeyComputeSeqMask = ((uint64_t(1) << BGFX_CONFIG_SORT_KEY_NUM_BITS_SEQ) - 1)
                                            << kSortKeyComputeSeqShift;

constexpr uint8_t kSortKeyComputeProgramShift = kSortKeyComputeSeqShift - BGFX_CONFIG_SORT_KEY_NUM_BITS_PROGRAM;
constexpr uint64_t kSortKeyComputeProgramMask = uint64_t(BGFX_CONFIG_MAX_PROGRAMS - 1) << kSortKeyComputeProgramShift;

BX_STATIC_ASSERT(BGFX_CONFIG_MAX_VIEWS <= (1 << kSortKeyViewNumBits));
BX_STATIC_ASSERT((BGFX_CONFIG_MAX_PROGRAMS & (BGFX_CONFIG_MAX_PROGRAMS - 1)) == 0); // Must be power of 2.
BX_STATIC_ASSERT((0 // Render key mask shouldn't overlap.
                  | kSortKeyViewMask | kSortKeyDrawBit | kSortKeyDrawTypeMask | kSortKeyDraw0BlendMask |
                  kSortKeyDraw0ProgramMask | kSortKeyDraw0DepthMask) ==
                 (0 ^ kSortKeyViewMask ^ kSortKeyDrawBit ^ kSortKeyDrawTypeMask ^ kSortKeyDraw0BlendMask ^
                  kSortKeyDraw0ProgramMask ^ kSortKeyDraw0DepthMask));
BX_STATIC_ASSERT((0 // Render key mask shouldn't overlap.
                  | kSortKeyViewMask | kSortKeyDrawBit | kSortKeyDrawTypeMask | kSortKeyDraw1DepthMask |
                  kSortKeyDraw1BlendMask | kSortKeyDraw1ProgramMask) ==
                 (0 ^ kSortKeyViewMask ^ kSortKeyDrawBit ^ kSortKeyDrawTypeMask ^ kSortKeyDraw1DepthMask ^
                  kSortKeyDraw1BlendMask ^ kSortKeyDraw1ProgramMask));
BX_STATIC_ASSERT((0 // Render key mask shouldn't overlap.
                  | kSortKeyViewMask | kSortKeyDrawBit | kSortKeyDrawTypeMask | kSortKeyDraw2SeqMask |
                  kSortKeyDraw2BlendMask | kSortKeyDraw2ProgramMask) ==
                 (0 ^ kSortKeyViewMask ^ kSortKeyDrawBit ^ kSortKeyDrawTypeMask ^ kSortKeyDraw2SeqMask ^
                  kSortKeyDraw2BlendMask ^ kSortKeyDraw2ProgramMask));
BX_STATIC_ASSERT((0 // Compute key mask shouldn't overlap.
                  | kSortKeyViewMask | kSortKeyDrawBit | kSortKeyComputeSeqShift | kSortKeyComputeProgramMask) ==
                 (0 ^ kSortKeyViewMask ^ kSortKeyDrawBit ^ kSortKeyComputeSeqShift ^ kSortKeyComputeProgramMask));

// |               3               2               1               0|
// |fedcba9876543210fedcba9876543210fedcba9876543210fedcba9876543210| Common
// |vvvvvvvvd                                                       |
// |       ^^                                                       |
// |       ||                                                       |
// |  view-+|                                                       |
// |        +-draw                                                  |
// |----------------------------------------------------------------| Draw Key 0 - Sort by program
// |        |kkttpppppppppdddddddddddddddddddddddddddddddd          |
// |        |   ^        ^                               ^          |
// |        |   |        |                               |          |
// |        |   +-blend  +-program                 depth-+          |
// |        |                                                       |
// |----------------------------------------------------------------| Draw Key 1 - Sort by depth
// |        |kkddddddddddddddddddddddddddddddddttppppppppp          |
// |        |                                ^^ ^        ^          |
// |        |                                || +-trans  |          |
// |        |                          depth-+   program-+          |
// |        |                                                       |
// |----------------------------------------------------------------| Draw Key 2 - Sequential
// |        |kkssssssssssssssssssssttppppppppp                      |
// |        |                     ^ ^        ^                      |
// |        |                     | |        |                      |
// |        |                 seq-+ +-trans  +-program              |
// |        |                                                       |
// |----------------------------------------------------------------| Compute Key
// |        |ssssssssssssssssssssppppppppp                          |
// |        |                   ^        ^                          |
// |        |                   |        |                          |
// |        |               seq-+        +-program                  |
// |        |                                                       |
// |--------+-------------------------------------------------------|
//

struct SortKey
{
    enum Enum
    {
        SortProgram,
        SortDepth,
        SortSequence,
    };

    uint64_t encodeDraw(Enum _type)
    {
        switch (_type)
        {
        case SortProgram: {
            const uint64_t depth = (uint64_t(m_depth) << kSortKeyDraw0DepthShift) & kSortKeyDraw0DepthMask;
            const uint64_t program = (uint64_t(m_program.idx) << kSortKeyDraw0ProgramShift) & kSortKeyDraw0ProgramMask;
            const uint64_t blend = (uint64_t(m_blend) << kSortKeyDraw0BlendShift) & kSortKeyDraw0BlendMask;
            const uint64_t view = (uint64_t(m_view) << kSortKeyViewBitShift) & kSortKeyViewMask;
            const uint64_t key = view | kSortKeyDrawBit | kSortKeyDrawTypeProgram | blend | program | depth;

            return key;
        }
        break;

        case SortDepth: {
            const uint64_t depth = (uint64_t(m_depth) << kSortKeyDraw1DepthShift) & kSortKeyDraw1DepthMask;
            const uint64_t program = (uint64_t(m_program.idx) << kSortKeyDraw1ProgramShift) & kSortKeyDraw1ProgramMask;
            const uint64_t blend = (uint64_t(m_blend) << kSortKeyDraw1BlendShift) & kSortKeyDraw1BlendMask;
            const uint64_t view = (uint64_t(m_view) << kSortKeyViewBitShift) & kSortKeyViewMask;
            const uint64_t key = view | kSortKeyDrawBit | kSortKeyDrawTypeDepth | depth | blend | program;
            return key;
        }
        break;

        case SortSequence: {
            const uint64_t seq = (uint64_t(m_seq) << kSortKeyDraw2SeqShift) & kSortKeyDraw2SeqMask;
            const uint64_t program = (uint64_t(m_program.idx) << kSortKeyDraw2ProgramShift) & kSortKeyDraw2ProgramMask;
            const uint64_t blend = (uint64_t(m_blend) << kSortKeyDraw2BlendShift) & kSortKeyDraw2BlendMask;
            const uint64_t view = (uint64_t(m_view) << kSortKeyViewBitShift) & kSortKeyViewMask;
            const uint64_t key = view | kSortKeyDrawBit | kSortKeyDrawTypeSequence | seq | blend | program;

            BX_ASSERT(seq == (uint64_t(m_seq) << kSortKeyDraw2SeqShift),
                      "SortKey error, sequence is truncated (m_seq: %d).", m_seq);

            return key;
        }
        break;
        }

        BX_ASSERT(false, "You should not be here.");
        return 0;
    }

    uint64_t encodeCompute()
    {
        const uint64_t program = (uint64_t(m_program.idx) << kSortKeyComputeProgramShift) & kSortKeyComputeProgramMask;
        const uint64_t seq = (uint64_t(m_seq) << kSortKeyComputeSeqShift) & kSortKeyComputeSeqMask;
        const uint64_t view = (uint64_t(m_view) << kSortKeyViewBitShift) & kSortKeyViewMask;
        const uint64_t key = program | seq | view;

        BX_ASSERT(seq == (uint64_t(m_seq) << kSortKeyComputeSeqShift),
                  "SortKey error, sequence is truncated (m_seq: %d).", m_seq);

        return key;
    }

    /// Returns true if item is compute command.
    bool decode(uint64_t _key, ViewId _viewRemap[BGFX_CONFIG_MAX_VIEWS])
    {
        m_view = _viewRemap[(_key & kSortKeyViewMask) >> kSortKeyViewBitShift];
        if (_key & kSortKeyDrawBit)
        {
            uint64_t type = _key & kSortKeyDrawTypeMask;
            if (type == kSortKeyDrawTypeDepth)
            {
                m_program.idx = uint16_t((_key & kSortKeyDraw1ProgramMask) >> kSortKeyDraw1ProgramShift);
                return false;
            }
            else if (type == kSortKeyDrawTypeSequence)
            {
                m_program.idx = uint16_t((_key & kSortKeyDraw2ProgramMask) >> kSortKeyDraw2ProgramShift);
                return false;
            }

            m_program.idx = uint16_t((_key & kSortKeyDraw0ProgramMask) >> kSortKeyDraw0ProgramShift);
            return false; // draw
        }

        m_program.idx = uint16_t((_key & kSortKeyComputeProgramMask) >> kSortKeyComputeProgramShift);
        return true; // compute
    }

    static ViewId decodeView(uint64_t _key)
    {
        return ViewId((_key & kSortKeyViewMask) >> kSortKeyViewBitShift);
    }

    static uint64_t remapView(uint64_t _key, ViewId _viewRemap[BGFX_CONFIG_MAX_VIEWS])
    {
        const ViewId oldView = decodeView(_key);
        const uint64_t view = uint64_t(_viewRemap[oldView]) << kSortKeyViewBitShift;
        const uint64_t key = (_key & ~kSortKeyViewMask) | view;
        return key;
    }

    void reset()
    {
        m_depth = 0;
        m_seq = 0;
        m_program = {0};
        m_view = 0;
        m_blend = 0;
    }

    uint32_t m_depth;
    uint32_t m_seq;
    ProgramHandle m_program;
    ViewId m_view;
    uint8_t m_blend;
};
#undef SORT_KEY_RENDER_DRAW

struct BlitKey
{
    uint32_t encode()
    {
        return 0 | (uint32_t(m_view) << 24) | uint32_t(m_item);
    }

    void decode(uint32_t _key)
    {
        m_item = uint16_t(_key & UINT16_MAX);
        m_view = ViewId(_key >> 24);
    }

    static uint32_t remapView(uint32_t _key, ViewId _viewRemap[BGFX_CONFIG_MAX_VIEWS])
    {
        const ViewId oldView = ViewId(_key >> 24);
        const uint32_t view = uint32_t(_viewRemap[oldView]) << 24;
        const uint32_t key = (_key & ~UINT32_C(0xff000000)) | view;
        return key;
    }

    uint16_t m_item;
    ViewId m_view;
};

} // namespace bgfx