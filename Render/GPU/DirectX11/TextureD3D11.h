

struct TextureD3D11
{
    enum Enum
    {
        Texture2D,
        Texture3D,
        TextureCube,
    };

    TextureD3D11() : m_ptr(NULL), m_rt(NULL), m_srv(NULL), m_uav(NULL), m_numMips(0)
    {
    }

    void *create(const Memory *_mem, uint64_t _flags, uint8_t _skip);
    void destroy();
    void overrideInternal(uintptr_t _ptr);
    void update(uint8_t _side, uint8_t _mip, const Rect &_rect, uint16_t _z, uint16_t _depth, uint16_t _pitch,
                const Memory *_mem);
    void commit(uint8_t _stage, uint32_t _flags, const float _palette[][4]);
    void resolve(uint8_t _resolve, uint32_t _layer, uint32_t _numLayers, uint32_t _mip) const;
    TextureHandle getHandle() const;
    DXGI_FORMAT getSrvFormat() const;

    union {
        ID3D11Resource *m_ptr;
        ID3D11Texture2D *m_texture2d;
        ID3D11Texture3D *m_texture3d;
    };

    DirectAccessResourceD3D11 m_dar;

    union {
        ID3D11Resource *m_rt;
        ID3D11Texture2D *m_rt2d;
    };

    ID3D11ShaderResourceView *m_srv;
    ID3D11UnorderedAccessView *m_uav;
    uint64_t m_flags;
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_depth;
    uint32_t m_numLayers;
    uint8_t m_type;
    uint8_t m_requestedFormat;
    uint8_t m_textureFormat;
    uint8_t m_numMips;
};
