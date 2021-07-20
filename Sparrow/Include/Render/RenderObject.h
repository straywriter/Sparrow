#pragma once
#include "Render/RenderConfig.h"
#include "RenderHandle.h"
#include "Render/Render.h"
#include "Render/RTemp.h"

namespace bgfx
{

BX_ALIGN_DECL_16(struct) Srt
{
    float rotate[4];
    float translate[3];
    float pad0;
    float scale[3];
    float pad1;
};

BX_ALIGN_DECL_16(struct) Matrix4
{
    union {
        float val[16];
        bx::float4x4_t f4x4;
    } un;

    void setIdentity()
    {
        bx::memSet(un.val, 0, sizeof(un.val));
        un.val[0] = un.val[5] = un.val[10] = un.val[15] = 1.0f;
    }
};

struct MatrixCache
{
    MatrixCache() : m_num(1)
    {
        m_cache[0].setIdentity();
    }

    void reset()
    {
        m_num = 1;
    }

    uint32_t reserve(uint16_t *_num)
    {
        uint32_t num = *_num;
        uint32_t first = bx::atomicFetchAndAddsat<uint32_t>(&m_num, num, BGFX_CONFIG_MAX_MATRIX_CACHE - 1);
        BX_WARN(first + num < BGFX_CONFIG_MAX_MATRIX_CACHE, "Matrix cache overflow. %d (max: %d)", first + num,
                BGFX_CONFIG_MAX_MATRIX_CACHE);
        num = bx::min(num, BGFX_CONFIG_MAX_MATRIX_CACHE - 1 - first);
        *_num = (uint16_t)num;
        return first;
    }

    uint32_t add(const void *_mtx, uint16_t _num)
    {
        if (NULL != _mtx)
        {
            uint32_t first = reserve(&_num);
            bx::memCopy(&m_cache[first], _mtx, sizeof(Matrix4) * _num);
            return first;
        }

        return 0;
    }

    float *toPtr(uint32_t _cacheIdx)
    {
        BX_ASSERT(_cacheIdx < BGFX_CONFIG_MAX_MATRIX_CACHE, "Matrix cache out of bounds index %d (max: %d)", _cacheIdx,
                  BGFX_CONFIG_MAX_MATRIX_CACHE);
        return m_cache[_cacheIdx].un.val;
    }

    uint32_t fromPtr(const void *_ptr) const
    {
        return uint32_t((const Matrix4 *)_ptr - m_cache);
    }

    Matrix4 m_cache[BGFX_CONFIG_MAX_MATRIX_CACHE];
    uint32_t m_num;
};

struct RectCache
{
    RectCache() : m_num(0)
    {
    }

    void reset()
    {
        m_num = 0;
    }

    uint32_t add(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height)
    {
        const uint32_t first = bx::atomicFetchAndAddsat<uint32_t>(&m_num, 1, BGFX_CONFIG_MAX_RECT_CACHE - 1);
        BX_ASSERT(first + 1 < BGFX_CONFIG_MAX_RECT_CACHE, "Rect cache overflow. %d (max: %d)", first,
                  BGFX_CONFIG_MAX_RECT_CACHE);

        Rect &rect = m_cache[first];

        rect.m_x = _x;
        rect.m_y = _y;
        rect.m_width = _width;
        rect.m_height = _height;

        return first;
    }

    Rect m_cache[BGFX_CONFIG_MAX_RECT_CACHE];
    uint32_t m_num;
};

constexpr uint8_t kConstantOpcodeTypeShift = 27;
constexpr uint32_t kConstantOpcodeTypeMask = UINT32_C(0xf8000000);
constexpr uint8_t kConstantOpcodeLocShift = 11;
constexpr uint32_t kConstantOpcodeLocMask = UINT32_C(0x07fff800);
constexpr uint8_t kConstantOpcodeNumShift = 1;
constexpr uint32_t kConstantOpcodeNumMask = UINT32_C(0x000007fe);
constexpr uint8_t kConstantOpcodeCopyShift = 0;
constexpr uint32_t kConstantOpcodeCopyMask = UINT32_C(0x00000001);

constexpr uint8_t kUniformFragmentBit = 0x10;
constexpr uint8_t kUniformSamplerBit = 0x20;
constexpr uint8_t kUniformReadOnlyBit = 0x40;
constexpr uint8_t kUniformCompareBit = 0x80;
constexpr uint8_t kUniformMask =
    0 | kUniformFragmentBit | kUniformSamplerBit | kUniformReadOnlyBit | kUniformCompareBit;

class UniformBuffer
{
  public:
    static UniformBuffer *create(uint32_t _size = 1 << 20)
    {
        const uint32_t structSize = sizeof(UniformBuffer) - sizeof(UniformBuffer::m_buffer);

        uint32_t size = bx::alignUp(_size, 16);
        void *data = BX_ALLOC(g_allocator, size + structSize);
        return BX_PLACEMENT_NEW(data, UniformBuffer)(size);
    }

    static void destroy(UniformBuffer *_uniformBuffer)
    {
        _uniformBuffer->~UniformBuffer();
        BX_FREE(g_allocator, _uniformBuffer);
    }

    static void update(UniformBuffer **_uniformBuffer, uint32_t _treshold = 64 << 10, uint32_t _grow = 1 << 20)
    {
        UniformBuffer *uniformBuffer = *_uniformBuffer;
        if (_treshold >= uniformBuffer->m_size - uniformBuffer->m_pos)
        {
            const uint32_t structSize = sizeof(UniformBuffer) - sizeof(UniformBuffer::m_buffer);
            uint32_t size = bx::alignUp(uniformBuffer->m_size + _grow, 16);
            void *data = BX_REALLOC(g_allocator, uniformBuffer, size + structSize);
            uniformBuffer = reinterpret_cast<UniformBuffer *>(data);
            uniformBuffer->m_size = size;

            *_uniformBuffer = uniformBuffer;
        }
    }

    static uint32_t encodeOpcode(UniformType::Enum _type, uint16_t _loc, uint16_t _num, uint16_t _copy)
    {
        const uint32_t type = _type << kConstantOpcodeTypeShift;
        const uint32_t loc = _loc << kConstantOpcodeLocShift;
        const uint32_t num = _num << kConstantOpcodeNumShift;
        const uint32_t copy = _copy << kConstantOpcodeCopyShift;
        return type | loc | num | copy;
    }

    static void decodeOpcode(uint32_t _opcode, UniformType::Enum &_type, uint16_t &_loc, uint16_t &_num,
                             uint16_t &_copy)
    {
        const uint32_t type = (_opcode & kConstantOpcodeTypeMask) >> kConstantOpcodeTypeShift;
        const uint32_t loc = (_opcode & kConstantOpcodeLocMask) >> kConstantOpcodeLocShift;
        const uint32_t num = (_opcode & kConstantOpcodeNumMask) >> kConstantOpcodeNumShift;
        const uint32_t copy = (_opcode & kConstantOpcodeCopyMask); // >> kConstantOpcodeCopyShift;

        _type = (UniformType::Enum)(type);
        _copy = (uint16_t)copy;
        _num = (uint16_t)num;
        _loc = (uint16_t)loc;
    }

    void write(const void *_data, uint32_t _size)
    {
        BX_ASSERT(m_pos + _size < m_size, "Write would go out of bounds. pos %d + size %d > max size: %d).", m_pos,
                  _size, m_size);

        if (m_pos + _size < m_size)
        {
            bx::memCopy(&m_buffer[m_pos], _data, _size);
            m_pos += _size;
        }
    }

    void write(uint32_t _value)
    {
        write(&_value, sizeof(uint32_t));
    }

    const char *read(uint32_t _size)
    {
        BX_ASSERT(m_pos < m_size, "Out of bounds %d (size: %d).", m_pos, m_size);
        const char *result = &m_buffer[m_pos];
        m_pos += _size;
        return result;
    }

    uint32_t read()
    {
        uint32_t result;
        bx::memCopy(&result, read(sizeof(uint32_t)), sizeof(uint32_t));
        return result;
    }

    bool isEmpty() const
    {
        return 0 == m_pos;
    }

    uint32_t getPos() const
    {
        return m_pos;
    }

    void reset(uint32_t _pos = 0)
    {
        m_pos = _pos;
    }

    void finish()
    {
        write(UniformType::End);
        m_pos = 0;
    }

    void writeUniform(UniformType::Enum _type, uint16_t _loc, const void *_value, uint16_t _num = 1);
    void writeUniformHandle(UniformType::Enum _type, uint16_t _loc, UniformHandle _handle, uint16_t _num = 1);
    void writeMarker(const char *_marker);

  private:
    UniformBuffer(uint32_t _size) : m_size(_size), m_pos(0)
    {
        finish();
    }

    ~UniformBuffer()
    {
    }

    uint32_t m_size;
    uint32_t m_pos;
    char m_buffer[256 << 20];
};

struct UniformRegInfo
{
    UniformHandle m_handle;
};

class UniformRegistry
{
  public:
    UniformRegistry()
    {
    }

    ~UniformRegistry()
    {
    }

    const UniformRegInfo *find(const char *_name) const
    {
        uint16_t handle = m_uniforms.find(bx::hash<bx::HashMurmur2A>(_name));
        if (kInvalidHandle != handle)
        {
            return &m_info[handle];
        }

        return NULL;
    }

    const UniformRegInfo &add(UniformHandle _handle, const char *_name)
    {
        BX_ASSERT(isValid(_handle), "Uniform handle is invalid (name: %s)!", _name);
        const uint32_t key = bx::hash<bx::HashMurmur2A>(_name);
        m_uniforms.removeByKey(key);
        m_uniforms.insert(key, _handle.idx);

        UniformRegInfo &info = m_info[_handle.idx];
        info.m_handle = _handle;

        return info;
    }

    void remove(UniformHandle _handle)
    {
        m_uniforms.removeByHandle(_handle.idx);
    }

  private:
    typedef bx::HandleHashMapT<BGFX_CONFIG_MAX_UNIFORMS * 2> UniformHashMap;
    UniformHashMap m_uniforms;
    UniformRegInfo m_info[BGFX_CONFIG_MAX_UNIFORMS];
};

struct Binding
{
    enum Enum
    {
        Image,
        IndexBuffer,
        VertexBuffer,
        Texture,

        Count
    };

    uint32_t m_samplerFlags;
    uint16_t m_idx;
    uint8_t m_type;
    uint8_t m_format;
    uint8_t m_access;
    uint8_t m_mip;
};

struct Stream
{
    void clear()
    {
        m_startVertex = 0;
        m_handle.idx = kInvalidHandle;
        m_layoutHandle.idx = kInvalidHandle;
    }

    uint32_t m_startVertex;
    VertexBufferHandle m_handle;
    VertexLayoutHandle m_layoutHandle;
};

BX_ALIGN_DECL_CACHE_LINE(struct) RenderBind
{
    void clear(uint8_t _flags = BGFX_DISCARD_ALL)
    {
        if (0 != (_flags & BGFX_DISCARD_BINDINGS))
        {
            for (uint32_t ii = 0; ii < BGFX_CONFIG_MAX_TEXTURE_SAMPLERS; ++ii)
            {
                Binding &bind = m_bind[ii];
                bind.m_idx = kInvalidHandle;
                bind.m_type = 0;
                bind.m_samplerFlags = 0;
            }
        }
    };

    Binding m_bind[BGFX_CONFIG_MAX_TEXTURE_SAMPLERS];
};

BX_ALIGN_DECL_CACHE_LINE(struct) RenderDraw
{
    void clear(uint8_t _flags = BGFX_DISCARD_ALL)
    {
        if (0 != (_flags & BGFX_DISCARD_STATE))
        {
            m_uniformBegin = 0;
            m_uniformEnd = 0;
            m_uniformIdx = UINT8_MAX;

            m_stateFlags = BGFX_STATE_DEFAULT;
            m_stencil = packStencil(BGFX_STENCIL_DEFAULT, BGFX_STENCIL_DEFAULT);
            m_rgba = 0;
            m_scissor = UINT16_MAX;
        }

        if (0 != (_flags & BGFX_DISCARD_TRANSFORM))
        {
            m_startMatrix = 0;
            m_numMatrices = 1;
        }

        if (0 != (_flags & BGFX_DISCARD_INSTANCE_DATA))
        {
            m_instanceDataOffset = 0;
            m_instanceDataStride = 0;
            m_numInstances = 1;
            m_instanceDataBuffer.idx = kInvalidHandle;
        }

        if (0 != (_flags & BGFX_DISCARD_VERTEX_STREAMS))
        {
            m_numVertices = UINT32_MAX;
            m_streamMask = 0;
            m_stream[0].clear();
        }

        if (0 != (_flags & BGFX_DISCARD_INDEX_BUFFER))
        {
            m_startIndex = 0;
            m_numIndices = UINT32_MAX;
            m_indexBuffer.idx = kInvalidHandle;
        }

        m_submitFlags = 0;
        m_startIndirect = 0;
        m_numIndirect = UINT16_MAX;
        m_indirectBuffer.idx = kInvalidHandle;
        m_occlusionQuery.idx = kInvalidHandle;
    }

    bool setStreamBit(uint8_t _stream, VertexBufferHandle _handle)
    {
        const uint8_t bit = 1 << _stream;
        const uint8_t mask = m_streamMask & ~bit;
        const uint8_t tmp = isValid(_handle) ? bit : 0;
        m_streamMask = mask | tmp;
        return 0 != tmp;
    }

    Stream m_stream[BGFX_CONFIG_MAX_VERTEX_STREAMS];
    uint64_t m_stateFlags;
    uint64_t m_stencil;
    uint32_t m_rgba;
    uint32_t m_uniformBegin;
    uint32_t m_uniformEnd;
    uint32_t m_startMatrix;
    uint32_t m_startIndex;
    uint32_t m_numIndices;
    uint32_t m_numVertices;
    uint32_t m_instanceDataOffset;
    uint32_t m_numInstances;
    uint16_t m_instanceDataStride;
    uint16_t m_startIndirect;
    uint16_t m_numIndirect;
    uint16_t m_numMatrices;
    uint16_t m_scissor;
    uint8_t m_submitFlags;
    uint8_t m_streamMask;
    uint8_t m_uniformIdx;

    IndexBufferHandle m_indexBuffer;
    VertexBufferHandle m_instanceDataBuffer;
    IndirectBufferHandle m_indirectBuffer;
    OcclusionQueryHandle m_occlusionQuery;
};

BX_ALIGN_DECL_CACHE_LINE(struct) RenderCompute
{
    void clear(uint8_t _flags)
    {
        if (0 != (_flags & BGFX_DISCARD_STATE))
        {
            m_uniformBegin = 0;
            m_uniformEnd = 0;
            m_uniformIdx = UINT8_MAX;
        }

        if (0 != (_flags & BGFX_DISCARD_TRANSFORM))
        {
            m_startMatrix = 0;
            m_numMatrices = 0;
        }

        m_numX = 0;
        m_numY = 0;
        m_numZ = 0;
        m_submitFlags = 0;
        m_indirectBuffer.idx = kInvalidHandle;
        m_startIndirect = 0;
        m_numIndirect = UINT16_MAX;
    }

    uint32_t m_uniformBegin;
    uint32_t m_uniformEnd;
    uint32_t m_startMatrix;
    IndirectBufferHandle m_indirectBuffer;

    uint32_t m_numX;
    uint32_t m_numY;
    uint32_t m_numZ;
    uint16_t m_startIndirect;
    uint16_t m_numIndirect;
    uint16_t m_numMatrices;
    uint8_t m_submitFlags;
    uint8_t m_uniformIdx;
};

union RenderItem {
    RenderDraw draw;
    RenderCompute compute;
};

BX_ALIGN_DECL_CACHE_LINE(struct) BlitItem
{
    uint16_t m_srcX;
    uint16_t m_srcY;
    uint16_t m_srcZ;
    uint16_t m_dstX;
    uint16_t m_dstY;
    uint16_t m_dstZ;
    uint16_t m_width;
    uint16_t m_height;
    uint16_t m_depth;
    uint8_t m_srcMip;
    uint8_t m_dstMip;
    TextureHandle m_src;
    TextureHandle m_dst;
};

struct IndexBuffer
{
    String m_name;
    uint32_t m_size;
};

struct VertexBuffer
{
    String m_name;
    uint32_t m_size;
    uint16_t m_stride;
};

struct DynamicIndexBuffer
{
    IndexBufferHandle m_handle;
    uint32_t m_offset;
    uint32_t m_size;
    uint32_t m_startIndex;
    uint16_t m_flags;
};

struct DynamicVertexBuffer
{
    VertexBufferHandle m_handle;
    uint32_t m_offset;
    uint32_t m_size;
    uint32_t m_startVertex;
    uint32_t m_numVertices;
    uint16_t m_stride;
    VertexLayoutHandle m_layoutHandle;
    uint16_t m_flags;
};

struct ShaderRef
{
    UniformHandle *m_uniforms;
    String m_name;
    uint32_t m_hashIn;
    uint32_t m_hashOut;
    uint16_t m_num;
    int16_t m_refCount;
};

struct ProgramRef
{
    ShaderHandle m_vsh;
    ShaderHandle m_fsh;
    int16_t m_refCount;
};

struct UniformRef
{
    String m_name;
    UniformType::Enum m_type;
    uint16_t m_num;
    int16_t m_refCount;
};

struct TextureRef
{
    void init(BackbufferRatio::Enum _ratio, TextureFormat::Enum _format, uint32_t _storageSize, uint8_t _numMips,
              uint16_t _numLayers, bool _ptrPending, bool _immutable, bool _rt)
    {
        m_ptr = _ptrPending ? (void *)UINTPTR_MAX : NULL;
        m_storageSize = _storageSize;
        m_refCount = 1;
        m_bbRatio = uint8_t(_ratio);
        m_format = uint8_t(_format);
        m_numMips = _numMips;
        m_numLayers = _numLayers;
        m_owned = false;
        m_immutable = _immutable;
        m_rt = _rt;
    }

    String m_name;
    void *m_ptr;
    uint32_t m_storageSize;
    int16_t m_refCount;
    uint8_t m_bbRatio;
    uint8_t m_format;
    uint8_t m_numMips;
    uint16_t m_numLayers;
    bool m_owned;
    bool m_immutable;
    bool m_rt;
};

struct FrameBufferRef
{
    String m_name;

    union un {
        TextureHandle m_th[BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS];
        void *m_nwh;
    } un;

    bool m_window;
};

BX_ALIGN_DECL_CACHE_LINE(struct) View
{
    void reset()
    {
        setRect(0, 0, 1, 1);
        setScissor(0, 0, 0, 0);
        setClear(BGFX_CLEAR_NONE, 0, 0.0f, 0);
        setMode(ViewMode::Default);
        setFrameBuffer(BGFX_INVALID_HANDLE);
        setTransform(NULL, NULL);
    }

    void setRect(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height)
    {
        m_rect.m_x = uint16_t(bx::max<int16_t>(int16_t(_x), 0));
        m_rect.m_y = uint16_t(bx::max<int16_t>(int16_t(_y), 0));
        m_rect.m_width = bx::max<uint16_t>(_width, 1);
        m_rect.m_height = bx::max<uint16_t>(_height, 1);
    }

    void setScissor(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height)
    {
        m_scissor.m_x = _x;
        m_scissor.m_y = _y;
        m_scissor.m_width = _width;
        m_scissor.m_height = _height;
    }

    void setClear(uint16_t _flags, uint32_t _rgba, float _depth, uint8_t _stencil)
    {
        m_clear.set(_flags, _rgba, _depth, _stencil);
    }

    void setClear(uint16_t _flags, float _depth, uint8_t _stencil, uint8_t _0, uint8_t _1, uint8_t _2, uint8_t _3,
                  uint8_t _4, uint8_t _5, uint8_t _6, uint8_t _7)
    {
        m_clear.set(_flags, _depth, _stencil, _0, _1, _2, _3, _4, _5, _6, _7);
    }

    void setMode(ViewMode::Enum _mode)
    {
        m_mode = uint8_t(_mode);
    }

    void setFrameBuffer(FrameBufferHandle _handle)
    {
        m_fbh = _handle;
    }

    void setTransform(const void *_view, const void *_proj)
    {
        if (NULL != _view)
        {
            bx::memCopy(m_view.un.val, _view, sizeof(Matrix4));
        }
        else
        {
            m_view.setIdentity();
        }

        if (NULL != _proj)
        {
            bx::memCopy(m_proj.un.val, _proj, sizeof(Matrix4));
        }
        else
        {
            m_proj.setIdentity();
        }
    }

    Clear m_clear;
    Rect m_rect;
    Rect m_scissor;
    Matrix4 m_view;
    Matrix4 m_proj;
    FrameBufferHandle m_fbh;
    uint8_t m_mode;
};

struct FrameCache
{
    void reset()
    {
        m_matrixCache.reset();
        m_rectCache.reset();
    }

    bool isZeroArea(const Rect &_rect, uint16_t _scissor) const
    {
        if (UINT16_MAX != _scissor)
        {
            Rect scissorRect;
            scissorRect.setIntersect(_rect, m_rectCache.m_cache[_scissor]);
            return scissorRect.isZeroArea();
        }

        return false;
    }

    MatrixCache m_matrixCache;
    RectCache m_rectCache;
};

BX_ALIGN_DECL_CACHE_LINE(struct) Frame
{
    Frame() : m_waitSubmit(0), m_waitRender(0), m_capture(false)
    {
        SortKey term;
        term.reset();
        term.m_program = BGFX_INVALID_HANDLE;
        m_sortKeys[BGFX_CONFIG_MAX_DRAW_CALLS] = term.encodeDraw(SortKey::SortProgram);
        m_sortValues[BGFX_CONFIG_MAX_DRAW_CALLS] = BGFX_CONFIG_MAX_DRAW_CALLS;
        bx::memSet(m_occlusion, 0xff, sizeof(m_occlusion));

        m_perfStats.viewStats = m_viewStats;
    }

    ~Frame()
    {
    }

    void create(uint32_t _minResourceCbSize)
    {
        m_cmdPre.init(_minResourceCbSize);
        m_cmdPost.init(_minResourceCbSize);

        {
            const uint32_t num = g_caps.limits.maxEncoders;

            m_uniformBuffer = (UniformBuffer **)BX_ALLOC(g_allocator, sizeof(UniformBuffer *) * num);

            for (uint32_t ii = 0; ii < num; ++ii)
            {
                m_uniformBuffer[ii] = UniformBuffer::create();
            }
        }

        reset();
        start();
        m_textVideoMem = BX_NEW(g_allocator, TextVideoMem);
    }

    void destroy()
    {
        for (uint32_t ii = 0, num = g_caps.limits.maxEncoders; ii < num; ++ii)
        {
            UniformBuffer::destroy(m_uniformBuffer[ii]);
        }

        BX_FREE(g_allocator, m_uniformBuffer);
        BX_DELETE(g_allocator, m_textVideoMem);
    }

    void reset()
    {
        start();
        finish();
        resetFreeHandles();
    }

    void start()
    {
        m_perfStats.transientVbUsed = m_vboffset;
        m_perfStats.transientIbUsed = m_iboffset;

        m_frameCache.reset();
        m_numRenderItems = 0;
        m_numBlitItems = 0;
        m_iboffset = 0;
        m_vboffset = 0;
        m_cmdPre.start();
        m_cmdPost.start();
        m_capture = false;
    }

    void finish()
    {
        m_cmdPre.finish();
        m_cmdPost.finish();

        //			if (0 < m_numDropped)
        //			{
        //				BX_TRACE("Too many draw calls: %d, dropped %d (max: %d)"
        //					, m_numRenderItems+m_numDropped
        //					, m_numDropped
        //					, BGFX_CONFIG_MAX_DRAW_CALLS
        //					);
        //			}
    }

    void sort();

    uint32_t getAvailTransientIndexBuffer(uint32_t _num)
    {
        uint32_t offset = bx::strideAlign(m_iboffset, sizeof(uint16_t));
        uint32_t iboffset = offset + _num * sizeof(uint16_t);
        iboffset = bx::min<uint32_t>(iboffset, g_caps.limits.transientIbSize);
        uint32_t num = (iboffset - offset) / sizeof(uint16_t);
        return num;
    }

    uint32_t allocTransientIndexBuffer(uint32_t & _num)
    {
        uint32_t offset = bx::strideAlign(m_iboffset, sizeof(uint16_t));
        uint32_t num = getAvailTransientIndexBuffer(_num);
        m_iboffset = offset + num * sizeof(uint16_t);
        _num = num;

        return offset;
    }

    uint32_t getAvailTransientVertexBuffer(uint32_t _num, uint16_t _stride)
    {
        uint32_t offset = bx::strideAlign(m_vboffset, _stride);
        uint32_t vboffset = offset + _num * _stride;
        vboffset = bx::min<uint32_t>(vboffset, g_caps.limits.transientVbSize);
        uint32_t num = (vboffset - offset) / _stride;
        return num;
    }

    uint32_t allocTransientVertexBuffer(uint32_t & _num, uint16_t _stride)
    {
        uint32_t offset = bx::strideAlign(m_vboffset, _stride);
        uint32_t num = getAvailTransientVertexBuffer(_num, _stride);
        m_vboffset = offset + num * _stride;
        _num = num;

        return offset;
    }

    bool free(IndexBufferHandle _handle)
    {
        return m_freeIndexBuffer.queue(_handle);
    }

    bool free(VertexLayoutHandle _handle)
    {
        return m_freeVertexLayout.queue(_handle);
    }

    bool free(VertexBufferHandle _handle)
    {
        return m_freeVertexBuffer.queue(_handle);
    }

    bool free(ShaderHandle _handle)
    {
        return m_freeShader.queue(_handle);
    }

    bool free(ProgramHandle _handle)
    {
        return m_freeProgram.queue(_handle);
    }

    bool free(TextureHandle _handle)
    {
        return m_freeTexture.queue(_handle);
    }

    bool free(FrameBufferHandle _handle)
    {
        return m_freeFrameBuffer.queue(_handle);
    }

    bool free(UniformHandle _handle)
    {
        return m_freeUniform.queue(_handle);
    }

    void resetFreeHandles()
    {
        m_freeIndexBuffer.reset();
        m_freeVertexLayout.reset();
        m_freeVertexBuffer.reset();
        m_freeShader.reset();
        m_freeProgram.reset();
        m_freeTexture.reset();
        m_freeFrameBuffer.reset();
        m_freeUniform.reset();
    }

    ViewId m_viewRemap[BGFX_CONFIG_MAX_VIEWS];
    float m_colorPalette[BGFX_CONFIG_MAX_COLOR_PALETTE][4];

    View m_view[BGFX_CONFIG_MAX_VIEWS];

    int32_t m_occlusion[BGFX_CONFIG_MAX_OCCLUSION_QUERIES];

    uint64_t m_sortKeys[BGFX_CONFIG_MAX_DRAW_CALLS + 1];
    RenderItemCount m_sortValues[BGFX_CONFIG_MAX_DRAW_CALLS + 1];
    RenderItem m_renderItem[BGFX_CONFIG_MAX_DRAW_CALLS + 1];
    RenderBind m_renderItemBind[BGFX_CONFIG_MAX_DRAW_CALLS + 1];

    uint32_t m_blitKeys[BGFX_CONFIG_MAX_BLIT_ITEMS + 1];
    BlitItem m_blitItem[BGFX_CONFIG_MAX_BLIT_ITEMS + 1];

    FrameCache m_frameCache;
    UniformBuffer **m_uniformBuffer;

    uint32_t m_numRenderItems;
    uint16_t m_numBlitItems;

    uint32_t m_iboffset;
    uint32_t m_vboffset;
    TransientIndexBuffer *m_transientIb;
    TransientVertexBuffer *m_transientVb;

    Resolution m_resolution;
    uint32_t m_debug;

    CommandBuffer m_cmdPre;
    CommandBuffer m_cmdPost;

    template <typename Ty, uint32_t Max> struct FreeHandle
    {
        FreeHandle() : m_num(0)
        {
        }

        bool isQueued(Ty _handle)
        {
            for (uint32_t ii = 0, num = m_num; ii < num; ++ii)
            {
                if (m_queue[ii].idx == _handle.idx)
                {
                    return true;
                }
            }

            return false;
        }

        bool queue(Ty _handle)
        {
            if (BX_ENABLED(BGFX_CONFIG_DEBUG))
            {
                if (isQueued(_handle))
                {
                    return false;
                }
            }

            m_queue[m_num] = _handle;
            ++m_num;

            return true;
        }

        void reset()
        {
            m_num = 0;
        }

        Ty get(uint16_t _idx) const
        {
            return m_queue[_idx];
        }

        uint16_t getNumQueued() const
        {
            return m_num;
        }

        Ty m_queue[Max];
        uint16_t m_num;
    };

    FreeHandle<IndexBufferHandle, BGFX_CONFIG_MAX_INDEX_BUFFERS> m_freeIndexBuffer;
    FreeHandle<VertexLayoutHandle, BGFX_CONFIG_MAX_VERTEX_LAYOUTS> m_freeVertexLayout;
    FreeHandle<VertexBufferHandle, BGFX_CONFIG_MAX_VERTEX_BUFFERS> m_freeVertexBuffer;
    FreeHandle<ShaderHandle, BGFX_CONFIG_MAX_SHADERS> m_freeShader;
    FreeHandle<ProgramHandle, BGFX_CONFIG_MAX_PROGRAMS> m_freeProgram;
    FreeHandle<TextureHandle, BGFX_CONFIG_MAX_TEXTURES> m_freeTexture;
    FreeHandle<FrameBufferHandle, BGFX_CONFIG_MAX_FRAME_BUFFERS> m_freeFrameBuffer;
    FreeHandle<UniformHandle, BGFX_CONFIG_MAX_UNIFORMS> m_freeUniform;

    TextVideoMem *m_textVideoMem;

    Stats m_perfStats;
    ViewStats m_viewStats[BGFX_CONFIG_MAX_VIEWS];

    int64_t m_waitSubmit;
    int64_t m_waitRender;

    bool m_capture;
};

BX_ALIGN_DECL_CACHE_LINE(struct) EncoderImpl
{
    EncoderImpl()
    {
        discard(BGFX_DISCARD_ALL);
    }

    void begin(Frame * _frame, uint8_t _idx)
    {
        m_frame = _frame;

        m_cpuTimeBegin = bx::getHPCounter();

        m_uniformIdx = _idx;
        m_uniformBegin = 0;
        m_uniformEnd = 0;

        UniformBuffer *uniformBuffer = m_frame->m_uniformBuffer[m_uniformIdx];
        uniformBuffer->reset();

        m_numSubmitted = 0;
        m_numDropped = 0;
    }

    void end(bool _finalize)
    {
        if (_finalize)
        {
            UniformBuffer *uniformBuffer = m_frame->m_uniformBuffer[m_uniformIdx];
            uniformBuffer->finish();

            m_cpuTimeEnd = bx::getHPCounter();
        }

        if (BX_ENABLED(BGFX_CONFIG_DEBUG_OCCLUSION))
        {
            m_occlusionQuerySet.clear();
        }

        if (BX_ENABLED(BGFX_CONFIG_DEBUG_UNIFORM))
        {
            m_uniformSet.clear();
        }
    }

    void setMarker(const char *_name)
    {
        UniformBuffer *uniformBuffer = m_frame->m_uniformBuffer[m_uniformIdx];
        uniformBuffer->writeMarker(_name);
    }

    void setUniform(UniformType::Enum _type, UniformHandle _handle, const void *_value, uint16_t _num)
    {
        if (BX_ENABLED(BGFX_CONFIG_DEBUG_UNIFORM))
        {
            BX_ASSERT(m_uniformSet.end() == m_uniformSet.find(_handle.idx),
                      "Uniform %d (%s) was already set for this draw call.", _handle.idx, getName(_handle));
            m_uniformSet.insert(_handle.idx);
        }

        UniformBuffer::update(&m_frame->m_uniformBuffer[m_uniformIdx]);
        UniformBuffer *uniformBuffer = m_frame->m_uniformBuffer[m_uniformIdx];
        uniformBuffer->writeUniform(_type, _handle.idx, _value, _num);
    }

    void setState(uint64_t _state, uint32_t _rgba)
    {
        uint8_t blend = ((_state & BGFX_STATE_BLEND_MASK) >> BGFX_STATE_BLEND_SHIFT) & 0xff;
        uint8_t alphaRef = ((_state & BGFX_STATE_ALPHA_REF_MASK) >> BGFX_STATE_ALPHA_REF_SHIFT) & 0xff;
        // transparency sort order table
        m_key.m_blend =
            "\x0\x2\x2\x3\x3\x2\x3\x2\x3\x2\x2\x2\x2\x2\x2\x2\x2\x2\x2"[((blend)&0xf) + (!!blend)] + !!alphaRef;
        m_draw.m_stateFlags = _state;
        m_draw.m_rgba = _rgba;
    }

    void setCondition(OcclusionQueryHandle _handle, bool _visible)
    {
        m_draw.m_occlusionQuery = _handle;
        m_draw.m_submitFlags |= _visible ? BGFX_SUBMIT_INTERNAL_OCCLUSION_VISIBLE : 0;
    }

    void setStencil(uint32_t _fstencil, uint32_t _bstencil)
    {
        m_draw.m_stencil = packStencil(_fstencil, _bstencil);
    }

    uint16_t setScissor(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height)
    {
        uint16_t scissor = (uint16_t)m_frame->m_frameCache.m_rectCache.add(_x, _y, _width, _height);
        m_draw.m_scissor = scissor;
        return scissor;
    }

    void setScissor(uint16_t _cache)
    {
        m_draw.m_scissor = _cache;
    }

    uint32_t setTransform(const void *_mtx, uint16_t _num)
    {
        m_draw.m_startMatrix = m_frame->m_frameCache.m_matrixCache.add(_mtx, _num);
        m_draw.m_numMatrices = _num;

        return m_draw.m_startMatrix;
    }

    uint32_t allocTransform(Transform * _transform, uint16_t _num)
    {
        uint32_t first = m_frame->m_frameCache.m_matrixCache.reserve(&_num);
        _transform->data = m_frame->m_frameCache.m_matrixCache.toPtr(first);
        _transform->num = _num;

        return first;
    }

    void setTransform(uint32_t _cache, uint16_t _num)
    {
        BX_ASSERT(_cache < BGFX_CONFIG_MAX_MATRIX_CACHE, "Matrix cache out of bounds index %d (max: %d)", _cache,
                  BGFX_CONFIG_MAX_MATRIX_CACHE);
        m_draw.m_startMatrix = _cache;
        m_draw.m_numMatrices = uint16_t(bx::min<uint32_t>(_cache + _num, BGFX_CONFIG_MAX_MATRIX_CACHE - 1) - _cache);
    }

    void setIndexBuffer(IndexBufferHandle _handle, uint32_t _firstIndex, uint32_t _numIndices)
    {
        BX_ASSERT(UINT8_MAX != m_draw.m_streamMask, "");
        m_draw.m_startIndex = _firstIndex;
        m_draw.m_numIndices = _numIndices;
        m_draw.m_indexBuffer = _handle;
    }

    void setIndexBuffer(const DynamicIndexBuffer &_dib, uint32_t _firstIndex, uint32_t _numIndices)
    {
        BX_ASSERT(UINT8_MAX != m_draw.m_streamMask, "");
        const uint32_t indexSize = 0 == (_dib.m_flags & BGFX_BUFFER_INDEX32) ? 2 : 4;
        m_draw.m_startIndex = _dib.m_startIndex + _firstIndex;
        m_draw.m_numIndices = bx::min(_numIndices, _dib.m_size / indexSize);
        m_draw.m_indexBuffer = _dib.m_handle;
    }

    void setIndexBuffer(const TransientIndexBuffer *_tib, uint32_t _firstIndex, uint32_t _numIndices)
    {
        BX_ASSERT(UINT8_MAX != m_draw.m_streamMask, "");
        const uint32_t numIndices = bx::min(_numIndices, _tib->size / 2);
        m_draw.m_indexBuffer = _tib->handle;
        m_draw.m_startIndex = _tib->startIndex + _firstIndex;
        m_draw.m_numIndices = numIndices;
        m_discard = 0 == numIndices;
    }

    void setVertexBuffer(uint8_t _stream, VertexBufferHandle _handle, uint32_t _startVertex, uint32_t _numVertices,
                         VertexLayoutHandle _layoutHandle)
    {
        BX_ASSERT(UINT8_MAX != m_draw.m_streamMask, "");
        BX_ASSERT(_stream < BGFX_CONFIG_MAX_VERTEX_STREAMS, "Invalid stream %d (max %d).", _stream,
                  BGFX_CONFIG_MAX_VERTEX_STREAMS);
        if (m_draw.setStreamBit(_stream, _handle))
        {
            Stream &stream = m_draw.m_stream[_stream];
            stream.m_startVertex = _startVertex;
            stream.m_handle = _handle;
            stream.m_layoutHandle = _layoutHandle;
            m_numVertices[_stream] = _numVertices;
        }
    }

    void setVertexBuffer(uint8_t _stream, const DynamicVertexBuffer &_dvb, uint32_t _startVertex, uint32_t _numVertices,
                         VertexLayoutHandle _layoutHandle)
    {
        BX_ASSERT(UINT8_MAX != m_draw.m_streamMask, "");
        BX_ASSERT(_stream < BGFX_CONFIG_MAX_VERTEX_STREAMS, "Invalid stream %d (max %d).", _stream,
                  BGFX_CONFIG_MAX_VERTEX_STREAMS);
        if (m_draw.setStreamBit(_stream, _dvb.m_handle))
        {
            Stream &stream = m_draw.m_stream[_stream];
            stream.m_startVertex = _dvb.m_startVertex + _startVertex;
            stream.m_handle = _dvb.m_handle;
            stream.m_layoutHandle = isValid(_layoutHandle) ? _layoutHandle : _dvb.m_layoutHandle;
            m_numVertices[_stream] = bx::min(bx::uint32_imax(0, _dvb.m_numVertices - _startVertex), _numVertices);
        }
    }

    void setVertexBuffer(uint8_t _stream, const TransientVertexBuffer *_tvb, uint32_t _startVertex,
                         uint32_t _numVertices, VertexLayoutHandle _layoutHandle)
    {
        BX_ASSERT(UINT8_MAX != m_draw.m_streamMask, "");
        BX_ASSERT(_stream < BGFX_CONFIG_MAX_VERTEX_STREAMS, "Invalid stream %d (max %d).", _stream,
                  BGFX_CONFIG_MAX_VERTEX_STREAMS);
        if (m_draw.setStreamBit(_stream, _tvb->handle))
        {
            Stream &stream = m_draw.m_stream[_stream];
            stream.m_startVertex = _tvb->startVertex + _startVertex;
            stream.m_handle = _tvb->handle;
            stream.m_layoutHandle = isValid(_layoutHandle) ? _layoutHandle : _tvb->layoutHandle;
            m_numVertices[_stream] =
                bx::min(bx::uint32_imax(0, _tvb->size / _tvb->stride - _startVertex), _numVertices);
        }
    }

    void setVertexCount(uint32_t _numVertices)
    {
        BX_ASSERT(0 == m_draw.m_streamMask, "Vertex buffer already set.");
        m_draw.m_streamMask = UINT8_MAX;
        Stream &stream = m_draw.m_stream[0];
        stream.m_startVertex = 0;
        stream.m_handle.idx = kInvalidHandle;
        stream.m_layoutHandle.idx = kInvalidHandle;
        m_numVertices[0] = _numVertices;
    }

    void setInstanceDataBuffer(const InstanceDataBuffer *_idb, uint32_t _start, uint32_t _num)
    {
        const uint32_t start = bx::min(_start, _idb->num);
        const uint32_t num = bx::min(_idb->num - start, _num);
        m_draw.m_instanceDataOffset = _idb->offset + start * _idb->stride;
        m_draw.m_instanceDataStride = _idb->stride;
        m_draw.m_numInstances = num;
        m_draw.m_instanceDataBuffer = _idb->handle;
    }

    void setInstanceDataBuffer(VertexBufferHandle _handle, uint32_t _startVertex, uint32_t _num, uint16_t _stride)
    {
        m_draw.m_instanceDataOffset = _startVertex * _stride;
        m_draw.m_instanceDataStride = _stride;
        m_draw.m_numInstances = _num;
        m_draw.m_instanceDataBuffer = _handle;
    }

    void setInstanceCount(uint32_t _numInstances)
    {
        BX_ASSERT(!isValid(m_draw.m_instanceDataBuffer), "Instance buffer already set.");
        m_draw.m_numInstances = _numInstances;
    }

    void setTexture(uint8_t _stage, UniformHandle _sampler, TextureHandle _handle, uint32_t _flags)
    {
        Binding &bind = m_bind.m_bind[_stage];
        bind.m_idx = _handle.idx;
        bind.m_type = uint8_t(Binding::Texture);
        bind.m_samplerFlags = (_flags & BGFX_SAMPLER_INTERNAL_DEFAULT) ? BGFX_SAMPLER_INTERNAL_DEFAULT : _flags;

        if (isValid(_sampler))
        {
            uint32_t stage = _stage;
            setUniform(UniformType::Sampler, _sampler, &stage, 1);
        }
    }

    void setBuffer(uint8_t _stage, IndexBufferHandle _handle, Access::Enum _access)
    {
        Binding &bind = m_bind.m_bind[_stage];
        bind.m_idx = _handle.idx;
        bind.m_type = uint8_t(Binding::IndexBuffer);
        bind.m_format = 0;
        bind.m_access = uint8_t(_access);
        bind.m_mip = 0;
    }

    void setBuffer(uint8_t _stage, VertexBufferHandle _handle, Access::Enum _access)
    {
        Binding &bind = m_bind.m_bind[_stage];
        bind.m_idx = _handle.idx;
        bind.m_type = uint8_t(Binding::VertexBuffer);
        bind.m_format = 0;
        bind.m_access = uint8_t(_access);
        bind.m_mip = 0;
    }

    void setImage(uint8_t _stage, TextureHandle _handle, uint8_t _mip, Access::Enum _access,
                  TextureFormat::Enum _format)
    {
        Binding &bind = m_bind.m_bind[_stage];
        bind.m_idx = _handle.idx;
        bind.m_type = uint8_t(Binding::Image);
        bind.m_format = uint8_t(_format);
        bind.m_access = uint8_t(_access);
        bind.m_mip = _mip;
    }

    void discard(uint8_t _flags)
    {
        if (BX_ENABLED(BGFX_CONFIG_DEBUG_UNIFORM))
        {
            m_uniformSet.clear();
        }

        m_discard = false;
        m_draw.clear(_flags);
        m_compute.clear(_flags);
        m_bind.clear(_flags);
    }

    void submit(ViewId _id, ProgramHandle _program, OcclusionQueryHandle _occlusionQuery, uint32_t _depth,
                uint8_t _flags);

    void submit(ViewId _id, ProgramHandle _program, IndirectBufferHandle _indirectHandle, uint16_t _start,
                uint16_t _num, uint32_t _depth, uint8_t _flags)
    {
        m_draw.m_startIndirect = _start;
        m_draw.m_numIndirect = _num;
        m_draw.m_indirectBuffer = _indirectHandle;
        OcclusionQueryHandle handle = BGFX_INVALID_HANDLE;
        submit(_id, _program, handle, _depth, _flags);
    }

    void dispatch(ViewId _id, ProgramHandle _handle, uint32_t _ngx, uint32_t _ngy, uint32_t _ngz, uint8_t _flags);

    void dispatch(ViewId _id, ProgramHandle _handle, IndirectBufferHandle _indirectHandle, uint16_t _start,
                  uint16_t _num, uint8_t _flags)
    {
        m_compute.m_indirectBuffer = _indirectHandle;
        m_compute.m_startIndirect = _start;
        m_compute.m_numIndirect = _num;
        dispatch(_id, _handle, 0, 0, 0, _flags);
    }

    void blit(ViewId _id, TextureHandle _dst, uint8_t _dstMip, uint16_t _dstX, uint16_t _dstY, uint16_t _dstZ,
              TextureHandle _src, uint8_t _srcMip, uint16_t _srcX, uint16_t _srcY, uint16_t _srcZ, uint16_t _width,
              uint16_t _height, uint16_t _depth);

    Frame *m_frame;

    SortKey m_key;

    RenderDraw m_draw;
    RenderCompute m_compute;
    RenderBind m_bind;

    uint32_t m_numSubmitted;
    uint32_t m_numDropped;

    uint32_t m_uniformBegin;
    uint32_t m_uniformEnd;
    uint32_t m_numVertices[BGFX_CONFIG_MAX_VERTEX_STREAMS];
    uint8_t m_uniformIdx;
    bool m_discard;

    typedef stl::unordered_set<uint16_t> HandleSet;
    HandleSet m_uniformSet;
    HandleSet m_occlusionQuerySet;

    int64_t m_cpuTimeBegin;
    int64_t m_cpuTimeEnd;
};

struct VertexLayoutRef
{
    VertexLayoutRef()
    {
    }

    void init()
    {
        bx::memSet(m_vertexLayoutRef, 0, sizeof(m_vertexLayoutRef));
        bx::memSet(m_vertexBufferRef, 0xff, sizeof(m_vertexBufferRef));
        bx::memSet(m_dynamicVertexBufferRef, 0xff, sizeof(m_dynamicVertexBufferRef));
    }

    template <uint16_t MaxHandlesT> void shutdown(bx::HandleAllocT<MaxHandlesT> &_handleAlloc)
    {
        for (uint16_t ii = 0, num = _handleAlloc.getNumHandles(); ii < num; ++ii)
        {
            VertexLayoutHandle handle = {_handleAlloc.getHandleAt(ii)};
            m_vertexLayoutRef[handle.idx] = 0;
            m_vertexLayoutMap.removeByHandle(handle.idx);
            _handleAlloc.free(handle.idx);
        }

        m_vertexLayoutMap.reset();
    }

    VertexLayoutHandle find(uint32_t _hash)
    {
        VertexLayoutHandle handle = {m_vertexLayoutMap.find(_hash)};
        return handle;
    }

    void add(VertexLayoutHandle _layoutHandle, uint32_t _hash)
    {
        m_vertexLayoutRef[_layoutHandle.idx]++;
        m_vertexLayoutMap.insert(_hash, _layoutHandle.idx);
    }

    void add(VertexBufferHandle _handle, VertexLayoutHandle _layoutHandle, uint32_t _hash)
    {
        BX_ASSERT(m_vertexBufferRef[_handle.idx].idx == kInvalidHandle, "");
        m_vertexBufferRef[_handle.idx] = _layoutHandle;
        m_vertexLayoutRef[_layoutHandle.idx]++;
        m_vertexLayoutMap.insert(_hash, _layoutHandle.idx);
    }

    void add(DynamicVertexBufferHandle _handle, VertexLayoutHandle _layoutHandle, uint32_t _hash)
    {
        BX_ASSERT(m_dynamicVertexBufferRef[_handle.idx].idx == kInvalidHandle, "");
        m_dynamicVertexBufferRef[_handle.idx] = _layoutHandle;
        m_vertexLayoutRef[_layoutHandle.idx]++;
        m_vertexLayoutMap.insert(_hash, _layoutHandle.idx);
    }

    VertexLayoutHandle release(VertexLayoutHandle _layoutHandle)
    {
        if (isValid(_layoutHandle))
        {
            m_vertexLayoutRef[_layoutHandle.idx]--;

            if (0 == m_vertexLayoutRef[_layoutHandle.idx])
            {
                m_vertexLayoutMap.removeByHandle(_layoutHandle.idx);
                return _layoutHandle;
            }
        }

        return BGFX_INVALID_HANDLE;
    }

    VertexLayoutHandle release(VertexBufferHandle _handle)
    {
        VertexLayoutHandle layoutHandle = m_vertexBufferRef[_handle.idx];
        layoutHandle = release(layoutHandle);
        m_vertexBufferRef[_handle.idx].idx = kInvalidHandle;

        return layoutHandle;
    }

    VertexLayoutHandle release(DynamicVertexBufferHandle _handle)
    {
        VertexLayoutHandle layoutHandle = m_dynamicVertexBufferRef[_handle.idx];
        layoutHandle = release(layoutHandle);
        m_dynamicVertexBufferRef[_handle.idx].idx = kInvalidHandle;

        return layoutHandle;
    }

    typedef bx::HandleHashMapT<BGFX_CONFIG_MAX_VERTEX_LAYOUTS * 2> VertexLayoutMap;
    VertexLayoutMap m_vertexLayoutMap;

    uint16_t m_vertexLayoutRef[BGFX_CONFIG_MAX_VERTEX_LAYOUTS];
    VertexLayoutHandle m_vertexBufferRef[BGFX_CONFIG_MAX_VERTEX_BUFFERS];
    VertexLayoutHandle m_dynamicVertexBufferRef[BGFX_CONFIG_MAX_DYNAMIC_VERTEX_BUFFERS];
};

// First-fit non-local allocator.
class NonLocalAllocator
{
  public:
    static const uint64_t kInvalidBlock = UINT64_MAX;

    NonLocalAllocator()
    {
    }

    ~NonLocalAllocator()
    {
    }

    void reset()
    {
        m_free.clear();
        m_used.clear();
    }

    void add(uint64_t _ptr, uint32_t _size)
    {
        m_free.push_back(Free(_ptr, _size));
    }

    uint64_t remove()
    {
        BX_ASSERT(0 == m_used.size(), "");

        if (0 < m_free.size())
        {
            Free freeBlock = m_free.front();
            m_free.pop_front();
            return freeBlock.m_ptr;
        }

        return 0;
    }

    uint64_t alloc(uint32_t _size)
    {
        _size = bx::max(_size, 16u);

        for (FreeList::iterator it = m_free.begin(), itEnd = m_free.end(); it != itEnd; ++it)
        {
            if (it->m_size >= _size)
            {
                uint64_t ptr = it->m_ptr;

                m_used.insert(stl::make_pair(ptr, _size));

                if (it->m_size != _size)
                {
                    it->m_size -= _size;
                    it->m_ptr += _size;
                }
                else
                {
                    m_free.erase(it);
                }

                return ptr;
            }
        }

        // there is no block large enough.
        return kInvalidBlock;
    }

    void free(uint64_t _block)
    {
        UsedList::iterator it = m_used.find(_block);
        if (it != m_used.end())
        {
            m_free.push_front(Free(it->first, it->second));
            m_used.erase(it);
        }
    }

    bool compact()
    {
        m_free.sort();

        for (FreeList::iterator it = m_free.begin(), next = it, itEnd = m_free.end(); next != itEnd;)
        {
            if ((it->m_ptr + it->m_size) == next->m_ptr)
            {
                it->m_size += next->m_size;
                next = m_free.erase(next);
            }
            else
            {
                it = next;
                ++next;
            }
        }

        return 0 == m_used.size();
    }

  private:
    struct Free
    {
        Free(uint64_t _ptr, uint32_t _size) : m_ptr(_ptr), m_size(_size)
        {
        }

        bool operator<(const Free &rhs) const
        {
            return m_ptr < rhs.m_ptr;
        }

        uint64_t m_ptr;
        uint32_t m_size;
    };

    typedef stl::list<Free> FreeList;
    FreeList m_free;

    typedef stl::unordered_map<uint64_t, uint32_t> UsedList;
    UsedList m_used;
};


}