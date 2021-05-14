

struct TextureD3D12
{
    enum Enum
    {
        Texture2D,
        Texture3D,
        TextureCube,
    };

    TextureD3D12()
        : m_ptr(NULL), m_singleMsaa(NULL), m_directAccessPtr(NULL), m_state(D3D12_RESOURCE_STATE_COMMON), m_numMips(0)
    {
        bx::memSet(&m_srvd, 0, sizeof(m_srvd));
        bx::memSet(&m_uavd, 0, sizeof(m_uavd));
    }

    void *create(const Memory *_mem, uint64_t _flags, uint8_t _skip);
    void destroy();
    void overrideInternal(uintptr_t _ptr);
    void update(ID3D12GraphicsCommandList *_commandList, uint8_t _side, uint8_t _mip, const Rect &_rect, uint16_t _z,
                uint16_t _depth, uint16_t _pitch, const Memory *_mem);
    void resolve(ID3D12GraphicsCommandList *_commandList, uint8_t _resolve, uint32_t _layer, uint32_t _numLayers,
                 uint32_t _mip);
    D3D12_RESOURCE_STATES setState(ID3D12GraphicsCommandList *_commandList, D3D12_RESOURCE_STATES _state);

    D3D12_SHADER_RESOURCE_VIEW_DESC m_srvd;
    D3D12_UNORDERED_ACCESS_VIEW_DESC m_uavd;
    ID3D12Resource *m_ptr;
    ID3D12Resource *m_singleMsaa;
    void *m_directAccessPtr;
    D3D12_RESOURCE_STATES m_state;
    uint64_t m_flags;
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_depth;
    uint32_t m_numLayers;
    uint16_t m_samplerIdx;
    uint8_t m_type;
    uint8_t m_requestedFormat;
    uint8_t m_textureFormat;
    uint8_t m_numMips;
};
