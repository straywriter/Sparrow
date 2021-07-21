#include "Render/RenderContext.h"
#include "Render/IRenderContext.h"

#include "bx/bx.h"

#include "Render/StaticVariable.h"
#include "RenderDefine.h"
#include "RenderEnum.h"
#include "Render/Tools/VertexLayout/VertexLayout.h"
#include "RenderContext.h"

namespace bgfx
{

extern Context *s_ctx;
// extern const CapsFlags s_capsFlags[];
extern bool s_renderFrameCalled;
extern uint32_t s_threadIndex;
//  extern const TextureFormat::Enum s_emulatedFormats[];

static void dumpCaps()
{
    BX_TRACE("");

    if (0 < g_caps.numGPUs)
    {
        BX_TRACE("Detected GPUs (%d):", g_caps.numGPUs);
        BX_TRACE("\t +----------------   Index");
        BX_TRACE("\t |  +-------------   Device ID");
        BX_TRACE("\t |  |    +--------   Vendor ID");
        for (uint32_t ii = 0; ii < g_caps.numGPUs; ++ii)
        {
            const Caps::GPU &gpu = g_caps.gpu[ii];
            BX_UNUSED(gpu);

            BX_TRACE("\t %d: %04x %04x", ii, gpu.deviceId, gpu.vendorId);
        }

        BX_TRACE("");
    }

    RendererType::Enum renderers[RendererType::Count];
    uint8_t num = getSupportedRenderers(BX_COUNTOF(renderers), renderers);

    BX_TRACE("Supported renderer backends (%d):", num);
    for (uint32_t ii = 0; ii < num; ++ii)
    {
        BX_TRACE("\t - %s", getRendererName(renderers[ii]));
    }

    BX_TRACE("");
    BX_TRACE("Sort key masks:");
    BX_TRACE("\t   View     %016" PRIx64, kSortKeyViewMask);
    BX_TRACE("\t   Draw bit %016" PRIx64, kSortKeyDrawBit);

    BX_TRACE("");
    BX_TRACE("\tD  Type     %016" PRIx64, kSortKeyDrawTypeMask);

    BX_TRACE("");
    BX_TRACE("\tD0 Blend    %016" PRIx64, kSortKeyDraw0BlendMask);
    BX_TRACE("\tD0 Program  %016" PRIx64, kSortKeyDraw0ProgramMask);
    BX_TRACE("\tD0 Depth    %016" PRIx64, kSortKeyDraw0DepthMask);

    BX_TRACE("");
    BX_TRACE("\tD1 Depth    %016" PRIx64, kSortKeyDraw1DepthMask);
    BX_TRACE("\tD1 Blend    %016" PRIx64, kSortKeyDraw1BlendMask);
    BX_TRACE("\tD1 Program  %016" PRIx64, kSortKeyDraw1ProgramMask);

    BX_TRACE("");
    BX_TRACE("\tD2 Seq      %016" PRIx64, kSortKeyDraw2SeqMask);
    BX_TRACE("\tD2 Blend    %016" PRIx64, kSortKeyDraw2BlendMask);
    BX_TRACE("\tD2 Program  %016" PRIx64, kSortKeyDraw2ProgramMask);

    BX_TRACE("");
    BX_TRACE("\t C Seq      %016" PRIx64, kSortKeyComputeSeqMask);
    BX_TRACE("\t C Program  %016" PRIx64, kSortKeyComputeProgramMask);

    BX_TRACE("");
    BX_TRACE("Capabilities (renderer %s, vendor 0x%04x, device 0x%04x):", s_ctx->m_renderCtx->getRendererName(),
             g_caps.vendorId, g_caps.deviceId);
    for (uint32_t ii = 0; ii < BX_COUNTOF(s_capsFlags); ++ii)
    {
        BX_TRACE("\t[%c] %s", 0 != (g_caps.supported & s_capsFlags[ii].m_flag) ? 'x' : ' ', s_capsFlags[ii].m_str);
    }

    BX_TRACE("");
    BX_TRACE("Limits:");
#define LIMITS(_x) BX_TRACE("\t%-24s%10d", #_x, g_caps.limits._x)
    LIMITS(maxDrawCalls);
    LIMITS(maxBlits);
    LIMITS(maxTextureSize);
    LIMITS(maxTextureLayers);
    LIMITS(maxViews);
    LIMITS(maxFrameBuffers);
    LIMITS(maxFBAttachments);
    LIMITS(maxPrograms);
    LIMITS(maxShaders);
    LIMITS(maxTextures);
    LIMITS(maxTextureSamplers);
    LIMITS(maxComputeBindings);
    LIMITS(maxVertexLayouts);
    LIMITS(maxVertexStreams);
    LIMITS(maxIndexBuffers);
    LIMITS(maxVertexBuffers);
    LIMITS(maxDynamicIndexBuffers);
    LIMITS(maxDynamicVertexBuffers);
    LIMITS(maxUniforms);
    LIMITS(maxOcclusionQueries);
    LIMITS(maxEncoders);
    LIMITS(minResourceCbSize);
    LIMITS(transientVbSize);
    LIMITS(transientIbSize);
#undef LIMITS

    BX_TRACE("");
    BX_TRACE("Supported texture formats:");
    BX_TRACE("\t +----------------   2D: x = supported / * = emulated");
    BX_TRACE("\t |+---------------   2D: sRGB format");
    BX_TRACE("\t ||+--------------   3D: x = supported / * = emulated");
    BX_TRACE("\t |||+-------------   3D: sRGB format");
    BX_TRACE("\t ||||+------------ Cube: x = supported / * = emulated");
    BX_TRACE("\t |||||+----------- Cube: sRGB format");
    BX_TRACE("\t ||||||+---------- vertex format");
    BX_TRACE("\t |||||||+--------- image");
    BX_TRACE("\t ||||||||+-------- framebuffer");
    BX_TRACE("\t |||||||||+------- MSAA framebuffer");
    BX_TRACE("\t ||||||||||+------ MSAA texture");
    BX_TRACE("\t |||||||||||+----- Auto-generated mips");
    BX_TRACE("\t ||||||||||||  +-- name");
    for (uint32_t ii = 0; ii < TextureFormat::Count; ++ii)
    {
        if (TextureFormat::Unknown != ii && TextureFormat::UnknownDepth != ii)
        {
            uint16_t flags = g_caps.formats[ii];
            BX_TRACE("\t[%c%c%c%c%c%c%c%c%c%c%c%c] %s",
                     flags & BGFX_CAPS_FORMAT_TEXTURE_2D            ? 'x'
                     : flags & BGFX_CAPS_FORMAT_TEXTURE_2D_EMULATED ? '*'
                                                                    : ' ',
                     flags & BGFX_CAPS_FORMAT_TEXTURE_2D_SRGB ? 'l' : ' ',
                     flags & BGFX_CAPS_FORMAT_TEXTURE_3D            ? 'x'
                     : flags & BGFX_CAPS_FORMAT_TEXTURE_3D_EMULATED ? '*'
                                                                    : ' ',
                     flags & BGFX_CAPS_FORMAT_TEXTURE_3D_SRGB ? 'l' : ' ',
                     flags & BGFX_CAPS_FORMAT_TEXTURE_CUBE            ? 'x'
                     : flags & BGFX_CAPS_FORMAT_TEXTURE_CUBE_EMULATED ? '*'
                                                                      : ' ',
                     flags & BGFX_CAPS_FORMAT_TEXTURE_CUBE_SRGB ? 'l' : ' ',
                     flags & BGFX_CAPS_FORMAT_TEXTURE_VERTEX ? 'v' : ' ',
                     flags & BGFX_CAPS_FORMAT_TEXTURE_IMAGE ? 'i' : ' ',
                     flags & BGFX_CAPS_FORMAT_TEXTURE_FRAMEBUFFER ? 'f' : ' ',
                     flags & BGFX_CAPS_FORMAT_TEXTURE_FRAMEBUFFER_MSAA ? '+' : ' ',
                     flags & BGFX_CAPS_FORMAT_TEXTURE_MSAA ? 'm' : ' ',
                     flags & BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN ? 'M' : ' ', getName(TextureFormat::Enum(ii)));
            BX_UNUSED(flags);
        }
    }

    BX_TRACE("");
    BX_TRACE("NDC depth [%d, 1], origin %s left.", g_caps.homogeneousDepth ? -1 : 0,
             g_caps.originBottomLeft ? "bottom" : "top");

    BX_TRACE("");
}


bool Context::init(const Init &_init)
{
    BX_ASSERT(!m_rendererInitialized, "Already initialized?");

    m_init = _init;
    m_init.resolution.reset &= ~BGFX_RESET_INTERNAL_FORCE;

    if (g_platformData.ndt == NULL && g_platformData.nwh == NULL && g_platformData.context == NULL &&
        g_platformData.backBuffer == NULL && g_platformData.backBufferDS == NULL)
    {
        bx::memCopy(&g_platformData, &m_init.platformData, sizeof(PlatformData));
    }
    else
    {
        bx::memCopy(&m_init.platformData, &g_platformData, sizeof(PlatformData));
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
    m_encoder = (EncoderImpl *)BX_ALLOC(g_allocator, sizeof(EncoderImpl) * _init.limits.maxEncoders);
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
    BX_FREE(g_allocator, m_encoder);
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

#if BX_PLATFORM_OSX || BX_PLATFORM_IOS
struct NSAutoreleasePoolScope
{
    NSAutoreleasePoolScope()
    {
        id obj = class_createInstance(objc_getClass("NSAutoreleasePool"), 0);
        typedef id (*objc_msgSend_init)(void *, SEL);
        pool = ((objc_msgSend_init)objc_msgSend)(obj, sel_getUid("init"));
    }

    ~NSAutoreleasePoolScope()
    {
        typedef void (*objc_msgSend_release)(void *, SEL);
        ((objc_msgSend_release)objc_msgSend)(pool, sel_getUid("release"));
    }

    id pool;
};
#endif // BX_PLATFORM_OSX

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
            BGFX_PROFILER_SCOPE("bgfx/Render submit", 0xff2040ff);
            m_renderCtx->submit(m_render, m_clearQuad, m_textVideoMemBlitter);
            m_flipped = false;
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

	void Context::rendererExecCommands(CommandBuffer& _cmdbuf)
	{
		_cmdbuf.reset();

		bool end = false;

		if (NULL == m_renderCtx)
		{
			uint8_t command;
			_cmdbuf.read(command);

			switch (command)
			{
			case CommandBuffer::RendererShutdownEnd:
				m_exit = true;
				return;

			case CommandBuffer::End:
				return;

			default:
				{
					BX_ASSERT(CommandBuffer::RendererInit == command
						, "RendererInit must be the first command in command buffer before initialization. Unexpected command %d?"
						, command
						);
					BX_ASSERT(!m_rendererInitialized, "This shouldn't happen! Bad synchronization?");

					Init init;
					_cmdbuf.read(init);

					m_renderCtx = rendererCreate(init);

					m_rendererInitialized = NULL != m_renderCtx;

					if (!m_rendererInitialized)
					{
						_cmdbuf.read(command);
						BX_ASSERT(CommandBuffer::End == command, "Unexpected command %d?"
							, command
							);
						return;
					}
				}
				break;
			}
		}

		do
		{
			uint8_t command;
			_cmdbuf.read(command);

			switch (command)
			{
			case CommandBuffer::RendererShutdownBegin:
				{
					BX_ASSERT(m_rendererInitialized, "This shouldn't happen! Bad synchronization?");
					m_rendererInitialized = false;
				}
				break;

			case CommandBuffer::RendererShutdownEnd:
				{
					BX_ASSERT(!m_rendererInitialized && !m_exit, "This shouldn't happen! Bad synchronization?");

					rendererDestroy(m_renderCtx);
					m_renderCtx = NULL;

					m_exit = true;
				}
				BX_FALLTHROUGH;

			case CommandBuffer::End:
				end = true;
				break;

			case CommandBuffer::CreateIndexBuffer:
				{
					BGFX_PROFILER_SCOPE("CreateIndexBuffer", 0xff2040ff);

					IndexBufferHandle handle;
					_cmdbuf.read(handle);

					const Memory* mem;
					_cmdbuf.read(mem);

					uint16_t flags;
					_cmdbuf.read(flags);

					m_renderCtx->createIndexBuffer(handle, mem, flags);

					release(mem);
				}
				break;

			case CommandBuffer::DestroyIndexBuffer:
				{
					BGFX_PROFILER_SCOPE("DestroyIndexBuffer", 0xff2040ff);

					IndexBufferHandle handle;
					_cmdbuf.read(handle);

					m_renderCtx->destroyIndexBuffer(handle);
				}
				break;

			case CommandBuffer::CreateVertexLayout:
				{
					BGFX_PROFILER_SCOPE("CreateVertexLayout", 0xff2040ff);

					VertexLayoutHandle handle;
					_cmdbuf.read(handle);

					VertexLayout layout;
					_cmdbuf.read(layout);

					m_renderCtx->createVertexLayout(handle, layout);
				}
				break;

			case CommandBuffer::DestroyVertexLayout:
				{
					BGFX_PROFILER_SCOPE("DestroyVertexLayout", 0xff2040ff);

					VertexLayoutHandle handle;
					_cmdbuf.read(handle);

					m_renderCtx->destroyVertexLayout(handle);
				}
				break;

			case CommandBuffer::CreateVertexBuffer:
				{
					BGFX_PROFILER_SCOPE("CreateVertexBuffer", 0xff2040ff);

					VertexBufferHandle handle;
					_cmdbuf.read(handle);

					const Memory* mem;
					_cmdbuf.read(mem);

					VertexLayoutHandle layoutHandle;
					_cmdbuf.read(layoutHandle);

					uint16_t flags;
					_cmdbuf.read(flags);

					m_renderCtx->createVertexBuffer(handle, mem, layoutHandle, flags);

					release(mem);
				}
				break;

			case CommandBuffer::DestroyVertexBuffer:
				{
					BGFX_PROFILER_SCOPE("DestroyVertexBuffer", 0xff2040ff);

					VertexBufferHandle handle;
					_cmdbuf.read(handle);

					m_renderCtx->destroyVertexBuffer(handle);
				}
				break;

			case CommandBuffer::CreateDynamicIndexBuffer:
				{
					BGFX_PROFILER_SCOPE("CreateDynamicIndexBuffer", 0xff2040ff);

					IndexBufferHandle handle;
					_cmdbuf.read(handle);

					uint32_t size;
					_cmdbuf.read(size);

					uint16_t flags;
					_cmdbuf.read(flags);

					m_renderCtx->createDynamicIndexBuffer(handle, size, flags);
				}
				break;

			case CommandBuffer::UpdateDynamicIndexBuffer:
				{
					BGFX_PROFILER_SCOPE("UpdateDynamicIndexBuffer", 0xff2040ff);

					IndexBufferHandle handle;
					_cmdbuf.read(handle);

					uint32_t offset;
					_cmdbuf.read(offset);

					uint32_t size;
					_cmdbuf.read(size);

					const Memory* mem;
					_cmdbuf.read(mem);

					m_renderCtx->updateDynamicIndexBuffer(handle, offset, size, mem);

					release(mem);
				}
				break;

			case CommandBuffer::DestroyDynamicIndexBuffer:
				{
					BGFX_PROFILER_SCOPE("DestroyDynamicIndexBuffer", 0xff2040ff);

					IndexBufferHandle handle;
					_cmdbuf.read(handle);

					m_renderCtx->destroyDynamicIndexBuffer(handle);
				}
				break;

			case CommandBuffer::CreateDynamicVertexBuffer:
				{
					BGFX_PROFILER_SCOPE("CreateDynamicVertexBuffer", 0xff2040ff);

					VertexBufferHandle handle;
					_cmdbuf.read(handle);

					uint32_t size;
					_cmdbuf.read(size);

					uint16_t flags;
					_cmdbuf.read(flags);

					m_renderCtx->createDynamicVertexBuffer(handle, size, flags);
				}
				break;

			case CommandBuffer::UpdateDynamicVertexBuffer:
				{
					BGFX_PROFILER_SCOPE("UpdateDynamicVertexBuffer", 0xff2040ff);

					VertexBufferHandle handle;
					_cmdbuf.read(handle);

					uint32_t offset;
					_cmdbuf.read(offset);

					uint32_t size;
					_cmdbuf.read(size);

					const Memory* mem;
					_cmdbuf.read(mem);

					m_renderCtx->updateDynamicVertexBuffer(handle, offset, size, mem);

					release(mem);
				}
				break;

			case CommandBuffer::DestroyDynamicVertexBuffer:
				{
					BGFX_PROFILER_SCOPE("DestroyDynamicVertexBuffer", 0xff2040ff);

					VertexBufferHandle handle;
					_cmdbuf.read(handle);

					m_renderCtx->destroyDynamicVertexBuffer(handle);
				}
				break;

			case CommandBuffer::CreateShader:
				{
					BGFX_PROFILER_SCOPE("CreateShader", 0xff2040ff);

					ShaderHandle handle;
					_cmdbuf.read(handle);

					const Memory* mem;
					_cmdbuf.read(mem);

					m_renderCtx->createShader(handle, mem);

					release(mem);
				}
				break;

			case CommandBuffer::DestroyShader:
				{
					BGFX_PROFILER_SCOPE("DestroyShader", 0xff2040ff);

					ShaderHandle handle;
					_cmdbuf.read(handle);

					m_renderCtx->destroyShader(handle);
				}
				break;

			case CommandBuffer::CreateProgram:
				{
					BGFX_PROFILER_SCOPE("CreateProgram", 0xff2040ff);

					ProgramHandle handle;
					_cmdbuf.read(handle);

					ShaderHandle vsh;
					_cmdbuf.read(vsh);

					ShaderHandle fsh;
					_cmdbuf.read(fsh);

					m_renderCtx->createProgram(handle, vsh, fsh);
				}
				break;

			case CommandBuffer::DestroyProgram:
				{
					BGFX_PROFILER_SCOPE("DestroyProgram", 0xff2040ff);

					ProgramHandle handle;
					_cmdbuf.read(handle);

					m_renderCtx->destroyProgram(handle);
				}
				break;

			case CommandBuffer::CreateTexture:
				{
					BGFX_PROFILER_SCOPE("CreateTexture", 0xff2040ff);

					TextureHandle handle;
					_cmdbuf.read(handle);

					const Memory* mem;
					_cmdbuf.read(mem);

					uint64_t flags;
					_cmdbuf.read(flags);

					uint8_t skip;
					_cmdbuf.read(skip);

					void* ptr = m_renderCtx->createTexture(handle, mem, flags, skip);
					if (NULL != ptr)
					{
						setDirectAccessPtr(handle, ptr);
					}

					bx::MemoryReader reader(mem->data, mem->size);

					uint32_t magic;
					bx::read(&reader, magic);

					if (BGFX_CHUNK_MAGIC_TEX == magic)
					{
						TextureCreate tc;
						bx::read(&reader, tc);

						if (NULL != tc.m_mem)
						{
							release(tc.m_mem);
						}
					}

					release(mem);
				}
				break;

			case CommandBuffer::UpdateTexture:
				{
					BGFX_PROFILER_SCOPE("UpdateTexture", 0xff2040ff);

					if (m_textureUpdateBatch.isFull() )
					{
						flushTextureUpdateBatch(_cmdbuf);
					}

					uint32_t value = _cmdbuf.m_pos;

					TextureHandle handle;
					_cmdbuf.read(handle);

					uint8_t side;
					_cmdbuf.read(side);

					uint8_t mip;
					_cmdbuf.read(mip);

					_cmdbuf.skip<Rect>();
					_cmdbuf.skip<uint16_t>();
					_cmdbuf.skip<uint16_t>();
					_cmdbuf.skip<uint16_t>();
					_cmdbuf.skip<Memory*>();

					uint32_t key = (handle.idx<<16)
						| (side<<8)
						| mip
						;

					m_textureUpdateBatch.add(key, value);
				}
				break;

			case CommandBuffer::ReadTexture:
				{
					BGFX_PROFILER_SCOPE("ReadTexture", 0xff2040ff);

					TextureHandle handle;
					_cmdbuf.read(handle);

					void* data;
					_cmdbuf.read(data);

					uint8_t mip;
					_cmdbuf.read(mip);

					m_renderCtx->readTexture(handle, data, mip);
				}
				break;

			case CommandBuffer::ResizeTexture:
				{
					BGFX_PROFILER_SCOPE("ResizeTexture", 0xff2040ff);

					TextureHandle handle;
					_cmdbuf.read(handle);

					uint16_t width;
					_cmdbuf.read(width);

					uint16_t height;
					_cmdbuf.read(height);

					uint8_t numMips;
					_cmdbuf.read(numMips);

					uint16_t numLayers;
					_cmdbuf.read(numLayers);

					m_renderCtx->resizeTexture(handle, width, height, numMips, numLayers);
				}
				break;

			case CommandBuffer::DestroyTexture:
				{
					BGFX_PROFILER_SCOPE("DestroyTexture", 0xff2040ff);

					TextureHandle handle;
					_cmdbuf.read(handle);

					m_renderCtx->destroyTexture(handle);
				}
				break;

			case CommandBuffer::CreateFrameBuffer:
				{
					BGFX_PROFILER_SCOPE("CreateFrameBuffer", 0xff2040ff);

					FrameBufferHandle handle;
					_cmdbuf.read(handle);

					bool window;
					_cmdbuf.read(window);

					if (window)
					{
						void* nwh;
						_cmdbuf.read(nwh);

						uint16_t width;
						_cmdbuf.read(width);

						uint16_t height;
						_cmdbuf.read(height);

						TextureFormat::Enum format;
						_cmdbuf.read(format);

						TextureFormat::Enum depthFormat;
						_cmdbuf.read(depthFormat);

						m_renderCtx->createFrameBuffer(handle, nwh, width, height, format, depthFormat);
					}
					else
					{
						uint8_t num;
						_cmdbuf.read(num);

						Attachment attachment[BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS];
						_cmdbuf.read(attachment, sizeof(Attachment) * num);

						m_renderCtx->createFrameBuffer(handle, num, attachment);
					}
				}
				break;

			case CommandBuffer::DestroyFrameBuffer:
				{
					BGFX_PROFILER_SCOPE("DestroyFrameBuffer", 0xff2040ff);

					FrameBufferHandle handle;
					_cmdbuf.read(handle);

					m_renderCtx->destroyFrameBuffer(handle);
				}
				break;

			case CommandBuffer::CreateUniform:
				{
					BGFX_PROFILER_SCOPE("CreateUniform", 0xff2040ff);

					UniformHandle handle;
					_cmdbuf.read(handle);

					UniformType::Enum type;
					_cmdbuf.read(type);

					uint16_t num;
					_cmdbuf.read(num);

					uint8_t len;
					_cmdbuf.read(len);

					const char* name = (const char*)_cmdbuf.skip(len);

					m_renderCtx->createUniform(handle, type, num, name);
				}
				break;

			case CommandBuffer::DestroyUniform:
				{
					BGFX_PROFILER_SCOPE("DestroyUniform", 0xff2040ff);

					UniformHandle handle;
					_cmdbuf.read(handle);

					m_renderCtx->destroyUniform(handle);
				}
				break;

			case CommandBuffer::RequestScreenShot:
				{
					BGFX_PROFILER_SCOPE("RequestScreenShot", 0xff2040ff);

					FrameBufferHandle handle;
					_cmdbuf.read(handle);

					uint16_t len;
					_cmdbuf.read(len);

					const char* filePath = (const char*)_cmdbuf.skip(len);

					m_renderCtx->requestScreenShot(handle, filePath);
				}
				break;

			case CommandBuffer::UpdateViewName:
				{
					BGFX_PROFILER_SCOPE("UpdateViewName", 0xff2040ff);

					ViewId id;
					_cmdbuf.read(id);

					uint16_t len;
					_cmdbuf.read(len);

					const char* name = (const char*)_cmdbuf.skip(len);

					m_renderCtx->updateViewName(id, name);
				}
				break;

			case CommandBuffer::InvalidateOcclusionQuery:
				{
					BGFX_PROFILER_SCOPE("InvalidateOcclusionQuery", 0xff2040ff);

					OcclusionQueryHandle handle;
					_cmdbuf.read(handle);

					m_renderCtx->invalidateOcclusionQuery(handle);
				}
				break;

			case CommandBuffer::SetName:
				{
					BGFX_PROFILER_SCOPE("SetName", 0xff2040ff);

					Handle handle;
					_cmdbuf.read(handle);

					uint16_t len;
					_cmdbuf.read(len);

					const char* name = (const char*)_cmdbuf.skip(len);

					m_renderCtx->setName(handle, name, len-1);
				}
				break;

			default:
				BX_ASSERT(false, "Invalid command: %d", command);
				break;
			}
		} while (!end);

		flushTextureUpdateBatch(_cmdbuf);
	}


} // namespace bgfx
