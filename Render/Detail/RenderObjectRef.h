

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
