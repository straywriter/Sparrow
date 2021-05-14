
class DescriptorAllocatorD3D12
{
  public:
    DescriptorAllocatorD3D12() : m_numDescriptorsPerBlock(1)
    {
    }

    ~DescriptorAllocatorD3D12()
    {
    }

    void create(D3D12_DESCRIPTOR_HEAP_TYPE _type, uint16_t _maxDescriptors, uint16_t _numDescriptorsPerBlock = 1);
    void destroy();

    uint16_t alloc(ID3D12Resource *_ptr, const D3D12_SHADER_RESOURCE_VIEW_DESC *_desc);
    uint16_t alloc(const uint32_t *_flags, uint32_t _num, const float _palette[][4]);
    void free(uint16_t _handle);
    void reset();

    D3D12_GPU_DESCRIPTOR_HANDLE get(uint16_t _handle);

    ID3D12DescriptorHeap *getHeap()
    {
        return m_heap;
    }

  private:
    ID3D12DescriptorHeap *m_heap;
    bx::HandleAlloc *m_handleAlloc;
    D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandle;
    D3D12_GPU_DESCRIPTOR_HANDLE m_gpuHandle;
    uint32_t m_incrementSize;
    uint16_t m_numDescriptorsPerBlock;
};
