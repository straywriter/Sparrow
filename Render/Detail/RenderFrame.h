

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
