
struct ShaderD3D12
{
    ShaderD3D12() : m_code(NULL), m_constantBuffer(NULL), m_hash(0), m_numUniforms(0), m_numPredefined(0)
    {
    }

    void create(const Memory *_mem);

    void destroy()
    {
        if (NULL != m_constantBuffer)
        {
            UniformBuffer::destroy(m_constantBuffer);
            m_constantBuffer = NULL;
        }

        m_numPredefined = 0;

        if (NULL != m_code)
        {
            release(m_code);
            m_code = NULL;
            m_hash = 0;
        }
    }

    const Memory *m_code;
    UniformBuffer *m_constantBuffer;

    PredefinedUniform m_predefined[PredefinedUniform::Count];
    uint16_t m_attrMask[Attrib::Count];

    uint32_t m_hash;
    uint16_t m_numUniforms;
    uint16_t m_size;
    uint8_t m_numPredefined;
};