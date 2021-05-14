

struct TextureVK
{
    TextureVK()
        : m_directAccessPtr(NULL), m_sampler({1, VK_SAMPLE_COUNT_1_BIT}), m_format(VK_FORMAT_UNDEFINED),
          m_textureImage(VK_NULL_HANDLE), m_textureDeviceMem(VK_NULL_HANDLE),
          m_currentImageLayout(VK_IMAGE_LAYOUT_UNDEFINED), m_singleMsaaImage(VK_NULL_HANDLE),
          m_singleMsaaDeviceMem(VK_NULL_HANDLE), m_currentSingleMsaaImageLayout(VK_IMAGE_LAYOUT_UNDEFINED)
    {
    }

    void *create(VkCommandBuffer _commandBuffer, const Memory *_mem, uint64_t _flags, uint8_t _skip);
    // internal render target
    VkResult create(VkCommandBuffer _commandBuffer, uint32_t _width, uint32_t _height, uint64_t _flags,
                    VkFormat _format);

    void destroy();

    void update(VkCommandBuffer _commandBuffer, uint8_t _side, uint8_t _mip, const Rect &_rect, uint16_t _z,
                uint16_t _depth, uint16_t _pitch, const Memory *_mem);
    void resolve(VkCommandBuffer _commandBuffer, uint8_t _resolve, uint32_t _layer, uint32_t _numLayers, uint32_t _mip);

    void copyBufferToTexture(VkCommandBuffer _commandBuffer, VkBuffer _stagingBuffer, uint32_t _bufferImageCopyCount,
                             VkBufferImageCopy *_bufferImageCopy);
    VkImageLayout setImageMemoryBarrier(VkCommandBuffer _commandBuffer, VkImageLayout _newImageLayout,
                                        bool _singleMsaaImage = false);

    VkResult createView(uint32_t _layer, uint32_t _numLayers, uint32_t _mip, uint32_t _numMips, VkImageViewType _type,
                        VkImageAspectFlags _aspectMask, bool _renderTarget, ::VkImageView *_view) const;

    void *m_directAccessPtr;
    uint64_t m_flags;
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_depth;
    uint32_t m_numLayers;
    uint32_t m_numSides;
    uint8_t m_requestedFormat;
    uint8_t m_textureFormat;
    uint8_t m_numMips;

    MsaaSamplerVK m_sampler;

    VkImageViewType m_type;
    VkFormat m_format;
    VkComponentMapping m_components;
    VkImageAspectFlags m_aspectMask;

    VkImage m_textureImage;
    VkDeviceMemory m_textureDeviceMem;
    VkImageLayout m_currentImageLayout;

    VkImage m_singleMsaaImage;
    VkDeviceMemory m_singleMsaaDeviceMem;
    VkImageLayout m_currentSingleMsaaImageLayout;

    VkImageLayout m_sampledLayout;

    ReadbackVK m_readback;

  private:
    VkResult createImages(VkCommandBuffer _commandBuffer);
    static VkImageAspectFlags getAspectMask(VkFormat _format);
};