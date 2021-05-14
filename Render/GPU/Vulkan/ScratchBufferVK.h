

class ScratchBufferVK
{
  public:
    ScratchBufferVK()
    {
    }

    ~ScratchBufferVK()
    {
    }

    void create(uint32_t _size, uint32_t _count);
    void destroy();
    void reset();
    uint32_t write(const void *_data, uint32_t _size);
    void flush();

    VkBuffer m_buffer;
    VkDeviceMemory m_deviceMem;
    uint8_t *m_data;
    uint32_t m_size;
    uint32_t m_pos;
};
