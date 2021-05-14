

struct SwapChainVK
{
    SwapChainVK()
        : m_nwh(NULL), m_swapchain(VK_NULL_HANDLE), m_lastImageRenderedSemaphore(VK_NULL_HANDLE),
          m_lastImageAcquiredSemaphore(VK_NULL_HANDLE), m_backBufferColorMsaaImageView(VK_NULL_HANDLE)
    {
    }

    VkResult create(VkCommandBuffer _commandBuffer, void *_nwh, const Resolution &_resolution,
                    TextureFormat::Enum _depthFormat = TextureFormat::Count);

    void destroy();

    void update(VkCommandBuffer _commandBuffer, void *_nwh, const Resolution &_resolution);

    VkResult createSurface();
    VkResult createSwapChain();
    VkResult createAttachments(VkCommandBuffer _commandBuffer);
    VkResult createFrameBuffer();

    void releaseSurface();
    void releaseSwapChain();
    void releaseAttachments();
    void releaseFrameBuffer();

    uint32_t findPresentMode(bool _vsync);
    TextureFormat::Enum findSurfaceFormat(TextureFormat::Enum _format, VkColorSpaceKHR _colorSpace, bool _srgb);

    bool acquire(VkCommandBuffer _commandBuffer);
    void present();

    void transitionImage(VkCommandBuffer _commandBuffer);

    VkQueue m_queue;
    VkSwapchainCreateInfoKHR m_sci;

    void *m_nwh;
    Resolution m_resolution;

    TextureFormat::Enum m_colorFormat;
    TextureFormat::Enum m_depthFormat;

    VkSurfaceKHR m_surface;
    VkSwapchainKHR m_swapchain;
    uint32_t m_numSwapchainImages;
    VkImageLayout m_backBufferColorImageLayout[BGFX_CONFIG_MAX_BACK_BUFFERS];
    VkImage m_backBufferColorImage[BGFX_CONFIG_MAX_BACK_BUFFERS];
    VkImageView m_backBufferColorImageView[BGFX_CONFIG_MAX_BACK_BUFFERS];
    VkFramebuffer m_backBufferFrameBuffer[BGFX_CONFIG_MAX_BACK_BUFFERS];
    VkFence m_backBufferFence[BGFX_CONFIG_MAX_BACK_BUFFERS];
    uint32_t m_backBufferColorIdx;

    VkSemaphore m_presentDoneSemaphore[BGFX_CONFIG_MAX_BACK_BUFFERS];
    VkSemaphore m_renderDoneSemaphore[BGFX_CONFIG_MAX_BACK_BUFFERS];
    uint32_t m_currentSemaphore;

    VkSemaphore m_lastImageRenderedSemaphore;
    VkSemaphore m_lastImageAcquiredSemaphore;

    bool m_needPresent;
    bool m_needToRefreshSwapchain;
    bool m_needToRecreateSurface;

    TextureVK m_backBufferDepthStencil;
    VkImageView m_backBufferDepthStencilImageView;

    TextureVK m_backBufferColorMsaa;
    VkImageView m_backBufferColorMsaaImageView;
    MsaaSamplerVK m_sampler;

    bool m_supportsReadback;
    bool m_supportsManualResolve;
};
