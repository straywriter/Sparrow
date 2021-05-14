
struct BufferD3D12
	{
		BufferD3D12()
			: m_ptr(NULL)
			, m_state(D3D12_RESOURCE_STATE_COMMON)
			, m_size(0)
			, m_flags(BGFX_BUFFER_NONE)
			, m_dynamic(false)
		{
		}

		void create(uint32_t _size, void* _data, uint16_t _flags, bool _vertex, uint32_t _stride = 0);
		void update(ID3D12GraphicsCommandList* _commandList, uint32_t _offset, uint32_t _size, void* _data, bool _discard = false);
		void destroy();

		D3D12_RESOURCE_STATES setState(ID3D12GraphicsCommandList* _commandList, D3D12_RESOURCE_STATES _state);

		D3D12_SHADER_RESOURCE_VIEW_DESC  m_srvd;
		D3D12_UNORDERED_ACCESS_VIEW_DESC m_uavd;
		ID3D12Resource* m_ptr;
		D3D12_GPU_VIRTUAL_ADDRESS m_gpuVA;
		D3D12_RESOURCE_STATES m_state;
		uint32_t m_size;
		uint16_t m_flags;
		bool m_dynamic;
	};
