#include "Render/Platform.h"

namespace bgfx
{
void setPlatformData(const PlatformData &_data)
{
    if (NULL != s_ctx)
    {
        BGFX_FATAL(true && g_platformData.ndt == _data.ndt && g_platformData.context == _data.context,
                   Fatal::UnableToInitialize,
                   "Only backbuffer pointer and native window handle can be changed after initialization!");
    }
    bx::memCopy(&g_platformData, &_data, sizeof(PlatformData));
    g_platformDataChangedSinceReset = true;
}

const InternalData *getInternalData()
{
    return &g_internalData;
}


	uintptr_t overrideInternal(TextureHandle _handle, uintptr_t _ptr)
	{
		BGFX_CHECK_RENDER_THREAD();
		RendererContextI* rci = s_ctx->m_renderCtx;
		if (0 == rci->getInternal(_handle) )
		{
			return 0;
		}

		rci->overrideInternal(_handle, _ptr);

		return rci->getInternal(_handle);
	}

	uintptr_t overrideInternal(TextureHandle _handle, uint16_t _width, uint16_t _height, uint8_t _numMips, TextureFormat::Enum _format, uint64_t _flags)
	{
		BGFX_CHECK_RENDER_THREAD();
		RendererContextI* rci = s_ctx->m_renderCtx;
		if (0 == rci->getInternal(_handle) )
		{
			return 0;
		}

		uint32_t size = sizeof(uint32_t) + sizeof(TextureCreate);
		Memory* mem = const_cast<Memory*>(alloc(size) );

		bx::StaticMemoryBlockWriter writer(mem->data, mem->size);
		uint32_t magic = BGFX_CHUNK_MAGIC_TEX;
		bx::write(&writer, magic);

		TextureCreate tc;
		tc.m_width     = _width;
		tc.m_height    = _height;
		tc.m_depth     = 0;
		tc.m_numLayers = 1;
		tc.m_numMips   = bx::max<uint8_t>(1, _numMips);
		tc.m_format    = _format;
		tc.m_cubeMap   = false;
		tc.m_mem       = NULL;
		bx::write(&writer, tc);

		rci->destroyTexture(_handle);
		rci->createTexture(_handle, mem, _flags, 0);

		release(mem);

		return rci->getInternal(_handle);
	}

	RenderFrame::Enum renderFrame(int32_t _msecs)
	{
		if (BX_ENABLED(BGFX_CONFIG_MULTITHREADED) )
		{
			if (s_renderFrameCalled)
			{
				BGFX_CHECK_RENDER_THREAD();
			}

			if (NULL == s_ctx)
			{
				s_renderFrameCalled = true;
				s_threadIndex = ~BGFX_API_THREAD_MAGIC;
				return RenderFrame::NoContext;
			}

			int32_t msecs = -1 == _msecs
				? BGFX_CONFIG_API_SEMAPHORE_TIMEOUT
				: _msecs
				;
			RenderFrame::Enum result = s_ctx->renderFrame(msecs);
			if (RenderFrame::Exiting == result)
			{
				Context* ctx = s_ctx;
				ctx->apiSemWait();
				s_ctx = NULL;
				ctx->renderSemPost();
			}

			return result;
		}

		BX_ASSERT(false, "This call only makes sense if used with multi-threaded renderer.");
		return RenderFrame::NoContext;
	}
} // namespace bgfx