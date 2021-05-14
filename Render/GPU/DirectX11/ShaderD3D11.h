

struct ShaderD3D11
{
    ShaderD3D11()
        : m_ptr(NULL), m_code(NULL), m_buffer(NULL), m_constantBuffer(NULL), m_hash(0), m_numUniforms(0),
          m_numPredefined(0), m_hasDepthOp(false)
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

        if (NULL != m_buffer)
        {
            DX_RELEASE(m_buffer, 0);
        }

        DX_RELEASE(m_ptr, 0);

        if (NULL != m_code)
        {
            release(m_code);
            m_code = NULL;
            m_hash = 0;
        }
    }

    union {
        ID3D11ComputeShader *m_computeShader;
        ID3D11PixelShader *m_pixelShader;
        ID3D11VertexShader *m_vertexShader;
        ID3D11DeviceChild *m_ptr;
    };
    const Memory *m_code;
    ID3D11Buffer *m_buffer;
    UniformBuffer *m_constantBuffer;

    PredefinedUniform m_predefined[PredefinedUniform::Count];
    uint16_t m_attrMask[Attrib::Count];

    uint32_t m_hash;

    uint16_t m_numUniforms;
    uint8_t m_numPredefined;
    bool m_hasDepthOp;
};