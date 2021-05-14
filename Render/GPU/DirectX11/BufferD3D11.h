

struct BufferD3D11
{
    BufferD3D11()
        : m_ptr(NULL)
#if USE_D3D11_STAGING_BUFFER
          ,
          m_staging(NULL)
#endif // USE_D3D11_STAGING_BUFFER
          ,
          m_srv(NULL), m_uav(NULL), m_flags(BGFX_BUFFER_NONE), m_dynamic(false)
    {
    }

    void create(uint32_t _size, void *_data, uint16_t _flags, uint16_t _stride = 0, bool _vertex = false);
    void update(uint32_t _offset, uint32_t _size, void *_data, bool _discard = false);

    void destroy()
    {
        if (NULL != m_ptr)
        {
            DX_RELEASE(m_ptr, 0);
            m_dynamic = false;
        }

#if USE_D3D11_STAGING_BUFFER
        DX_RELEASE(m_staging, 0);
#endif // USE_D3D11_STAGING_BUFFER

        DX_RELEASE(m_srv, 0);
        DX_RELEASE(m_uav, 0);
    }

    ID3D11Buffer *m_ptr;
#if USE_D3D11_STAGING_BUFFER
    ID3D11Buffer *m_staging;
#endif // USE_D3D11_STAGING_BUFFER
    ID3D11ShaderResourceView *m_srv;
    ID3D11UnorderedAccessView *m_uav;
    uint32_t m_size;
    uint16_t m_flags;
    bool m_dynamic;
};

