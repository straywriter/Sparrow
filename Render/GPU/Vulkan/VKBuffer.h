
struct BufferVK
{
    BufferVK()
        : m_buffer(VK_NULL_HANDLE), m_deviceMem(VK_NULL_HANDLE), m_size(0), m_flags(BGFX_BUFFER_NONE), m_dynamic(false)
    {
    }

    void create(VkCommandBuffer _commandBuffer, uint32_t _size, void *_data, uint16_t _flags, bool _vertex,
                uint32_t _stride = 0);
    void update(VkCommandBuffer _commandBuffer, uint32_t _offset, uint32_t _size, void *_data, bool _discard = false);
    void destroy();

    VkBuffer m_buffer;
    VkDeviceMemory m_deviceMem;
    uint32_t m_size;
    uint16_t m_flags;
    bool m_dynamic;
};

typedef BufferVK IndexBufferVK;

struct MsaaSamplerVK
{
    uint16_t Count;
    VkSampleCountFlagBits Sample;
};