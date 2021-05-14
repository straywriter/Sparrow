

bool Context::init(const Init &_init)
{
    BX_ASSERT(!m_rendererInitialized, "Already initialized?");

    m_init = _init;
    m_init.resolution.reset &= ~BGFX_RESET_INTERNAL_FORCE;
    m_init.resolution.numBackBuffers =
        bx::clamp<uint8_t>(_init.resolution.numBackBuffers, 2, BGFX_CONFIG_MAX_BACK_BUFFERS);
    m_init.resolution.maxFrameLatency =
        bx::min<uint8_t>(_init.resolution.maxFrameLatency, BGFX_CONFIG_MAX_FRAME_LATENCY);
    dump(m_init.resolution);

    if (g_platformData.ndt == NULL && g_platformData.nwh == NULL && g_platformData.context == NULL &&
        g_platformData.backBuffer == NULL && g_platformData.backBufferDS == NULL)
    {
        bx::memCopy(&g_platformData, &m_init.platformData, sizeof(PlatformData));
    }
    else
    {
        bx::memCopy(&m_init.platformData, &g_platformData, sizeof(PlatformData));
    }

    if (true && !BX_ENABLED(BX_PLATFORM_EMSCRIPTEN || BX_PLATFORM_PS4) && RendererType::Noop != m_init.type &&
        NULL == m_init.platformData.ndt && NULL == m_init.platformData.nwh && NULL == m_init.platformData.context &&
        NULL == m_init.platformData.backBuffer && NULL == m_init.platformData.backBufferDS)
    {
        BX_TRACE("bgfx platform data like window handle or backbuffer is not set, creating headless device.");
    }

    m_exit = false;
    m_flipped = true;
    m_frames = 0;
    m_debug = BGFX_DEBUG_NONE;
    m_frameTimeLast = bx::getHPCounter();

    m_submit->create(_init.limits.minResourceCbSize);

#if BGFX_CONFIG_MULTITHREADED
    m_render->create(_init.limits.minResourceCbSize);

    if (s_renderFrameCalled)
    {
        // When bgfx::renderFrame is called before init render thread
        // should not be created.
        BX_TRACE("Application called bgfx::renderFrame directly, not creating render thread.");
        m_singleThreaded = true && ~BGFX_API_THREAD_MAGIC == s_threadIndex;
    }
    else
    {
        BX_TRACE("Creating rendering thread.");
        m_thread.init(renderThread, this, 0, "bgfx - renderer backend thread");
        m_singleThreaded = false;
    }
#else
    BX_TRACE("Multithreaded renderer is disabled.");
    m_singleThreaded = true;
#endif // BGFX_CONFIG_MULTITHREADED

    BX_TRACE("Running in %s-threaded mode", m_singleThreaded ? "single" : "multi");

    s_threadIndex = BGFX_API_THREAD_MAGIC;

    for (uint32_t ii = 0; ii < BX_COUNTOF(m_viewRemap); ++ii)
    {
        m_viewRemap[ii] = ViewId(ii);
    }

    for (uint32_t ii = 0; ii < BGFX_CONFIG_MAX_VIEWS; ++ii)
    {
        resetView(ViewId(ii));
    }

    for (uint32_t ii = 0; ii < BX_COUNTOF(m_clearColor); ++ii)
    {
        m_clearColor[ii][0] = 0.0f;
        m_clearColor[ii][1] = 0.0f;
        m_clearColor[ii][2] = 0.0f;
        m_clearColor[ii][3] = 1.0f;
    }

    m_vertexLayoutRef.init();

    CommandBuffer &cmdbuf = getCommandBuffer(CommandBuffer::RendererInit);
    cmdbuf.write(_init);

    frameNoRenderWait();

    m_encoderHandle = bx::createHandleAlloc(g_allocator, _init.limits.maxEncoders);
    m_encoder = (EncoderImpl *)BX_ALIGNED_ALLOC(g_allocator, sizeof(EncoderImpl) * _init.limits.maxEncoders,
                                                BX_ALIGNOF(EncoderImpl));
    m_encoderStats = (EncoderStats *)BX_ALLOC(g_allocator, sizeof(EncoderStats) * _init.limits.maxEncoders);
    for (uint32_t ii = 0, num = _init.limits.maxEncoders; ii < num; ++ii)
    {
        BX_PLACEMENT_NEW(&m_encoder[ii], EncoderImpl);
    }

    uint16_t idx = m_encoderHandle->alloc();
    BX_ASSERT(0 == idx, "Internal encoder handle is not 0 (idx %d).", idx);
    BX_UNUSED(idx);
    m_encoder[0].begin(m_submit, 0);
    m_encoder0 = reinterpret_cast<Encoder *>(&m_encoder[0]);

    // Make sure renderer init is called from render thread.
    // g_caps is initialized and available after this point.
    frame();

    if (!m_rendererInitialized)
    {
        getCommandBuffer(CommandBuffer::RendererShutdownEnd);
        frame();
        frame();
        m_vertexLayoutRef.shutdown(m_layoutHandle);
        m_submit->destroy();
#if BGFX_CONFIG_MULTITHREADED
        m_render->destroy();
#endif // BGFX_CONFIG_MULTITHREADED
        return false;
    }

    for (uint32_t ii = 0; ii < BX_COUNTOF(s_emulatedFormats); ++ii)
    {
        const uint32_t fmt = s_emulatedFormats[ii];
        g_caps.formats[fmt] |=
            0 == (g_caps.formats[fmt] & BGFX_CAPS_FORMAT_TEXTURE_2D) ? BGFX_CAPS_FORMAT_TEXTURE_2D_EMULATED : 0;
        g_caps.formats[fmt] |=
            0 == (g_caps.formats[fmt] & BGFX_CAPS_FORMAT_TEXTURE_3D) ? BGFX_CAPS_FORMAT_TEXTURE_3D_EMULATED : 0;
        g_caps.formats[fmt] |=
            0 == (g_caps.formats[fmt] & BGFX_CAPS_FORMAT_TEXTURE_CUBE) ? BGFX_CAPS_FORMAT_TEXTURE_CUBE_EMULATED : 0;
    }

    for (uint32_t ii = 0; ii < TextureFormat::UnknownDepth; ++ii)
    {
        bool convertable = bimg::imageConvert(bimg::TextureFormat::BGRA8, bimg::TextureFormat::Enum(ii));
        g_caps.formats[ii] |= 0 == (g_caps.formats[ii] & BGFX_CAPS_FORMAT_TEXTURE_2D) && convertable
                                  ? BGFX_CAPS_FORMAT_TEXTURE_2D_EMULATED
                                  : 0;
        g_caps.formats[ii] |= 0 == (g_caps.formats[ii] & BGFX_CAPS_FORMAT_TEXTURE_3D) && convertable
                                  ? BGFX_CAPS_FORMAT_TEXTURE_3D_EMULATED
                                  : 0;
        g_caps.formats[ii] |= 0 == (g_caps.formats[ii] & BGFX_CAPS_FORMAT_TEXTURE_CUBE) && convertable
                                  ? BGFX_CAPS_FORMAT_TEXTURE_CUBE_EMULATED
                                  : 0;
    }

    g_caps.rendererType = m_renderCtx->getRendererType();
    initAttribTypeSizeTable(g_caps.rendererType);

    g_caps.supported &= _init.capabilities;
    g_caps.supported |=
        0 | (BX_ENABLED(BGFX_CONFIG_MULTITHREADED) && !m_singleThreaded ? BGFX_CAPS_RENDERER_MULTITHREADED : 0) |
        (isGraphicsDebuggerPresent() ? BGFX_CAPS_GRAPHICS_DEBUGGER : 0);

    dumpCaps();

    m_textVideoMemBlitter.init();
    m_clearQuad.init();

    m_submit->m_transientVb = createTransientVertexBuffer(_init.limits.transientVbSize);
    m_submit->m_transientIb = createTransientIndexBuffer(_init.limits.transientIbSize);
    frame();

    if (BX_ENABLED(BGFX_CONFIG_MULTITHREADED))
    {
        m_submit->m_transientVb = createTransientVertexBuffer(_init.limits.transientVbSize);
        m_submit->m_transientIb = createTransientIndexBuffer(_init.limits.transientIbSize);
        frame();
    }

    g_internalData.caps = getCaps();

    return true;
}

void Context::shutdown()
{
    getCommandBuffer(CommandBuffer::RendererShutdownBegin);
    frame();

    destroyTransientVertexBuffer(m_submit->m_transientVb);
    destroyTransientIndexBuffer(m_submit->m_transientIb);
    m_textVideoMemBlitter.shutdown();
    m_clearQuad.shutdown();
    frame();

    if (BX_ENABLED(BGFX_CONFIG_MULTITHREADED))
    {
        destroyTransientVertexBuffer(m_submit->m_transientVb);
        destroyTransientIndexBuffer(m_submit->m_transientIb);
        frame();
    }

    frame(); // If any VertexLayouts needs to be destroyed.

    getCommandBuffer(CommandBuffer::RendererShutdownEnd);
    frame();

    m_encoder[0].end(true);
    m_encoderHandle->free(0);
    bx::destroyHandleAlloc(g_allocator, m_encoderHandle);
    m_encoderHandle = NULL;

    for (uint32_t ii = 0, num = g_caps.limits.maxEncoders; ii < num; ++ii)
    {
        m_encoder[ii].~EncoderImpl();
    }

    BX_ALIGNED_FREE(g_allocator, m_encoder, BX_ALIGNOF(EncoderImpl));
    BX_FREE(g_allocator, m_encoderStats);

    m_dynVertexBufferAllocator.compact();
    m_dynIndexBufferAllocator.compact();

    BX_ASSERT(m_layoutHandle.getNumHandles() == m_vertexLayoutRef.m_vertexLayoutMap.getNumElements(),
              "VertexLayoutRef mismatch, num handles %d, handles in hash map %d.", m_layoutHandle.getNumHandles(),
              m_vertexLayoutRef.m_vertexLayoutMap.getNumElements());

    m_vertexLayoutRef.shutdown(m_layoutHandle);

#if BGFX_CONFIG_MULTITHREADED
    // Render thread shutdown sequence.
    renderSemWait(); // Wait for previous frame.
    apiSemPost();    // OK to set context to NULL.
    // s_ctx is NULL here.
    renderSemWait(); // In RenderFrame::Exiting state.

    if (m_thread.isRunning())
    {
        m_thread.shutdown();
    }

    m_render->destroy();
#endif // BGFX_CONFIG_MULTITHREADED

    bx::memSet(&g_internalData, 0, sizeof(InternalData));
    s_ctx = NULL;

    m_submit->destroy();

    if (BX_ENABLED(BGFX_CONFIG_DEBUG))
    {
#define CHECK_HANDLE_LEAK(_name, _handleAlloc)                                                                         \
    BX_MACRO_BLOCK_BEGIN                                                                                               \
    if (0 != _handleAlloc.getNumHandles())                                                                             \
    {                                                                                                                  \
        BX_TRACE("LEAK: %s %d (max: %d)", _name, _handleAlloc.getNumHandles(), _handleAlloc.getMaxHandles());          \
        for (uint16_t ii = 0, num = _handleAlloc.getNumHandles(); ii < num; ++ii)                                      \
        {                                                                                                              \
            BX_TRACE("\t%3d: %4d", ii, _handleAlloc.getHandleAt(ii));                                                  \
        }                                                                                                              \
    }                                                                                                                  \
    BX_MACRO_BLOCK_END

#define CHECK_HANDLE_LEAK_NAME(_name, _handleAlloc, _type, _ref)                                                       \
    BX_MACRO_BLOCK_BEGIN                                                                                               \
    if (0 != _handleAlloc.getNumHandles())                                                                             \
    {                                                                                                                  \
        BX_TRACE("LEAK: %s %d (max: %d)", _name, _handleAlloc.getNumHandles(), _handleAlloc.getMaxHandles());          \
        for (uint16_t ii = 0, num = _handleAlloc.getNumHandles(); ii < num; ++ii)                                      \
        {                                                                                                              \
            uint16_t idx = _handleAlloc.getHandleAt(ii);                                                               \
            const _type &ref = _ref[idx];                                                                              \
            BX_UNUSED(ref);                                                                                            \
            BX_TRACE("\t%3d: %4d %s", ii, idx, ref.m_name.getPtr());                                                   \
        }                                                                                                              \
    }                                                                                                                  \
    BX_MACRO_BLOCK_END

#define CHECK_HANDLE_LEAK_RC_NAME(_name, _handleAlloc, _type, _ref)                                                    \
    BX_MACRO_BLOCK_BEGIN                                                                                               \
    if (0 != _handleAlloc.getNumHandles())                                                                             \
    {                                                                                                                  \
        BX_TRACE("LEAK: %s %d (max: %d)", _name, _handleAlloc.getNumHandles(), _handleAlloc.getMaxHandles());          \
        for (uint16_t ii = 0, num = _handleAlloc.getNumHandles(); ii < num; ++ii)                                      \
        {                                                                                                              \
            uint16_t idx = _handleAlloc.getHandleAt(ii);                                                               \
            const _type &ref = _ref[idx];                                                                              \
            BX_UNUSED(ref);                                                                                            \
            BX_TRACE("\t%3d: %4d %s (count %d)", ii, idx, ref.m_name.getPtr(), ref.m_refCount);                        \
        }                                                                                                              \
    }                                                                                                                  \
    BX_MACRO_BLOCK_END

        CHECK_HANDLE_LEAK("DynamicIndexBufferHandle", m_dynamicIndexBufferHandle);
        CHECK_HANDLE_LEAK("DynamicVertexBufferHandle", m_dynamicVertexBufferHandle);
        CHECK_HANDLE_LEAK_NAME("IndexBufferHandle", m_indexBufferHandle, IndexBuffer, m_indexBuffers);
        CHECK_HANDLE_LEAK("VertexLayoutHandle", m_layoutHandle);
        CHECK_HANDLE_LEAK_NAME("VertexBufferHandle", m_vertexBufferHandle, VertexBuffer, m_vertexBuffers);
        CHECK_HANDLE_LEAK_RC_NAME("ShaderHandle", m_shaderHandle, ShaderRef, m_shaderRef);
        CHECK_HANDLE_LEAK("ProgramHandle", m_programHandle);
        CHECK_HANDLE_LEAK_RC_NAME("TextureHandle", m_textureHandle, TextureRef, m_textureRef);
        CHECK_HANDLE_LEAK_NAME("FrameBufferHandle", m_frameBufferHandle, FrameBufferRef, m_frameBufferRef);
        CHECK_HANDLE_LEAK_RC_NAME("UniformHandle", m_uniformHandle, UniformRef, m_uniformRef);
        CHECK_HANDLE_LEAK("OcclusionQueryHandle", m_occlusionQueryHandle);
#undef CHECK_HANDLE_LEAK
#undef CHECK_HANDLE_LEAK_NAME
    }
}

void Context::freeDynamicBuffers()
{
    for (uint16_t ii = 0, num = m_numFreeDynamicIndexBufferHandles; ii < num; ++ii)
    {
        destroyDynamicIndexBufferInternal(m_freeDynamicIndexBufferHandle[ii]);
    }
    m_numFreeDynamicIndexBufferHandles = 0;

    for (uint16_t ii = 0, num = m_numFreeDynamicVertexBufferHandles; ii < num; ++ii)
    {
        destroyDynamicVertexBufferInternal(m_freeDynamicVertexBufferHandle[ii]);
    }
    m_numFreeDynamicVertexBufferHandles = 0;

    for (uint16_t ii = 0, num = m_numFreeOcclusionQueryHandles; ii < num; ++ii)
    {
        m_occlusionQueryHandle.free(m_freeOcclusionQueryHandle[ii].idx);
    }
    m_numFreeOcclusionQueryHandles = 0;
}

void Context::freeAllHandles(Frame *_frame)
{
    for (uint16_t ii = 0, num = _frame->m_freeIndexBuffer.getNumQueued(); ii < num; ++ii)
    {
        m_indexBufferHandle.free(_frame->m_freeIndexBuffer.get(ii).idx);
    }

    for (uint16_t ii = 0, num = _frame->m_freeVertexBuffer.getNumQueued(); ii < num; ++ii)
    {
        destroyVertexBufferInternal(_frame->m_freeVertexBuffer.get(ii));
    }

    for (uint16_t ii = 0, num = _frame->m_freeVertexLayout.getNumQueued(); ii < num; ++ii)
    {
        m_layoutHandle.free(_frame->m_freeVertexLayout.get(ii).idx);
    }

    for (uint16_t ii = 0, num = _frame->m_freeShader.getNumQueued(); ii < num; ++ii)
    {
        m_shaderHandle.free(_frame->m_freeShader.get(ii).idx);
    }

    for (uint16_t ii = 0, num = _frame->m_freeProgram.getNumQueued(); ii < num; ++ii)
    {
        m_programHandle.free(_frame->m_freeProgram.get(ii).idx);
    }

    for (uint16_t ii = 0, num = _frame->m_freeTexture.getNumQueued(); ii < num; ++ii)
    {
        m_textureHandle.free(_frame->m_freeTexture.get(ii).idx);
    }

    for (uint16_t ii = 0, num = _frame->m_freeFrameBuffer.getNumQueued(); ii < num; ++ii)
    {
        m_frameBufferHandle.free(_frame->m_freeFrameBuffer.get(ii).idx);
    }

    for (uint16_t ii = 0, num = _frame->m_freeUniform.getNumQueued(); ii < num; ++ii)
    {
        m_uniformHandle.free(_frame->m_freeUniform.get(ii).idx);
    }
}

Encoder *Context::begin(bool _forThread)
{
    EncoderImpl *encoder = &m_encoder[0];

#if BGFX_CONFIG_MULTITHREADED
    if (_forThread || BGFX_API_THREAD_MAGIC != s_threadIndex)
    {
        bx::MutexScope scopeLock(m_encoderApiLock);

        uint16_t idx = m_encoderHandle->alloc();
        if (kInvalidHandle == idx)
        {
            return NULL;
        }

        encoder = &m_encoder[idx];
        encoder->begin(m_submit, uint8_t(idx));
    }
#else
    BX_UNUSED(_forThread);
#endif // BGFX_CONFIG_MULTITHREADED

    return reinterpret_cast<Encoder *>(encoder);
}

void Context::end(Encoder *_encoder)
{
#if BGFX_CONFIG_MULTITHREADED
    EncoderImpl *encoder = reinterpret_cast<EncoderImpl *>(_encoder);
    if (encoder != &m_encoder[0])
    {
        encoder->end(true);
        m_encoderEndSem.post();
    }
#else
    BX_UNUSED(_encoder);
#endif // BGFX_CONFIG_MULTITHREADED
}

uint32_t Context::frame(bool _capture)
{
    m_encoder[0].end(true);

#if BGFX_CONFIG_MULTITHREADED
    bx::MutexScope resourceApiScope(m_resourceApiLock);

    encoderApiWait();
    bx::MutexScope encoderApiScope(m_encoderApiLock);
#else
    encoderApiWait();
#endif // BGFX_CONFIG_MULTITHREADED

    m_submit->m_capture = _capture;

    BGFX_PROFILER_SCOPE("bgfx/API thread frame", 0xff2040ff);
    // wait for render thread to finish
    renderSemWait();
    frameNoRenderWait();

    m_encoder[0].begin(m_submit, 0);

    return m_frames;
}

void Context::frameNoRenderWait()
{
    swap();

    // release render thread
    apiSemPost();
}

void Context::swap()
{
    freeDynamicBuffers();
    m_submit->m_resolution = m_init.resolution;
    m_init.resolution.reset &= ~BGFX_RESET_INTERNAL_FORCE;
    m_submit->m_debug = m_debug;
    m_submit->m_perfStats.numViews = 0;

    bx::memCopy(m_submit->m_viewRemap, m_viewRemap, sizeof(m_viewRemap));
    bx::memCopy(m_submit->m_view, m_view, sizeof(m_view));

    if (m_colorPaletteDirty > 0)
    {
        --m_colorPaletteDirty;
        bx::memCopy(m_submit->m_colorPalette, m_clearColor, sizeof(m_clearColor));
    }

    freeAllHandles(m_submit);
    m_submit->resetFreeHandles();

    m_submit->finish();

    bx::swap(m_render, m_submit);

    bx::memCopy(m_render->m_occlusion, m_submit->m_occlusion, sizeof(m_submit->m_occlusion));

    if (!BX_ENABLED(BGFX_CONFIG_MULTITHREADED) || m_singleThreaded)
    {
        renderFrame();
    }

    m_frames++;
    m_submit->start();

    bx::memSet(m_seq, 0, sizeof(m_seq));

    m_submit->m_textVideoMem->resize(m_render->m_textVideoMem->m_small, m_init.resolution.width,
                                     m_init.resolution.height);

    int64_t now = bx::getHPCounter();
    m_submit->m_perfStats.cpuTimeFrame = now - m_frameTimeLast;
    m_frameTimeLast = now;
}

void Context::flip()
{
    if (m_rendererInitialized && !m_flipped)
    {
        m_renderCtx->flip();
        m_flipped = true;

        if (m_renderCtx->isDeviceRemoved())
        {
            // Something horribly went wrong, fallback to noop renderer.
            rendererDestroy(m_renderCtx);

            Init init;
            init.type = RendererType::Noop;
            m_renderCtx = rendererCreate(init);
            g_caps.rendererType = RendererType::Noop;
        }
    }
}

RenderFrame::Enum Context::renderFrame(int32_t _msecs)
{
    BGFX_PROFILER_SCOPE("bgfx::renderFrame", 0xff2040ff);

#if BX_PLATFORM_OSX || BX_PLATFORM_IOS
    NSAutoreleasePoolScope pool;
#endif // BX_PLATFORM_OSX

    if (!m_flipAfterRender)
    {
        BGFX_PROFILER_SCOPE("bgfx/flip", 0xff2040ff);
        flip();
    }

    if (apiSemWait(_msecs))
    {
        {
            BGFX_PROFILER_SCOPE("bgfx/Exec commands pre", 0xff2040ff);
            rendererExecCommands(m_render->m_cmdPre);
        }

        if (m_rendererInitialized)
        {
            {
                BGFX_PROFILER_SCOPE("bgfx/Render submit", 0xff2040ff);
                m_renderCtx->submit(m_render, m_clearQuad, m_textVideoMemBlitter);
                m_flipped = false;
            }

            {
                BGFX_PROFILER_SCOPE("bgfx/Screenshot", 0xff2040ff);
                for (uint8_t ii = 0, num = m_render->m_numScreenShots; ii < num; ++ii)
                {
                    const ScreenShot &screenShot = m_render->m_screenShot[ii];
                    m_renderCtx->requestScreenShot(screenShot.handle, screenShot.filePath.getCPtr());
                }
            }
        }

        {
            BGFX_PROFILER_SCOPE("bgfx/Exec commands post", 0xff2040ff);
            rendererExecCommands(m_render->m_cmdPost);
        }

        renderSemPost();

        if (m_flipAfterRender)
        {
            BGFX_PROFILER_SCOPE("bgfx/flip", 0xff2040ff);
            flip();
        }
    }
    else
    {
        return RenderFrame::Timeout;
    }

    return m_exit ? RenderFrame::Exiting : RenderFrame::Render;
}

void rendererUpdateUniforms(RendererContextI *_renderCtx, UniformBuffer *_uniformBuffer, uint32_t _begin, uint32_t _end)
{
    _uniformBuffer->reset(_begin);
    while (_uniformBuffer->getPos() < _end)
    {
        uint32_t opcode = _uniformBuffer->read();

        if (UniformType::End == opcode)
        {
            break;
        }

        UniformType::Enum type;
        uint16_t loc;
        uint16_t num;
        uint16_t copy;
        UniformBuffer::decodeOpcode(opcode, type, loc, num, copy);

        uint32_t size = g_uniformTypeSize[type] * num;
        const char *data = _uniformBuffer->read(size);
        if (UniformType::Count > type)
        {
            if (copy)
            {
                _renderCtx->updateUniform(loc, data, size);
            }
            else
            {
                _renderCtx->updateUniform(loc, *(const char **)(data), size);
            }
        }
        else
        {
            _renderCtx->setMarker(data, uint16_t(size) - 1);
        }
    }
}

void Context::flushTextureUpdateBatch(CommandBuffer &_cmdbuf)
{
    if (m_textureUpdateBatch.sort())
    {
        const uint32_t pos = _cmdbuf.m_pos;

        uint32_t currentKey = UINT32_MAX;

        for (uint32_t ii = 0, num = m_textureUpdateBatch.m_num; ii < num; ++ii)
        {
            _cmdbuf.m_pos = m_textureUpdateBatch.m_values[ii];

            TextureHandle handle;
            _cmdbuf.read(handle);

            uint8_t side;
            _cmdbuf.read(side);

            uint8_t mip;
            _cmdbuf.read(mip);

            Rect rect;
            _cmdbuf.read(rect);

            uint16_t zz;
            _cmdbuf.read(zz);

            uint16_t depth;
            _cmdbuf.read(depth);

            uint16_t pitch;
            _cmdbuf.read(pitch);

            const Memory *mem;
            _cmdbuf.read(mem);

            uint32_t key = m_textureUpdateBatch.m_keys[ii];
            if (key != currentKey)
            {
                if (currentKey != UINT32_MAX)
                {
                    m_renderCtx->updateTextureEnd();
                }
                currentKey = key;
                m_renderCtx->updateTextureBegin(handle, side, mip);
            }

            m_renderCtx->updateTexture(handle, side, mip, rect, zz, depth, pitch, mem);

            release(mem);
        }

        if (currentKey != UINT32_MAX)
        {
            m_renderCtx->updateTextureEnd();
        }

        m_textureUpdateBatch.reset();

        _cmdbuf.m_pos = pos;
    }
}

typedef RendererContextI *(*RendererCreateFn)(const Init &_init);
typedef void (*RendererDestroyFn)();