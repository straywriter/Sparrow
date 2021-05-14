

struct CommandQueueD3D12
{
    CommandQueueD3D12() : m_currentFence(0), m_completedFence(0), m_control(BX_COUNTOF(m_commandList))
    {
        BX_STATIC_ASSERT(BX_COUNTOF(m_commandList) == BX_COUNTOF(m_release));
    }

    void init(ID3D12Device *_device);
    void shutdown();
    ID3D12GraphicsCommandList *alloc();
    uint64_t kick();
    void finish(uint64_t _waitFence = UINT64_MAX, bool _finishAll = false);
    bool tryFinish(uint64_t _waitFence);
    void release(ID3D12Resource *_ptr);
    bool consume(uint32_t _ms = INFINITE);

    struct CommandList
    {
        ID3D12GraphicsCommandList *m_commandList;
        ID3D12CommandAllocator *m_commandAllocator;
        HANDLE m_event;
    };

    ID3D12CommandQueue *m_commandQueue;
    uint64_t m_currentFence;
    uint64_t m_completedFence;
    ID3D12Fence *m_fence;
    CommandList m_commandList[256];
    typedef stl::vector<ID3D12Resource *> ResourceArray;
    ResourceArray m_release[256];
    bx::RingBufferControl m_control;
};
