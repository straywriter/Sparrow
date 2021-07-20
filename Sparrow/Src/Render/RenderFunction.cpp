#include "Render/RenderFunction.h"

namespace bgfx
{

uint32_t weldVertices(void *_output, const VertexLayout &_layout, const void *_data, uint32_t _num, bool _index32,
                      float _epsilon)
{
    return weldVertices(_output, _layout, _data, _num, _index32, _epsilon, g_allocator);
}

uint32_t topologyConvert(TopologyConvert::Enum _conversion, void *_dst, uint32_t _dstSize, const void *_indices,
                         uint32_t _numIndices, bool _index32)
{
    return topologyConvert(_conversion, _dst, _dstSize, _indices, _numIndices, _index32, g_allocator);
}

void topologySortTriList(TopologySort::Enum _sort, void *_dst, uint32_t _dstSize, const float _dir[3],
                         const float _pos[3], const void *_vertices, uint32_t _stride, const void *_indices,
                         uint32_t _numIndices, bool _index32)
{
    topologySortTriList(_sort, _dst, _dstSize, _dir, _pos, _vertices, _stride, _indices, _numIndices, _index32,
                        g_allocator);
}

uint8_t getSupportedRenderers(uint8_t _max, RendererType::Enum *_enum)
{
    _enum = _max == 0 ? NULL : _enum;

    uint8_t num = 0;
    for (uint8_t ii = 0; ii < RendererType::Count; ++ii)
    {
        if ((RendererType::Direct3D11 == ii || RendererType::Direct3D12 == ii) &&
            windowsVersionIs(Condition::LessEqual, 0x0502))
        {
            continue;
        }

        if (NULL == _enum)
        {
            num++;
        }
        else
        {
            if (num < _max && s_rendererCreator[ii].supported)
            {
                _enum[num++] = RendererType::Enum(ii);
            }
        }
    }

    return num;
}

const char *getRendererName(RendererType::Enum _type)
{
    BX_ASSERT(_type < RendererType::Count, "Invalid renderer type %d.", _type);
    return s_rendererCreator[_type].name;
}

bool init(const Init &_userInit)
{
    if (NULL != s_ctx)
    {
        BX_TRACE("bgfx is already initialized.");
        return false;
    }

    Init init = _userInit;

    init.limits.maxEncoders =
        bx::clamp<uint16_t>(init.limits.maxEncoders, 1, (0 != BGFX_CONFIG_MULTITHREADED) ? 128 : 1);
    init.limits.minResourceCbSize =
        bx::min<uint32_t>(init.limits.minResourceCbSize, BGFX_CONFIG_MIN_RESOURCE_COMMAND_BUFFER_SIZE);

    struct ErrorState
    {
        enum Enum
        {
            Default,
            ContextAllocated,
        };
    };

    ErrorState::Enum errorState = ErrorState::Default;

    if (NULL != init.allocator)
    {
        g_allocator = init.allocator;
    }
    else
    {
        bx::DefaultAllocator allocator;
        g_allocator = s_allocatorStub = BX_NEW(&allocator, AllocatorStub);
    }

    if (NULL != init.callback)
    {
        g_callback = init.callback;
    }
    else
    {
        g_callback = s_callbackStub = BX_NEW(g_allocator, CallbackStub);
    }

    if (true && !BX_ENABLED(BX_PLATFORM_EMSCRIPTEN || BX_PLATFORM_PS4) && RendererType::Noop != init.type &&
        NULL == init.platformData.ndt && NULL == init.platformData.nwh && NULL == init.platformData.context &&
        NULL == init.platformData.backBuffer && NULL == init.platformData.backBufferDS)
    {
        BX_TRACE("bgfx platform data like window handle or backbuffer is not set, creating headless device.");
    }

    bx::memSet(&g_caps, 0, sizeof(g_caps));
    g_caps.limits.maxDrawCalls = BGFX_CONFIG_MAX_DRAW_CALLS;
    g_caps.limits.maxBlits = BGFX_CONFIG_MAX_BLIT_ITEMS;
    g_caps.limits.maxTextureSize = 0;
    g_caps.limits.maxTextureLayers = 1;
    g_caps.limits.maxViews = BGFX_CONFIG_MAX_VIEWS;
    g_caps.limits.maxFrameBuffers = BGFX_CONFIG_MAX_FRAME_BUFFERS;
    g_caps.limits.maxPrograms = BGFX_CONFIG_MAX_PROGRAMS;
    g_caps.limits.maxShaders = BGFX_CONFIG_MAX_SHADERS;
    g_caps.limits.maxTextures = BGFX_CONFIG_MAX_TEXTURES;
    g_caps.limits.maxTextureSamplers = BGFX_CONFIG_MAX_TEXTURE_SAMPLERS;
    g_caps.limits.maxComputeBindings = 0;
    g_caps.limits.maxVertexLayouts = BGFX_CONFIG_MAX_VERTEX_LAYOUTS;
    g_caps.limits.maxVertexStreams = 1;
    g_caps.limits.maxIndexBuffers = BGFX_CONFIG_MAX_INDEX_BUFFERS;
    g_caps.limits.maxVertexBuffers = BGFX_CONFIG_MAX_VERTEX_BUFFERS;
    g_caps.limits.maxDynamicIndexBuffers = BGFX_CONFIG_MAX_DYNAMIC_INDEX_BUFFERS;
    g_caps.limits.maxDynamicVertexBuffers = BGFX_CONFIG_MAX_DYNAMIC_VERTEX_BUFFERS;
    g_caps.limits.maxUniforms = BGFX_CONFIG_MAX_UNIFORMS;
    g_caps.limits.maxOcclusionQueries = BGFX_CONFIG_MAX_OCCLUSION_QUERIES;
    g_caps.limits.maxFBAttachments = 1;
    g_caps.limits.maxEncoders = init.limits.maxEncoders;
    g_caps.limits.minResourceCbSize = init.limits.minResourceCbSize;
    g_caps.limits.transientVbSize = init.limits.transientVbSize;
    g_caps.limits.transientIbSize = init.limits.transientIbSize;

    g_caps.vendorId = init.vendorId;
    g_caps.deviceId = init.deviceId;

    BX_TRACE("Init...");

    // bgfx 1.104.7082
    //      ^ ^^^ ^^^^
    //      | |   +--- Commit number  (https://github.com/bkaradzic/bgfx / git rev-list --count HEAD)
    //      | +------- API version    (from https://github.com/bkaradzic/bgfx/blob/master/scripts/bgfx.idl#L4)
    //      +--------- Major revision (always 1)
    BX_TRACE(" 1.%d.%d (commit: " BGFX_REV_SHA1 ")", BGFX_API_VERSION, BGFX_REV_NUMBER);

    errorState = ErrorState::ContextAllocated;

    s_ctx = BX_ALIGNED_NEW(g_allocator, Context, Context::kAlignment);
    if (s_ctx->init(init))
    {
        BX_TRACE("Init complete.");
        return true;
    }

    BX_TRACE("Init failed.");

    switch (errorState)
    {
    case ErrorState::ContextAllocated:
        BX_ALIGNED_DELETE(g_allocator, s_ctx, Context::kAlignment);
        s_ctx = NULL;
        BX_FALLTHROUGH;

    case ErrorState::Default:
        if (NULL != s_callbackStub)
        {
            BX_DELETE(g_allocator, s_callbackStub);
            s_callbackStub = NULL;
        }

        if (NULL != s_allocatorStub)
        {
            bx::DefaultAllocator allocator;
            BX_DELETE(&allocator, s_allocatorStub);
            s_allocatorStub = NULL;
        }

        s_threadIndex = 0;
        g_callback = NULL;
        g_allocator = NULL;
        break;
    }

    return false;
}

void shutdown()
{
    BX_TRACE("Shutdown...");

    BGFX_CHECK_API_THREAD();
    Context *ctx = s_ctx; // it's going to be NULLd inside shutdown.
    ctx->shutdown();
    BX_ASSERT(NULL == s_ctx, "bgfx is should be uninitialized here.");

    BX_ALIGNED_DELETE(g_allocator, ctx, Context::kAlignment);

    BX_TRACE("Shutdown complete.");

    if (NULL != s_allocatorStub)
    {
        s_allocatorStub->checkLeaks();
    }

    if (NULL != s_callbackStub)
    {
        BX_DELETE(g_allocator, s_callbackStub);
        s_callbackStub = NULL;
    }

    if (NULL != s_allocatorStub)
    {
        bx::DefaultAllocator allocator;
        BX_DELETE(&allocator, s_allocatorStub);
        s_allocatorStub = NULL;
    }

    s_threadIndex = 0;
    g_callback = NULL;
    g_allocator = NULL;
}

void reset(uint32_t _width, uint32_t _height, uint32_t _flags, TextureFormat::Enum _format)
{
    BGFX_CHECK_API_THREAD();
    BX_ASSERT(0 == (_flags & BGFX_RESET_RESERVED_MASK), "Do not set reset reserved flags!");
    s_ctx->reset(_width, _height, _flags, _format);
}

void end(Encoder *_encoder)
{
    s_ctx->end(_encoder);
}

uint32_t frame(bool _capture)
{
    BGFX_CHECK_API_THREAD();
    return s_ctx->frame(_capture);
}

const Caps *getCaps()
{
    return &g_caps;
}

const Stats *getStats()
{
    return s_ctx->getPerfStats();
}

RendererType::Enum getRendererType()
{
    return g_caps.rendererType;
}

const Memory *alloc(uint32_t _size)
{
    BX_ASSERT(0 < _size, "Invalid memory operation. _size is 0.");
    Memory *mem = (Memory *)BX_ALLOC(g_allocator, sizeof(Memory) + _size);
    mem->size = _size;
    mem->data = (uint8_t *)mem + sizeof(Memory);
    return mem;
}

const Memory *copy(const void *_data, uint32_t _size)
{
    BX_ASSERT(0 < _size, "Invalid memory operation. _size is 0.");
    const Memory *mem = alloc(_size);
    bx::memCopy(mem->data, _data, _size);
    return mem;
}

struct MemoryRef
{
    Memory mem;
    ReleaseFn releaseFn;
    void *userData;
};

const Memory *makeRef(const void *_data, uint32_t _size, ReleaseFn _releaseFn, void *_userData)
{
    MemoryRef *memRef = (MemoryRef *)BX_ALLOC(g_allocator, sizeof(MemoryRef));
    memRef->mem.size = _size;
    memRef->mem.data = (uint8_t *)_data;
    memRef->releaseFn = _releaseFn;
    memRef->userData = _userData;
    return &memRef->mem;
}

bool isMemoryRef(const Memory *_mem)
{
    return _mem->data != (uint8_t *)_mem + sizeof(Memory);
}

void release(const Memory *_mem)
{
    BX_ASSERT(NULL != _mem, "_mem can't be NULL");
    Memory *mem = const_cast<Memory *>(_mem);
    if (isMemoryRef(mem))
    {
        MemoryRef *memRef = reinterpret_cast<MemoryRef *>(mem);
        if (NULL != memRef->releaseFn)
        {
            memRef->releaseFn(mem->data, memRef->userData);
        }
    }
    BX_FREE(g_allocator, mem);
}

void setDebug(uint32_t _debug)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->setDebug(_debug);
}

void dbgTextClear(uint8_t _attr, bool _small)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->dbgTextClear(_attr, _small);
}

void dbgTextPrintfVargs(uint16_t _x, uint16_t _y, uint8_t _attr, const char *_format, va_list _argList)
{
    s_ctx->dbgTextPrintfVargs(_x, _y, _attr, _format, _argList);
}

void dbgTextPrintf(uint16_t _x, uint16_t _y, uint8_t _attr, const char *_format, ...)
{
    BGFX_CHECK_API_THREAD();
    va_list argList;
    va_start(argList, _format);
    s_ctx->dbgTextPrintfVargs(_x, _y, _attr, _format, argList);
    va_end(argList);
}

void dbgTextImage(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height, const void *_data, uint16_t _pitch)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->dbgTextImage(_x, _y, _width, _height, _data, _pitch);
}

IndexBufferHandle createIndexBuffer(const Memory *_mem, uint16_t _flags)
{
    BX_ASSERT(NULL != _mem, "_mem can't be NULL");
    return s_ctx->createIndexBuffer(_mem, _flags);
}

void setName(IndexBufferHandle _handle, const char *_name, int32_t _len)
{
    s_ctx->setName(_handle, bx::StringView(_name, _len));
}

void destroy(IndexBufferHandle _handle)
{
    s_ctx->destroyIndexBuffer(_handle);
}

VertexLayoutHandle createVertexLayout(const VertexLayout &_layout)
{
    return s_ctx->createVertexLayout(_layout);
}

void destroy(VertexLayoutHandle _handle)
{
    s_ctx->destroyVertexLayout(_handle);
}

VertexBufferHandle createVertexBuffer(const Memory *_mem, const VertexLayout &_layout, uint16_t _flags)
{
    BX_ASSERT(NULL != _mem, "_mem can't be NULL");
    BX_ASSERT(isValid(_layout), "Invalid VertexLayout.");
    return s_ctx->createVertexBuffer(_mem, _layout, _flags);
}

void setName(VertexBufferHandle _handle, const char *_name, int32_t _len)
{
    s_ctx->setName(_handle, bx::StringView(_name, _len));
}

void destroy(VertexBufferHandle _handle)
{
    s_ctx->destroyVertexBuffer(_handle);
}

DynamicIndexBufferHandle createDynamicIndexBuffer(uint32_t _num, uint16_t _flags)
{
    return s_ctx->createDynamicIndexBuffer(_num, _flags);
}

DynamicIndexBufferHandle createDynamicIndexBuffer(const Memory *_mem, uint16_t _flags)
{
    BX_ASSERT(NULL != _mem, "_mem can't be NULL");
    return s_ctx->createDynamicIndexBuffer(_mem, _flags);
}

void update(DynamicIndexBufferHandle _handle, uint32_t _startIndex, const Memory *_mem)
{
    BX_ASSERT(NULL != _mem, "_mem can't be NULL");
    s_ctx->update(_handle, _startIndex, _mem);
}

void destroy(DynamicIndexBufferHandle _handle)
{
    s_ctx->destroyDynamicIndexBuffer(_handle);
}

DynamicVertexBufferHandle createDynamicVertexBuffer(uint32_t _num, const VertexLayout &_layout, uint16_t _flags)
{
    BX_ASSERT(isValid(_layout), "Invalid VertexLayout.");
    return s_ctx->createDynamicVertexBuffer(_num, _layout, _flags);
}

DynamicVertexBufferHandle createDynamicVertexBuffer(const Memory *_mem, const VertexLayout &_layout, uint16_t _flags)
{
    BX_ASSERT(NULL != _mem, "_mem can't be NULL");
    BX_ASSERT(isValid(_layout), "Invalid VertexLayout.");
    return s_ctx->createDynamicVertexBuffer(_mem, _layout, _flags);
}

void update(DynamicVertexBufferHandle _handle, uint32_t _startVertex, const Memory *_mem)
{
    BX_ASSERT(NULL != _mem, "_mem can't be NULL");
    s_ctx->update(_handle, _startVertex, _mem);
}

void destroy(DynamicVertexBufferHandle _handle)
{
    s_ctx->destroyDynamicVertexBuffer(_handle);
}

uint32_t getAvailTransientIndexBuffer(uint32_t _num)
{
    BX_ASSERT(0 < _num, "Requesting 0 indices.");
    return s_ctx->getAvailTransientIndexBuffer(_num);
}

uint32_t getAvailTransientVertexBuffer(uint32_t _num, const VertexLayout &_layout)
{
    BX_ASSERT(0 < _num, "Requesting 0 vertices.");
    BX_ASSERT(isValid(_layout), "Invalid VertexLayout.");
    return s_ctx->getAvailTransientVertexBuffer(_num, _layout.m_stride);
}

uint32_t getAvailInstanceDataBuffer(uint32_t _num, uint16_t _stride)
{
    BX_ASSERT(0 < _num, "Requesting 0 instances.");
    return s_ctx->getAvailTransientVertexBuffer(_num, _stride);
}

void allocTransientIndexBuffer(TransientIndexBuffer *_tib, uint32_t _num)
{
    BX_ASSERT(NULL != _tib, "_tib can't be NULL");
    BX_ASSERT(0 < _num, "Requesting 0 indices.");
    s_ctx->allocTransientIndexBuffer(_tib, _num);
    BX_ASSERT(_num == _tib->size / 2,
              "Failed to allocate transient index buffer (requested %d, available %d). "
              "Use bgfx::getAvailTransient* functions to ensure availability.",
              _num, _tib->size / 2);
}

void allocTransientVertexBuffer(TransientVertexBuffer *_tvb, uint32_t _num, const VertexLayout &_layout)
{
    BX_ASSERT(NULL != _tvb, "_tvb can't be NULL");
    BX_ASSERT(0 < _num, "Requesting 0 vertices.");
    BX_ASSERT(isValid(_layout), "Invalid VertexLayout.");
    s_ctx->allocTransientVertexBuffer(_tvb, _num, _layout);
    BX_ASSERT(_num == _tvb->size / _layout.m_stride,
              "Failed to allocate transient vertex buffer (requested %d, available %d). "
              "Use bgfx::getAvailTransient* functions to ensure availability.",
              _num, _tvb->size / _layout.m_stride);
}

bool allocTransientBuffers(bgfx::TransientVertexBuffer *_tvb, const bgfx::VertexLayout &_layout, uint32_t _numVertices,
                           bgfx::TransientIndexBuffer *_tib, uint32_t _numIndices)
{
    BGFX_MUTEX_SCOPE(s_ctx->m_resourceApiLock);

    if (_numVertices == getAvailTransientVertexBuffer(_numVertices, _layout) &&
        _numIndices == getAvailTransientIndexBuffer(_numIndices))
    {
        allocTransientVertexBuffer(_tvb, _numVertices, _layout);
        allocTransientIndexBuffer(_tib, _numIndices);
        return true;
    }

    return false;
}

void allocInstanceDataBuffer(InstanceDataBuffer *_idb, uint32_t _num, uint16_t _stride)
{
    BGFX_CHECK_CAPS(BGFX_CAPS_INSTANCING, "Instancing is not supported!");
    BX_ASSERT(bx::isAligned(_stride, 16), "Stride must be multiple of 16.");
    BX_ASSERT(0 < _num, "Requesting 0 instanced data vertices.");
    s_ctx->allocInstanceDataBuffer(_idb, _num, _stride);
    BX_ASSERT(_num == _idb->size / _stride,
              "Failed to allocate instance data buffer (requested %d, available %d). "
              "Use bgfx::getAvailTransient* functions to ensure availability.",
              _num, _idb->size / _stride);
}

IndirectBufferHandle createIndirectBuffer(uint32_t _num)
{
    return s_ctx->createIndirectBuffer(_num);
}

void destroy(IndirectBufferHandle _handle)
{
    s_ctx->destroyIndirectBuffer(_handle);
}

ShaderHandle createShader(const Memory *_mem)
{
    BX_ASSERT(NULL != _mem, "_mem can't be NULL");
    return s_ctx->createShader(_mem);
}

uint16_t getShaderUniforms(ShaderHandle _handle, UniformHandle *_uniforms, uint16_t _max)
{
    BX_WARN(NULL == _uniforms || 0 != _max, "Passing uniforms array pointer, but array maximum capacity is set to 0.");

    uint16_t num = s_ctx->getShaderUniforms(_handle, _uniforms, _max);

    BX_WARN(0 == _max || num <= _max,
            "Shader has more uniforms that capacity of output array. Output is truncated (num %d, max %d).", num, _max);

    return num;
}

void setName(ShaderHandle _handle, const char *_name, int32_t _len)
{
    s_ctx->setName(_handle, bx::StringView(_name, _len));
}

void destroy(ShaderHandle _handle)
{
    s_ctx->destroyShader(_handle);
}

ProgramHandle createProgram(ShaderHandle _vsh, ShaderHandle _fsh, bool _destroyShaders)
{
    if (!isValid(_fsh))
    {
        return createProgram(_vsh, _destroyShaders);
    }

    return s_ctx->createProgram(_vsh, _fsh, _destroyShaders);
}

ProgramHandle createProgram(ShaderHandle _csh, bool _destroyShader)
{
    return s_ctx->createProgram(_csh, _destroyShader);
}

void destroy(ProgramHandle _handle)
{
    s_ctx->destroyProgram(_handle);
}

static void isTextureValid(uint16_t _depth, bool _cubeMap, uint16_t _numLayers, TextureFormat::Enum _format,
                           uint64_t _flags, bx::Error *_err)
{
    BX_ERROR_SCOPE(_err);

    const bool is3DTexture = 1 < _depth;

    if (_cubeMap && is3DTexture)
    {
        _err->setError(BGFX_ERROR_TEXTURE_VALIDATION, "Texture can't be depth and cube map at the same time.");
        return;
    }

    if (is3DTexture && 0 == (g_caps.supported & BGFX_CAPS_TEXTURE_3D))
    {
        _err->setError(BGFX_ERROR_TEXTURE_VALIDATION,
                       "Texture3D is not supported! "
                       "Use bgfx::getCaps to check BGFX_CAPS_TEXTURE_3D backend renderer capabilities.");
        return;
    }

    if (0 != (_flags & BGFX_TEXTURE_RT_MASK) && 0 != (_flags & BGFX_TEXTURE_READ_BACK))
    {
        _err->setError(BGFX_ERROR_TEXTURE_VALIDATION, "Can't create render target with BGFX_TEXTURE_READ_BACK flag.");
        return;
    }

    if (1 < _numLayers && 0 == (g_caps.supported & BGFX_CAPS_TEXTURE_2D_ARRAY))
    {
        _err->setError(BGFX_ERROR_TEXTURE_VALIDATION,
                       "Texture array is not supported! "
                       "Use bgfx::getCaps to check BGFX_CAPS_TEXTURE_2D_ARRAY backend renderer capabilities.");
        return;
    }

    bool formatSupported;
    if (0 != (_flags & (BGFX_TEXTURE_RT | BGFX_TEXTURE_RT_WRITE_ONLY)))
    {
        formatSupported = 0 != (g_caps.formats[_format] & BGFX_CAPS_FORMAT_TEXTURE_FRAMEBUFFER);
    }
    else
    {
        formatSupported =
            0 != (g_caps.formats[_format] & (0 | BGFX_CAPS_FORMAT_TEXTURE_2D | BGFX_CAPS_FORMAT_TEXTURE_2D_EMULATED |
                                             BGFX_CAPS_FORMAT_TEXTURE_2D_SRGB));
    }

    uint16_t srgbCaps = BGFX_CAPS_FORMAT_TEXTURE_2D_SRGB;

    if (_cubeMap)
    {
        formatSupported = 0 != (g_caps.formats[_format] &
                                (0 | BGFX_CAPS_FORMAT_TEXTURE_CUBE | BGFX_CAPS_FORMAT_TEXTURE_CUBE_EMULATED |
                                 BGFX_CAPS_FORMAT_TEXTURE_CUBE_SRGB));
        srgbCaps = BGFX_CAPS_FORMAT_TEXTURE_CUBE_SRGB;
    }
    else if (is3DTexture)
    {
        formatSupported =
            0 != (g_caps.formats[_format] & (0 | BGFX_CAPS_FORMAT_TEXTURE_3D | BGFX_CAPS_FORMAT_TEXTURE_3D_EMULATED |
                                             BGFX_CAPS_FORMAT_TEXTURE_3D_SRGB));
        srgbCaps = BGFX_CAPS_FORMAT_TEXTURE_3D_SRGB;
    }

    if (formatSupported && 0 != (_flags & BGFX_TEXTURE_RT_MASK))
    {
        formatSupported = 0 != (g_caps.formats[_format] & (0 | BGFX_CAPS_FORMAT_TEXTURE_FRAMEBUFFER));
    }

    if (!formatSupported)
    {
        _err->setError(BGFX_ERROR_TEXTURE_VALIDATION,
                       "Texture format is not supported! "
                       "Use bgfx::isTextureValid to check support for texture format before creating it.");
        return;
    }

    if (0 != (_flags & BGFX_TEXTURE_MSAA_SAMPLE) && 0 == (g_caps.formats[_format] & BGFX_CAPS_FORMAT_TEXTURE_MSAA))
    {
        _err->setError(BGFX_ERROR_TEXTURE_VALIDATION, "MSAA sampling for this texture format is not supported.");
        return;
    }

    if (0 != (_flags & BGFX_TEXTURE_SRGB) &&
        0 == (g_caps.formats[_format] & srgbCaps &
              (0 | BGFX_CAPS_FORMAT_TEXTURE_2D_SRGB | BGFX_CAPS_FORMAT_TEXTURE_3D_SRGB |
               BGFX_CAPS_FORMAT_TEXTURE_CUBE_SRGB)))
    {
        _err->setError(BGFX_ERROR_TEXTURE_VALIDATION, "sRGB sampling for this texture format is not supported.");
        return;
    }
}

bool isTextureValid(uint16_t _depth, bool _cubeMap, uint16_t _numLayers, TextureFormat::Enum _format, uint64_t _flags)
{
    bx::Error err;
    isTextureValid(_depth, _cubeMap, _numLayers, _format, _flags, &err);
    return err.isOk();
}

void calcTextureSize(TextureInfo &_info, uint16_t _width, uint16_t _height, uint16_t _depth, bool _cubeMap,
                     bool _hasMips, uint16_t _numLayers, TextureFormat::Enum _format)
{
    bimg::imageGetSize((bimg::TextureInfo *)&_info, _width, _height, _depth, _cubeMap, _hasMips, _numLayers,
                       bimg::TextureFormat::Enum(_format));
}

TextureHandle createTexture(const Memory *_mem, uint64_t _flags, uint8_t _skip, TextureInfo *_info)
{
    BX_ASSERT(NULL != _mem, "_mem can't be NULL");
    return s_ctx->createTexture(_mem, _flags, _skip, _info, BackbufferRatio::Count, false);
}

void getTextureSizeFromRatio(BackbufferRatio::Enum _ratio, uint16_t &_width, uint16_t &_height)
{
    switch (_ratio)
    {
    case BackbufferRatio::Half:
        _width /= 2;
        _height /= 2;
        break;
    case BackbufferRatio::Quarter:
        _width /= 4;
        _height /= 4;
        break;
    case BackbufferRatio::Eighth:
        _width /= 8;
        _height /= 8;
        break;
    case BackbufferRatio::Sixteenth:
        _width /= 16;
        _height /= 16;
        break;
    case BackbufferRatio::Double:
        _width *= 2;
        _height *= 2;
        break;

    default:
        break;
    }

    _width = bx::max<uint16_t>(1, _width);
    _height = bx::max<uint16_t>(1, _height);
}

static TextureHandle createTexture2D(BackbufferRatio::Enum _ratio, uint16_t _width, uint16_t _height, bool _hasMips,
                                     uint16_t _numLayers, TextureFormat::Enum _format, uint64_t _flags,
                                     const Memory *_mem)
{
    bx::Error err;
    isTextureValid(0, false, _numLayers, _format, _flags, &err);
    BX_ASSERT(err.isOk(), "%s (layers %d, format %s)", err.getMessage().getPtr(), _numLayers, getName(_format));

    if (BackbufferRatio::Count != _ratio)
    {
        _width = uint16_t(s_ctx->m_init.resolution.width);
        _height = uint16_t(s_ctx->m_init.resolution.height);
        getTextureSizeFromRatio(_ratio, _width, _height);
    }

    const uint8_t numMips = calcNumMips(_hasMips, _width, _height);
    _numLayers = bx::max<uint16_t>(_numLayers, 1);

    if (BX_ENABLED(BGFX_CONFIG_DEBUG) && NULL != _mem)
    {
        TextureInfo ti;
        calcTextureSize(ti, _width, _height, 1, false, _hasMips, _numLayers, _format);
        BX_ASSERT(ti.storageSize == _mem->size,
                  "createTexture2D: Texture storage size doesn't match passed memory size (storage size: %d, memory "
                  "size: %d)",
                  ti.storageSize, _mem->size);
    }

    uint32_t size = sizeof(uint32_t) + sizeof(TextureCreate);
    const Memory *mem = alloc(size);

    bx::StaticMemoryBlockWriter writer(mem->data, mem->size);
    uint32_t magic = BGFX_CHUNK_MAGIC_TEX;
    bx::write(&writer, magic);

    TextureCreate tc;
    tc.m_width = _width;
    tc.m_height = _height;
    tc.m_depth = 0;
    tc.m_numLayers = _numLayers;
    tc.m_numMips = numMips;
    tc.m_format = _format;
    tc.m_cubeMap = false;
    tc.m_mem = _mem;
    bx::write(&writer, tc);

    return s_ctx->createTexture(mem, _flags, 0, NULL, _ratio, NULL != _mem);
}

TextureHandle createTexture2D(uint16_t _width, uint16_t _height, bool _hasMips, uint16_t _numLayers,
                              TextureFormat::Enum _format, uint64_t _flags, const Memory *_mem)
{
    BX_ASSERT(_width > 0 && _height > 0, "Invalid texture size (width %d, height %d).", _width, _height);
    return createTexture2D(BackbufferRatio::Count, _width, _height, _hasMips, _numLayers, _format, _flags, _mem);
}

TextureHandle createTexture2D(BackbufferRatio::Enum _ratio, bool _hasMips, uint16_t _numLayers,
                              TextureFormat::Enum _format, uint64_t _flags)
{
    BX_ASSERT(_ratio < BackbufferRatio::Count, "Invalid back buffer ratio.");
    return createTexture2D(_ratio, 0, 0, _hasMips, _numLayers, _format, _flags, NULL);
}

TextureHandle createTexture3D(uint16_t _width, uint16_t _height, uint16_t _depth, bool _hasMips,
                              TextureFormat::Enum _format, uint64_t _flags, const Memory *_mem)
{
    bx::Error err;
    isTextureValid(_depth, false, 1, _format, _flags, &err);
    BX_ASSERT(err.isOk(), "%s", err.getMessage().getPtr());

    const uint8_t numMips = calcNumMips(_hasMips, _width, _height, _depth);

    if (BX_ENABLED(BGFX_CONFIG_DEBUG) && NULL != _mem)
    {
        TextureInfo ti;
        calcTextureSize(ti, _width, _height, _depth, false, _hasMips, 1, _format);
        BX_ASSERT(ti.storageSize == _mem->size,
                  "createTexture3D: Texture storage size doesn't match passed memory size (storage size: %d, memory "
                  "size: %d)",
                  ti.storageSize, _mem->size);
    }

    uint32_t size = sizeof(uint32_t) + sizeof(TextureCreate);
    const Memory *mem = alloc(size);

    bx::StaticMemoryBlockWriter writer(mem->data, mem->size);
    uint32_t magic = BGFX_CHUNK_MAGIC_TEX;
    bx::write(&writer, magic);

    TextureCreate tc;
    tc.m_width = _width;
    tc.m_height = _height;
    tc.m_depth = _depth;
    tc.m_numLayers = 1;
    tc.m_numMips = numMips;
    tc.m_format = _format;
    tc.m_cubeMap = false;
    tc.m_mem = _mem;
    bx::write(&writer, tc);

    return s_ctx->createTexture(mem, _flags, 0, NULL, BackbufferRatio::Count, NULL != _mem);
}

TextureHandle createTextureCube(uint16_t _size, bool _hasMips, uint16_t _numLayers, TextureFormat::Enum _format,
                                uint64_t _flags, const Memory *_mem)
{
    bx::Error err;
    isTextureValid(0, true, _numLayers, _format, _flags, &err);
    BX_ASSERT(err.isOk(), "%s", err.getMessage().getPtr());

    const uint8_t numMips = calcNumMips(_hasMips, _size, _size);
    _numLayers = bx::max<uint16_t>(_numLayers, 1);

    if (BX_ENABLED(BGFX_CONFIG_DEBUG) && NULL != _mem)
    {
        TextureInfo ti;
        calcTextureSize(ti, _size, _size, 1, true, _hasMips, _numLayers, _format);
        BX_ASSERT(ti.storageSize == _mem->size,
                  "createTextureCube: Texture storage size doesn't match passed memory size (storage size: %d, memory "
                  "size: %d)",
                  ti.storageSize, _mem->size);
    }

    uint32_t size = sizeof(uint32_t) + sizeof(TextureCreate);
    const Memory *mem = alloc(size);

    bx::StaticMemoryBlockWriter writer(mem->data, mem->size);
    uint32_t magic = BGFX_CHUNK_MAGIC_TEX;
    bx::write(&writer, magic);

    TextureCreate tc;
    tc.m_width = _size;
    tc.m_height = _size;
    tc.m_depth = 0;
    tc.m_numLayers = _numLayers;
    tc.m_numMips = numMips;
    tc.m_format = _format;
    tc.m_cubeMap = true;
    tc.m_mem = _mem;
    bx::write(&writer, tc);

    return s_ctx->createTexture(mem, _flags, 0, NULL, BackbufferRatio::Count, NULL != _mem);
}

void setName(TextureHandle _handle, const char *_name, int32_t _len)
{
    s_ctx->setName(_handle, bx::StringView(_name, _len));
}

void *getDirectAccessPtr(TextureHandle _handle)
{
    return s_ctx->getDirectAccessPtr(_handle);
}

void destroy(TextureHandle _handle)
{
    s_ctx->destroyTexture(_handle);
}

void updateTexture2D(TextureHandle _handle, uint16_t _layer, uint8_t _mip, uint16_t _x, uint16_t _y, uint16_t _width,
                     uint16_t _height, const Memory *_mem, uint16_t _pitch)
{
    BX_ASSERT(NULL != _mem, "_mem can't be NULL");
    if (_width == 0 || _height == 0)
    {
        release(_mem);
    }
    else
    {
        s_ctx->updateTexture(_handle, 0, _mip, _x, _y, _layer, _width, _height, 1, _pitch, _mem);
    }
}

void updateTexture3D(TextureHandle _handle, uint8_t _mip, uint16_t _x, uint16_t _y, uint16_t _z, uint16_t _width,
                     uint16_t _height, uint16_t _depth, const Memory *_mem)
{
    BX_ASSERT(NULL != _mem, "_mem can't be NULL");
    BGFX_CHECK_CAPS(BGFX_CAPS_TEXTURE_3D, "Texture3D is not supported!");

    if (0 == _width || 0 == _height || 0 == _depth)
    {
        release(_mem);
    }
    else
    {
        s_ctx->updateTexture(_handle, 0, _mip, _x, _y, _z, _width, _height, _depth, UINT16_MAX, _mem);
    }
}

void updateTextureCube(TextureHandle _handle, uint16_t _layer, uint8_t _side, uint8_t _mip, uint16_t _x, uint16_t _y,
                       uint16_t _width, uint16_t _height, const Memory *_mem, uint16_t _pitch)
{
    BX_ASSERT(NULL != _mem, "_mem can't be NULL");
    BX_ASSERT(_side <= 5, "Invalid side %d.", _side);
    if (0 == _width || 0 == _height)
    {
        release(_mem);
    }
    else
    {
        s_ctx->updateTexture(_handle, _side, _mip, _x, _y, _layer, _width, _height, 1, _pitch, _mem);
    }
}

uint32_t readTexture(TextureHandle _handle, void *_data, uint8_t _mip)
{
    BX_ASSERT(NULL != _data, "_data can't be NULL");
    BGFX_CHECK_CAPS(BGFX_CAPS_TEXTURE_READ_BACK, "Texture read-back is not supported!");
    return s_ctx->readTexture(_handle, _data, _mip);
}

FrameBufferHandle createFrameBuffer(uint16_t _width, uint16_t _height, TextureFormat::Enum _format,
                                    uint64_t _textureFlags)
{
    _textureFlags |= _textureFlags & BGFX_TEXTURE_RT_MSAA_MASK ? 0 : BGFX_TEXTURE_RT;
    TextureHandle th = createTexture2D(_width, _height, false, 1, _format, _textureFlags);
    return createFrameBuffer(1, &th, true);
}

FrameBufferHandle createFrameBuffer(BackbufferRatio::Enum _ratio, TextureFormat::Enum _format, uint64_t _textureFlags)
{
    BX_ASSERT(_ratio < BackbufferRatio::Count, "Invalid back buffer ratio.");
    _textureFlags |= _textureFlags & BGFX_TEXTURE_RT_MSAA_MASK ? 0 : BGFX_TEXTURE_RT;
    TextureHandle th = createTexture2D(_ratio, false, 1, _format, _textureFlags);
    return createFrameBuffer(1, &th, true);
}

FrameBufferHandle createFrameBuffer(uint8_t _num, const TextureHandle *_handles, bool _destroyTextures)
{
    Attachment attachment[BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS];
    for (uint8_t ii = 0; ii < _num; ++ii)
    {
        Attachment &at = attachment[ii];
        at.init(_handles[ii], Access::Write, 0, 0, BGFX_RESOLVE_AUTO_GEN_MIPS);
    }
    return createFrameBuffer(_num, attachment, _destroyTextures);
}

FrameBufferHandle createFrameBuffer(uint8_t _num, const Attachment *_attachment, bool _destroyTextures)
{
    BX_ASSERT(_num != 0, "Number of frame buffer attachments can't be 0.");
    BX_ASSERT(_num <= BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS,
              "Number of frame buffer attachments is larger than allowed %d (max: %d).", _num,
              BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS);
    BX_ASSERT(NULL != _attachment, "_attachment can't be NULL");
    return s_ctx->createFrameBuffer(_num, _attachment, _destroyTextures);
}

FrameBufferHandle createFrameBuffer(void *_nwh, uint16_t _width, uint16_t _height, TextureFormat::Enum _format,
                                    TextureFormat::Enum _depthFormat)
{
    BGFX_CHECK_CAPS(BGFX_CAPS_SWAP_CHAIN, "Swap chain is not supported!");
    BX_WARN(_width > 0 && _height > 0, "Invalid frame buffer dimensions (width %d, height %d).", _width, _height);
    BX_ASSERT(_format == TextureFormat::Count || bimg::isColor(bimg::TextureFormat::Enum(_format)),
              "Invalid texture format for color (%s).", bimg::getName(bimg::TextureFormat::Enum(_format)));
    BX_ASSERT(_depthFormat == TextureFormat::Count || bimg::isDepth(bimg::TextureFormat::Enum(_depthFormat)),
              "Invalid texture format for depth (%s).", bimg::getName(bimg::TextureFormat::Enum(_depthFormat)));
    return s_ctx->createFrameBuffer(_nwh, bx::max<uint16_t>(_width, 1), bx::max<uint16_t>(_height, 1), _format,
                                    _depthFormat);
}

void setName(FrameBufferHandle _handle, const char *_name, int32_t _len)
{
    s_ctx->setName(_handle, bx::StringView(_name, _len));
}

TextureHandle getTexture(FrameBufferHandle _handle, uint8_t _attachment)
{
    return s_ctx->getTexture(_handle, _attachment);
}

void destroy(FrameBufferHandle _handle)
{
    s_ctx->destroyFrameBuffer(_handle);
}

UniformHandle createUniform(const char *_name, UniformType::Enum _type, uint16_t _num)
{
    return s_ctx->createUniform(_name, _type, _num);
}

void getUniformInfo(UniformHandle _handle, UniformInfo &_info)
{
    s_ctx->getUniformInfo(_handle, _info);
}

void destroy(UniformHandle _handle)
{
    s_ctx->destroyUniform(_handle);
}

OcclusionQueryHandle createOcclusionQuery()
{
    BGFX_CHECK_CAPS(BGFX_CAPS_OCCLUSION_QUERY, "Occlusion query is not supported!");
    return s_ctx->createOcclusionQuery();
}

OcclusionQueryResult::Enum getResult(OcclusionQueryHandle _handle, int32_t *_result)
{
    BGFX_CHECK_CAPS(BGFX_CAPS_OCCLUSION_QUERY, "Occlusion query is not supported!");
    return s_ctx->getResult(_handle, _result);
}

void destroy(OcclusionQueryHandle _handle)
{
    BGFX_CHECK_CAPS(BGFX_CAPS_OCCLUSION_QUERY, "Occlusion query is not supported!");
    s_ctx->destroyOcclusionQuery(_handle);
}

void setPaletteColor(uint8_t _index, uint32_t _rgba)
{
    const uint8_t rr = uint8_t(_rgba >> 24);
    const uint8_t gg = uint8_t(_rgba >> 16);
    const uint8_t bb = uint8_t(_rgba >> 8);
    const uint8_t aa = uint8_t(_rgba >> 0);

    const float rgba[4] = {
        rr * 1.0f / 255.0f,
        gg * 1.0f / 255.0f,
        bb * 1.0f / 255.0f,
        aa * 1.0f / 255.0f,
    };

    s_ctx->setPaletteColor(_index, rgba);
}

void setPaletteColor(uint8_t _index, float _r, float _g, float _b, float _a)
{
    float rgba[4] = {_r, _g, _b, _a};
    s_ctx->setPaletteColor(_index, rgba);
}

void setPaletteColor(uint8_t _index, const float _rgba[4])
{
    s_ctx->setPaletteColor(_index, _rgba);
}

bool checkView(ViewId _id)
{
    // workaround GCC 4.9 type-limit check.
    const uint32_t id = _id;
    return id < BGFX_CONFIG_MAX_VIEWS;
}

void setViewName(ViewId _id, const char *_name)
{
    BX_ASSERT(checkView(_id), "Invalid view id: %d", _id);
    s_ctx->setViewName(_id, _name);
}

void setViewRect(ViewId _id, uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height)
{
    BX_ASSERT(checkView(_id), "Invalid view id: %d", _id);
    s_ctx->setViewRect(_id, _x, _y, _width, _height);
}

void setViewRect(ViewId _id, uint16_t _x, uint16_t _y, BackbufferRatio::Enum _ratio)
{
    BX_ASSERT(checkView(_id), "Invalid view id: %d", _id);

    uint16_t width = uint16_t(s_ctx->m_init.resolution.width);
    uint16_t height = uint16_t(s_ctx->m_init.resolution.height);
    getTextureSizeFromRatio(_ratio, width, height);
    setViewRect(_id, _x, _y, width, height);
}

void setViewScissor(ViewId _id, uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height)
{
    BX_ASSERT(checkView(_id), "Invalid view id: %d", _id);
    s_ctx->setViewScissor(_id, _x, _y, _width, _height);
}

void setViewClear(ViewId _id, uint16_t _flags, uint32_t _rgba, float _depth, uint8_t _stencil)
{
    BX_ASSERT(checkView(_id), "Invalid view id: %d", _id);
    s_ctx->setViewClear(_id, _flags, _rgba, _depth, _stencil);
}

void setViewClear(ViewId _id, uint16_t _flags, float _depth, uint8_t _stencil, uint8_t _0, uint8_t _1, uint8_t _2,
                  uint8_t _3, uint8_t _4, uint8_t _5, uint8_t _6, uint8_t _7)
{
    BX_ASSERT(checkView(_id), "Invalid view id: %d", _id);
    s_ctx->setViewClear(_id, _flags, _depth, _stencil, _0, _1, _2, _3, _4, _5, _6, _7);
}

void setViewMode(ViewId _id, ViewMode::Enum _mode)
{
    BX_ASSERT(checkView(_id), "Invalid view id: %d", _id);
    s_ctx->setViewMode(_id, _mode);
}

void setViewFrameBuffer(ViewId _id, FrameBufferHandle _handle)
{
    BX_ASSERT(checkView(_id), "Invalid view id: %d", _id);
    s_ctx->setViewFrameBuffer(_id, _handle);
}

void setViewTransform(ViewId _id, const void *_view, const void *_proj)
{
    BX_ASSERT(checkView(_id), "Invalid view id: %d", _id);
    s_ctx->setViewTransform(_id, _view, _proj);
}

void setViewOrder(ViewId _id, uint16_t _num, const ViewId *_order)
{
    BX_ASSERT(checkView(_id), "Invalid view id: %d", _id);
    s_ctx->setViewOrder(_id, _num, _order);
}

void resetView(ViewId _id)
{
    BX_ASSERT(checkView(_id), "Invalid view id: %d", _id);
    s_ctx->resetView(_id);
}

void setMarker(const char *_marker)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setMarker(_marker);
}

void setState(uint64_t _state, uint32_t _rgba)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setState(_state, _rgba);
}

void setCondition(OcclusionQueryHandle _handle, bool _visible)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setCondition(_handle, _visible);
}

void setStencil(uint32_t _fstencil, uint32_t _bstencil)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setStencil(_fstencil, _bstencil);
}

uint16_t setScissor(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height)
{
    BGFX_CHECK_API_THREAD();
    return s_ctx->m_encoder0->setScissor(_x, _y, _width, _height);
}

void setScissor(uint16_t _cache)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setScissor(_cache);
}

uint32_t setTransform(const void *_mtx, uint16_t _num)
{
    BGFX_CHECK_API_THREAD();
    return s_ctx->m_encoder0->setTransform(_mtx, _num);
}

uint32_t allocTransform(Transform *_transform, uint16_t _num)
{
    BGFX_CHECK_API_THREAD();
    return s_ctx->m_encoder0->allocTransform(_transform, _num);
}

void setTransform(uint32_t _cache, uint16_t _num)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setTransform(_cache, _num);
}

void setUniform(UniformHandle _handle, const void *_value, uint16_t _num)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setUniform(_handle, _value, _num);
}

void setIndexBuffer(IndexBufferHandle _handle)
{
    setIndexBuffer(_handle, 0, UINT32_MAX);
}

void setIndexBuffer(IndexBufferHandle _handle, uint32_t _firstIndex, uint32_t _numIndices)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setIndexBuffer(_handle, _firstIndex, _numIndices);
}

void setIndexBuffer(DynamicIndexBufferHandle _handle)
{
    setIndexBuffer(_handle, 0, UINT32_MAX);
}

void setIndexBuffer(DynamicIndexBufferHandle _handle, uint32_t _firstIndex, uint32_t _numIndices)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setIndexBuffer(_handle, _firstIndex, _numIndices);
}

void setIndexBuffer(const TransientIndexBuffer *_tib)
{
    setIndexBuffer(_tib, 0, UINT32_MAX);
}

void setIndexBuffer(const TransientIndexBuffer *_tib, uint32_t _firstIndex, uint32_t _numIndices)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setIndexBuffer(_tib, _firstIndex, _numIndices);
}

void setVertexBuffer(uint8_t _stream, VertexBufferHandle _handle, uint32_t _startVertex, uint32_t _numVertices,
                     VertexLayoutHandle _layoutHandle)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setVertexBuffer(_stream, _handle, _startVertex, _numVertices, _layoutHandle);
}

void setVertexBuffer(uint8_t _stream, VertexBufferHandle _handle)
{
    setVertexBuffer(_stream, _handle, 0, UINT32_MAX);
}

void setVertexBuffer(uint8_t _stream, DynamicVertexBufferHandle _handle, uint32_t _startVertex, uint32_t _numVertices,
                     VertexLayoutHandle _layoutHandle)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setVertexBuffer(_stream, _handle, _startVertex, _numVertices, _layoutHandle);
}

void setVertexBuffer(uint8_t _stream, DynamicVertexBufferHandle _handle)
{
    setVertexBuffer(_stream, _handle, 0, UINT32_MAX);
}

void setVertexBuffer(uint8_t _stream, const TransientVertexBuffer *_tvb, uint32_t _startVertex, uint32_t _numVertices,
                     VertexLayoutHandle _layoutHandle)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setVertexBuffer(_stream, _tvb, _startVertex, _numVertices, _layoutHandle);
}

void setVertexBuffer(uint8_t _stream, const TransientVertexBuffer *_tvb)
{
    setVertexBuffer(_stream, _tvb, 0, UINT32_MAX);
}

void setVertexCount(uint32_t _numVertices)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setVertexCount(_numVertices);
}

void setInstanceDataBuffer(const InstanceDataBuffer *_idb)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setInstanceDataBuffer(_idb);
}

void setInstanceDataBuffer(const InstanceDataBuffer *_idb, uint32_t _start, uint32_t _num)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setInstanceDataBuffer(_idb, _start, _num);
}

void setInstanceDataBuffer(VertexBufferHandle _handle, uint32_t _startVertex, uint32_t _num)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setInstanceDataBuffer(_handle, _startVertex, _num);
}

void setInstanceDataBuffer(DynamicVertexBufferHandle _handle, uint32_t _startVertex, uint32_t _num)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setInstanceDataBuffer(_handle, _startVertex, _num);
}

void setInstanceCount(uint32_t _numInstances)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setInstanceCount(_numInstances);
}

void setTexture(uint8_t _stage, UniformHandle _sampler, TextureHandle _handle, uint32_t _flags)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setTexture(_stage, _sampler, _handle, _flags);
}

void touch(ViewId _id)
{
    ProgramHandle handle = BGFX_INVALID_HANDLE;
    submit(_id, handle);
}

void submit(ViewId _id, ProgramHandle _program, uint32_t _depth, uint8_t _flags)
{
    OcclusionQueryHandle handle = BGFX_INVALID_HANDLE;
    submit(_id, _program, handle, _depth, _flags);
}

void submit(ViewId _id, ProgramHandle _program, OcclusionQueryHandle _occlusionQuery, uint32_t _depth, uint8_t _flags)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->submit(_id, _program, _occlusionQuery, _depth, _flags);
}

void submit(ViewId _id, ProgramHandle _program, IndirectBufferHandle _indirectHandle, uint16_t _start, uint16_t _num,
            uint32_t _depth, uint8_t _flags)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->submit(_id, _program, _indirectHandle, _start, _num, _depth, _flags);
}

void setBuffer(uint8_t _stage, IndexBufferHandle _handle, Access::Enum _access)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setBuffer(_stage, _handle, _access);
}

void setBuffer(uint8_t _stage, VertexBufferHandle _handle, Access::Enum _access)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setBuffer(_stage, _handle, _access);
}

void setBuffer(uint8_t _stage, DynamicIndexBufferHandle _handle, Access::Enum _access)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setBuffer(_stage, _handle, _access);
}

void setBuffer(uint8_t _stage, DynamicVertexBufferHandle _handle, Access::Enum _access)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setBuffer(_stage, _handle, _access);
}

void setBuffer(uint8_t _stage, IndirectBufferHandle _handle, Access::Enum _access)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setBuffer(_stage, _handle, _access);
}

void setImage(uint8_t _stage, TextureHandle _handle, uint8_t _mip, Access::Enum _access, TextureFormat::Enum _format)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->setImage(_stage, _handle, _mip, _access, _format);
}

void dispatch(ViewId _id, ProgramHandle _handle, uint32_t _numX, uint32_t _numY, uint32_t _numZ, uint8_t _flags)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->dispatch(_id, _handle, _numX, _numY, _numZ, _flags);
}

void dispatch(ViewId _id, ProgramHandle _handle, IndirectBufferHandle _indirectHandle, uint16_t _start, uint16_t _num,
              uint8_t _flags)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->dispatch(_id, _handle, _indirectHandle, _start, _num, _flags);
}

void discard(uint8_t _flags)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->discard(_flags);
}

void blit(ViewId _id, TextureHandle _dst, uint16_t _dstX, uint16_t _dstY, TextureHandle _src, uint16_t _srcX,
          uint16_t _srcY, uint16_t _width, uint16_t _height)
{
    blit(_id, _dst, 0, _dstX, _dstY, 0, _src, 0, _srcX, _srcY, 0, _width, _height, 0);
}

void blit(ViewId _id, TextureHandle _dst, uint8_t _dstMip, uint16_t _dstX, uint16_t _dstY, uint16_t _dstZ,
          TextureHandle _src, uint8_t _srcMip, uint16_t _srcX, uint16_t _srcY, uint16_t _srcZ, uint16_t _width,
          uint16_t _height, uint16_t _depth)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->m_encoder0->blit(_id, _dst, _dstMip, _dstX, _dstY, _dstZ, _src, _srcMip, _srcX, _srcY, _srcZ, _width,
                            _height, _depth);
}

void requestScreenShot(FrameBufferHandle _handle, const char *_filePath)
{
    BGFX_CHECK_API_THREAD();
    s_ctx->requestScreenShot(_handle, _filePath);
}
} // namespace bgfx