
struct FrameBufferD3D12
	{
		FrameBufferD3D12()
			: m_swapChain(NULL)
			, m_nwh(NULL)
			, m_width(0)
			, m_height(0)
			, m_denseIdx(UINT16_MAX)
			, m_num(0)
			, m_numTh(0)
			, m_state(D3D12_RESOURCE_STATE_PRESENT)
			, m_needPresent(false)
		{
			m_depth.idx = bgfx::kInvalidHandle;
		}

		void create(uint8_t _num, const Attachment* _attachment);
		void create(uint16_t _denseIdx, void* _nwh, uint32_t _width, uint32_t _height, TextureFormat::Enum _format, TextureFormat::Enum _depthFormat);
		uint16_t destroy();
		HRESULT present(uint32_t _syncInterval, uint32_t _flags);
		void preReset();
		void postReset();
		void resolve();
		void clear(ID3D12GraphicsCommandList* _commandList, const Clear& _clear, const float _palette[][4], const D3D12_RECT* _rect = NULL, uint32_t _num = 0);
		D3D12_RESOURCE_STATES setState(ID3D12GraphicsCommandList* _commandList, uint8_t _idx, D3D12_RESOURCE_STATES _state);

		TextureHandle m_texture[BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS];
		TextureHandle m_depth;
		Dxgi::SwapChainI* m_swapChain;
		void* m_nwh;
		uint32_t m_width;
		uint32_t m_height;
		uint16_t m_denseIdx;
		uint8_t m_num;
		uint8_t m_numTh;
		Attachment m_attachment[BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS];
		D3D12_RESOURCE_STATES m_state;
		bool m_needPresent;
	};