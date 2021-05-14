

struct ReadbackVK
{
    void create(VkImage _image, uint32_t _width, uint32_t _height, TextureFormat::Enum _format);
    void destroy();
    uint32_t pitch(uint8_t _mip = 0) const;
    void copyImageToBuffer(VkCommandBuffer _commandBuffer, VkBuffer _buffer, VkImageLayout _layout,
                           VkImageAspectFlags _aspect, uint8_t _mip = 0) const;
    void readback(VkDeviceMemory _memory, VkDeviceSize _offset, void *_data, uint8_t _mip = 0) const;

    VkImage m_image;
    uint32_t m_width;
    uint32_t m_height;
    TextureFormat::Enum m_format;
};