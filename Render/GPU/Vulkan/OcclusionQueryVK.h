

struct OcclusionQueryVK
{
    OcclusionQueryVK() : m_control(BX_COUNTOF(m_handle))
    {
    }

    VkResult init();
    void shutdown();
    void begin(OcclusionQueryHandle _handle);
    void end();
    void flush(Frame *_render);
    void resolve(Frame *_render);
    void invalidate(OcclusionQueryHandle _handle);

    OcclusionQueryHandle m_handle[BGFX_CONFIG_MAX_OCCLUSION_QUERIES];

    VkBuffer m_readback;
    VkDeviceMemory m_readbackMemory;
    VkQueryPool m_queryPool;
    const uint32_t *m_queryResult;
    bx::RingBufferControl m_control;
};