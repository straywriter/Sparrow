
class ScratchBufferD3D12
	{
	public:
		ScratchBufferD3D12()
		{
		}

		~ScratchBufferD3D12()
		{
		}

		void create(uint32_t _size, uint32_t _maxDescriptors);
		void destroy();
		void reset(D3D12_GPU_DESCRIPTOR_HANDLE& _gpuHandle);

		void  allocEmpty(D3D12_GPU_DESCRIPTOR_HANDLE& _gpuHandle);

		void* allocCbv(D3D12_GPU_VIRTUAL_ADDRESS& _gpuAddress, uint32_t _size);

		void  allocSrv(D3D12_GPU_DESCRIPTOR_HANDLE& _gpuHandle, struct TextureD3D12& _texture, uint8_t _mip = 0);
		void  allocSrv(D3D12_GPU_DESCRIPTOR_HANDLE& _gpuHandle, struct BufferD3D12& _buffer);

		void  allocUav(D3D12_GPU_DESCRIPTOR_HANDLE& _gpuHandle, struct TextureD3D12& _texture, uint8_t _mip = 0);
		void  allocUav(D3D12_GPU_DESCRIPTOR_HANDLE& _gpuHandle, struct BufferD3D12& _buffer);

		ID3D12DescriptorHeap* getHeap()
		{
			return m_heap;
		}

	private:
		ID3D12DescriptorHeap* m_heap;
		ID3D12Resource* m_upload;
		D3D12_GPU_VIRTUAL_ADDRESS m_gpuVA;
		D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE m_gpuHandle;
		uint32_t m_incrementSize;
		uint8_t* m_data;
		uint32_t m_size;
		uint32_t m_pos;
	};