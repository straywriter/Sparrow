

struct FrameBufferVK
{
    FrameBufferVK()
        : m_depth({kInvalidHandle}), m_width(0), m_height(0), m_denseIdx(kInvalidHandle), m_num(0), m_numTh(0),
          m_nwh(NULL), m_needPresent(false), m_framebuffer(VK_NULL_HANDLE)
    {
    }

    void create(uint8_t _num, const Attachment *_attachment);
    VkResult create(uint16_t _denseIdx, void *_nwh, uint32_t _width, uint32_t _height,
                    TextureFormat::Enum _format = TextureFormat::Count,
                    TextureFormat::Enum _depthFormat = TextureFormat::Count);
    uint16_t destroy();

    void update(VkCommandBuffer _commandBuffer, const Resolution &_resolution);

    void preReset();
    void postReset();

    void resolve();

    bool acquire(VkCommandBuffer _commandBuffer);
    void present();

    bool isRenderable() const;

    TextureHandle m_texture[BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS];
    TextureHandle m_depth;
    uint32_t m_width;
    uint32_t m_height;
    uint16_t m_denseIdx;
    uint8_t m_num;
    uint8_t m_numTh;
    Attachment m_attachment[BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS];

    SwapChainVK m_swapChain;
    void *m_nwh;
    bool m_needPresent;
    bool m_needResolve;

    VkImageView m_textureImageViews[BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS];
    VkFramebuffer m_framebuffer;
    VkRenderPass m_renderPass;
    MsaaSamplerVK m_sampler;

    VkFramebuffer m_currentFramebuffer;
};
