

	struct FrameBufferD3D11
	{
		FrameBufferD3D11()
			: m_dsv(NULL)
			, m_swapChain(NULL)
			, m_nwh(NULL)
			, m_width(0)
			, m_height(0)
			, m_denseIdx(UINT16_MAX)
			, m_num(0)
			, m_numTh(0)
			, m_numUav(0)
			, m_needPresent(false)
		{
		}

		void create(uint8_t _num, const Attachment* _attachment);
		void create(uint16_t _denseIdx, void* _nwh, uint32_t _width, uint32_t _height, TextureFormat::Enum _format, TextureFormat::Enum _depthFormat);
		uint16_t destroy();
		void preReset(bool _force = false);
		void postReset();
		void resolve();
		void clear(const Clear& _clear, const float _palette[][4]);
		void set();
		HRESULT present(uint32_t _syncInterval);

		ID3D11RenderTargetView*    m_rtv[BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS-1];
		ID3D11UnorderedAccessView* m_uav[BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS-1];
		ID3D11ShaderResourceView*  m_srv[BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS-1];
		ID3D11DepthStencilView*    m_dsv;
		Dxgi::SwapChainI* m_swapChain;
		void* m_nwh;
		uint32_t m_width;
		uint32_t m_height;

		Attachment m_attachment[BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS];
		uint16_t m_denseIdx;
		uint8_t m_num;
		uint8_t m_numTh;
		uint8_t m_numUav;
		bool m_needPresent;
	};