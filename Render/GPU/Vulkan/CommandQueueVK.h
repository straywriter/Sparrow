

struct CommandQueueVK
{
    VkResult init(uint32_t _queueFamily, VkQueue _queue, uint32_t _numFramesInFlight);
    VkResult reset();
    void shutdown();

    VkResult alloc(VkCommandBuffer *_commandBuffer);
    void addWaitSemaphore(VkSemaphore _semaphore, VkPipelineStageFlags _waitFlags = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
    void addSignalSemaphore(VkSemaphore _semaphore);
    void kick(bool _wait = false);
    void finish(bool _finishAll = false);

    void release(uint64_t _handle, VkObjectType _type);
    void consume();

    uint32_t m_queueFamily;
    VkQueue m_queue;

    uint32_t m_numFramesInFlight;

    uint32_t m_currentFrameInFlight;
    uint32_t m_consumeIndex;

    VkCommandBuffer m_activeCommandBuffer;

    VkFence m_currentFence;
    VkFence m_completedFence;

    uint64_t m_submitted;

    struct CommandList
    {
        VkCommandPool m_commandPool = VK_NULL_HANDLE;
        VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;
        VkFence m_fence = VK_NULL_HANDLE;
    };

    CommandList m_commandList[BGFX_CONFIG_MAX_FRAME_LATENCY];

    uint32_t m_numWaitSemaphores;
    VkSemaphore m_waitSemaphores[BGFX_CONFIG_MAX_FRAME_BUFFERS];
    VkPipelineStageFlags m_waitSemaphoreStages[BGFX_CONFIG_MAX_FRAME_BUFFERS];
    uint32_t m_numSignalSemaphores;
    VkSemaphore m_signalSemaphores[BGFX_CONFIG_MAX_FRAME_BUFFERS];

    struct Resource
    {
        VkObjectType m_type;
        uint64_t m_handle;
    };

    typedef stl::vector<Resource> ResourceArray;
    ResourceArray m_release[BGFX_CONFIG_MAX_FRAME_LATENCY];

  private:
    template <typename Ty> void destroy(uint64_t _handle)
    {
        typedef decltype(Ty::vk) vk_t;
        Ty obj = vk_t(_handle);
        vkDestroy(obj);
    }
};