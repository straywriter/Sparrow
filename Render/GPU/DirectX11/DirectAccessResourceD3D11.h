

struct DirectAccessResourceD3D11
{
    DirectAccessResourceD3D11() : m_ptr(NULL), m_descriptor(NULL)
    {
    }

    void *createTexture2D(const D3D11_TEXTURE2D_DESC *_gpuDesc, const D3D11_SUBRESOURCE_DATA *_srd,
                          ID3D11Texture2D **_gpuTexture2d);
    void *createTexture3D(const D3D11_TEXTURE3D_DESC *_gpuDesc, const D3D11_SUBRESOURCE_DATA *_srd,
                          ID3D11Texture3D **_gpuTexture3d);
    void destroy();

    union {
        ID3D11Resource *m_ptr;
        ID3D11Texture2D *m_texture2d;
        ID3D11Texture3D *m_texture3d;
    };

    IntelDirectAccessResourceDescriptor *m_descriptor;
};
