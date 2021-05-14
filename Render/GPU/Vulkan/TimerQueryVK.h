

struct TimerQueryVK
{
    TimerQueryVK() : m_control(BX_COUNTOF(m_query))
    {
    }

    VkResult init();
    void shutdown();
    uint32_t begin(uint32_t _resultIdx);
    void end(uint32_t _idx);
    bool update();

    struct Result
    {
        void reset()
        {
            m_begin = 0;
            m_end = 0;
            m_pending = 0;
        }

        uint64_t m_begin;
        uint64_t m_end;
        uint32_t m_pending;
    };

    struct Query
    {
        uint32_t m_resultIdx;
        bool m_ready;
        uint64_t m_completed;
    };

    uint64_t m_frequency;

    Result m_result[BGFX_CONFIG_MAX_VIEWS + 1];
    Query m_query[BGFX_CONFIG_MAX_VIEWS * 4];

    VkBuffer m_readback;
    VkDeviceMemory m_readbackMemory;
    VkQueryPool m_queryPool;
    const uint64_t *m_queryResult;
    bx::RingBufferControl m_control;
};
