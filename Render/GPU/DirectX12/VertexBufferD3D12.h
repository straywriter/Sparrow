
struct VertexBufferD3D12 : public BufferD3D12
{
    void create(uint32_t _size, void *_data, VertexLayoutHandle _layoutHandle, uint16_t _flags);

    VertexLayoutHandle m_layoutHandle;
};
