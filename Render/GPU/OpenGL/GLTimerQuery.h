

struct TimerQueryGL
{
    TimerQueryGL() : m_control(BX_COUNTOF(m_query))
    {
    }

    void create()
    {
        for (uint32_t ii = 0; ii < BX_COUNTOF(m_query); ++ii)
        {
            Query &query = m_query[ii];
            query.m_ready = false;
            GL_CHECK(glGenQueries(1, &query.m_begin));
            GL_CHECK(glGenQueries(1, &query.m_end));
        }

        for (uint32_t ii = 0; ii < BX_COUNTOF(m_result); ++ii)
        {
            Result &result = m_result[ii];
            result.reset();
        }
    }

    void destroy()
    {
        for (uint32_t ii = 0; ii < BX_COUNTOF(m_query); ++ii)
        {
            Query &query = m_query[ii];
            GL_CHECK(glDeleteQueries(1, &query.m_begin));
            GL_CHECK(glDeleteQueries(1, &query.m_end));
        }
    }

    uint32_t begin(uint32_t _resultIdx)
    {
        while (0 == m_control.reserve(1))
        {
            update();
        }

        Result &result = m_result[_resultIdx];
        ++result.m_pending;

        const uint32_t idx = m_control.m_current;
        Query &query = m_query[idx];
        query.m_resultIdx = _resultIdx;
        query.m_ready = false;

        GL_CHECK(glQueryCounter(query.m_begin, GL_TIMESTAMP));

        m_control.commit(1);

        return idx;
    }

    void end(uint32_t _idx)
    {
        Query &query = m_query[_idx];
        query.m_ready = true;

        GL_CHECK(glQueryCounter(query.m_end, GL_TIMESTAMP));

        while (update())
        {
        }
    }

    bool update()
    {
        if (0 != m_control.available())
        {
            Query &query = m_query[m_control.m_read];

            if (!query.m_ready)
            {
                return false;
            }

            GLint available;
            GL_CHECK(glGetQueryObjectiv(query.m_end, GL_QUERY_RESULT_AVAILABLE, &available));

            if (available)
            {
                m_control.consume(1);

                Result &result = m_result[query.m_resultIdx];
                --result.m_pending;

                GL_CHECK(glGetQueryObjectui64v(query.m_begin, GL_QUERY_RESULT, &result.m_begin));

                GL_CHECK(glGetQueryObjectui64v(query.m_end, GL_QUERY_RESULT, &result.m_end));

                return true;
            }
        }

        return false;
    }

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
        GLuint m_begin;
        GLuint m_end;
        uint32_t m_resultIdx;
        bool m_ready;
    };

    Result m_result[BGFX_CONFIG_MAX_VIEWS + 1];

    Query m_query[BGFX_CONFIG_MAX_VIEWS * 4];
    bx::RingBufferControl m_control;
};