



struct VertexBufferD3D11 : public BufferD3D11
{
    VertexBufferD3D11() : BufferD3D11()
    {
    }

    void create(uint32_t _size, void *_data, VertexLayoutHandle _layoutHandle, uint16_t _flags);

    VertexLayoutHandle m_layoutHandle;
};