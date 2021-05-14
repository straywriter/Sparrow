
/// Instance data buffer info.
///
/// @attention C99 equivalent is `bgfx_texture_info_t`.
///
struct InstanceDataBuffer
{
    uint8_t *data;             //!< Pointer to data.
    uint32_t size;             //!< Data size.
    uint32_t offset;           //!< Offset in vertex buffer.
    uint32_t num;              //!< Number of instances.
    uint16_t stride;           //!< Vertex buffer stride.
    VertexBufferHandle handle; //!< Vertex buffer object handle.
};
