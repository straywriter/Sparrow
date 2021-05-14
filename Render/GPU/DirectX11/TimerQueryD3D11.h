

struct TimerQueryD3D11
{
    TimerQueryD3D11() : m_control(BX_COUNTOF(m_query))
    {
    }

    void postReset();
    void preReset();
    uint32_t begin(uint32_t _resultIdx);
    void end(uint32_t _idx);
    bool update();

    struct Query
    {
        ID3D11Query *m_disjoint;
        ID3D11Query *m_begin;
        ID3D11Query *m_end;
        uint32_t m_resultIdx;
        bool m_ready;
    };

    struct Result
    {
        void reset()
        {
            m_begin = 0;
            m_end = 0;
            m_frequency = 1;
            m_pending = 0;
        }

        uint64_t m_begin;
        uint64_t m_end;
        uint64_t m_frequency;
        uint32_t m_pending;
    };

    Result m_result[BGFX_CONFIG_MAX_VIEWS + 1];

    Query m_query[BGFX_CONFIG_MAX_VIEWS * 4];
    bx::RingBufferControl m_control;
};