

struct OcclusionQueryD3D12
{
    OcclusionQueryD3D12() : m_control(BX_COUNTOF(m_handle))
    {
    }

    void init();
    void shutdown();
    void begin(ID3D12GraphicsCommandList *_commandList, Frame *_render, OcclusionQueryHandle _handle);
    void end(ID3D12GraphicsCommandList *_commandList);
    void invalidate(OcclusionQueryHandle _handle);

    ID3D12Resource *m_readback;
    ID3D12QueryHeap *m_queryHeap;
    OcclusionQueryHandle m_handle[BGFX_CONFIG_MAX_OCCLUSION_QUERIES];
    uint64_t *m_result;
    bx::RingBufferControl m_control;
};
