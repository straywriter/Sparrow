
#include "Render/IRenderContext.h"

#include "Render/RTemp.h" //windowsVersionIs

namespace bgfx
{

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

// struct RendererCreator
// {
//     RendererCreateFn createFn;
//     RendererDestroyFn destroyFn;
//     const char *name;
//     bool supported;
// };

// static RendererCreator s_rendererCreator[] = {
 RendererCreator s_rendererCreator[] = {
    {noop::rendererCreate, noop::rendererDestroy, BGFX_RENDERER_NOOP_NAME, true}, // Noop
    {d3d9::rendererCreate, d3d9::rendererDestroy, BGFX_RENDERER_DIRECT3D9_NAME,
     !!BGFX_CONFIG_RENDERER_DIRECT3D9}, // Direct3D9
    {d3d11::rendererCreate, d3d11::rendererDestroy, BGFX_RENDERER_DIRECT3D11_NAME,
     !!BGFX_CONFIG_RENDERER_DIRECT3D11}, // Direct3D11
    {d3d12::rendererCreate, d3d12::rendererDestroy, BGFX_RENDERER_DIRECT3D12_NAME,
     !!BGFX_CONFIG_RENDERER_DIRECT3D12},                                                             // Direct3D12
    {gnm::rendererCreate, gnm::rendererDestroy, BGFX_RENDERER_GNM_NAME, !!BGFX_CONFIG_RENDERER_GNM}, // GNM
#if BX_PLATFORM_OSX || BX_PLATFORM_IOS
    {mtl::rendererCreate, mtl::rendererDestroy, BGFX_RENDERER_METAL_NAME, !!BGFX_CONFIG_RENDERER_METAL}, // Metal
#else
    {noop::rendererCreate, noop::rendererDestroy, BGFX_RENDERER_NOOP_NAME, false}, // Noop
#endif // BX_PLATFORM_OSX || BX_PLATFORM_IOS
    {nvn::rendererCreate, nvn::rendererDestroy, BGFX_RENDERER_NVN_NAME, !!BGFX_CONFIG_RENDERER_NVN},       // NVN
    {gl::rendererCreate, gl::rendererDestroy, BGFX_RENDERER_OPENGL_NAME, !!BGFX_CONFIG_RENDERER_OPENGLES}, // OpenGLES
    {gl::rendererCreate, gl::rendererDestroy, BGFX_RENDERER_OPENGL_NAME, !!BGFX_CONFIG_RENDERER_OPENGL},   // OpenGL
    {vk::rendererCreate, vk::rendererDestroy, BGFX_RENDERER_VULKAN_NAME, !!BGFX_CONFIG_RENDERER_VULKAN},   // Vulkan
    {webgpu::rendererCreate, webgpu::rendererDestroy, BGFX_RENDERER_WEBGPU_NAME,
     !!BGFX_CONFIG_RENDERER_WEBGPU}, // WebGPU
};
BX_STATIC_ASSERT(BX_COUNTOF(s_rendererCreator) == RendererType::Count);

static int32_t compareDescending(const void *_lhs, const void *_rhs)
{
    return *(const int32_t *)_rhs - *(const int32_t *)_lhs;
}

RendererContextI *rendererCreate(const Init &_init)
{
    int32_t scores[RendererType::Count];
    uint32_t numScores = 0;

    for (uint32_t ii = 0; ii < RendererType::Count; ++ii)
    {
        RendererType::Enum renderer = RendererType::Enum(ii);
        if (s_rendererCreator[ii].supported)
        {
            int32_t score = 0;
            if (_init.type == renderer)
            {
                score += 1000;
            }

            score += RendererType::Noop != renderer ? 1 : 0;

            if (BX_ENABLED(BX_PLATFORM_WINDOWS))
            {
                if (windowsVersionIs(Condition::GreaterEqual, 0x0602))
                {
                    score += RendererType::Direct3D11 == renderer ? 20 : 0;
                    score += RendererType::Direct3D12 == renderer ? 10 : 0;
                }
                else if (windowsVersionIs(Condition::GreaterEqual, 0x0601))
                {
                    score += RendererType::Direct3D11 == renderer ? 20 : 0;
                    score += RendererType::Direct3D9 == renderer ? 10 : 0;
                    score += RendererType::Direct3D12 == renderer ? -100 : 0;
                }
                else
                {
                    score += RendererType::Direct3D12 == renderer ? -100 : 0;
                }
            }
            else if (BX_ENABLED(BX_PLATFORM_LINUX))
            {
                score += RendererType::OpenGL == renderer ? 20 : 0;
                score += RendererType::OpenGLES == renderer ? 10 : 0;
            }
            else if (BX_ENABLED(BX_PLATFORM_OSX))
            {
                score += RendererType::Metal == renderer ? 20 : 0;
                score += RendererType::OpenGL == renderer ? 10 : 0;
            }
            else if (BX_ENABLED(BX_PLATFORM_IOS))
            {
                score += RendererType::Metal == renderer ? 20 : 0;
                score += RendererType::OpenGLES == renderer ? 10 : 0;
            }
            else if (BX_ENABLED(0 || BX_PLATFORM_ANDROID || BX_PLATFORM_EMSCRIPTEN || BX_PLATFORM_RPI))
            {
                score += RendererType::OpenGLES == renderer ? 20 : 0;
            }
            else if (BX_ENABLED(BX_PLATFORM_PS4))
            {
                score += RendererType::Gnm == renderer ? 20 : 0;
            }
            else if (BX_ENABLED(0 || BX_PLATFORM_XBOXONE || BX_PLATFORM_WINRT))
            {
                score += RendererType::Direct3D12 == renderer ? 20 : 0;
                score += RendererType::Direct3D11 == renderer ? 10 : 0;
            }

            scores[numScores++] = (score << 8) | uint8_t(renderer);
        }
    }

    bx::quickSort(scores, numScores, sizeof(int32_t), compareDescending);

    RendererContextI *renderCtx = NULL;
    for (uint32_t ii = 0; ii < numScores; ++ii)
    {
        RendererType::Enum renderer = RendererType::Enum(scores[ii] & 0xff);
        renderCtx = s_rendererCreator[renderer].createFn(_init);
        if (NULL != renderCtx)
        {
            break;
        }

        s_rendererCreator[renderer].supported = false;
    }

    return renderCtx;
}

void rendererDestroy(RendererContextI *_renderCtx)
{
    if (NULL != _renderCtx)
    {
        s_rendererCreator[_renderCtx->getRendererType()].destroyFn();
    }
}

} // namespace bgfx