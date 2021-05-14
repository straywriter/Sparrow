
struct RendererContextD3D12 : public RendererContextI

{
    RendererContextD3D12()
        : m_d3d12Dll(NULL), m_renderDocDll(NULL), m_winPixEvent(NULL), m_featureLevel(D3D_FEATURE_LEVEL(0)),
          m_swapChain(NULL), m_wireframe(false), m_lost(false), m_maxAnisotropy(1), m_depthClamp(false),
          m_backBufferColorIdx(0), m_rtMsaa(false), m_directAccessSupport(false)
    {
    }

    ~RendererContextD3D12();

    bool init(const Init &_init);

    void shutdown();

    RendererType::Enum getRendererType() const override;

    const char *getRendererName() const override;

    bool isDeviceRemoved() override;

    void flip() override;

    void createIndexBuffer(IndexBufferHandle _handle, const Memory *_mem, uint16_t _flags) override;

    void destroyIndexBuffer(IndexBufferHandle _handle) override;

    void createVertexLayout(VertexLayoutHandle _handle, const VertexLayout &_layout) override

        void destroyVertexLayout(VertexLayoutHandle /*_handle*/) override;

    void createVertexBuffer(VertexBufferHandle _handle, const Memory *_mem, VertexLayoutHandle _layoutHandle,
                            uint16_t _flags) override;

    void destroyVertexBuffer(VertexBufferHandle _handle) override;

    void createDynamicIndexBuffer(IndexBufferHandle _handle, uint32_t _size, uint16_t _flags) override;

    void updateDynamicIndexBuffer(IndexBufferHandle _handle, uint32_t _offset, uint32_t _size,
                                  const Memory *_mem) override;

    void destroyDynamicIndexBuffer(IndexBufferHandle _handle) override;

    void createDynamicVertexBuffer(VertexBufferHandle _handle, uint32_t _size, uint16_t _flags) override;

    void updateDynamicVertexBuffer(VertexBufferHandle _handle, uint32_t _offset, uint32_t _size,
                                   const Memory *_mem) override;

    void destroyDynamicVertexBuffer(VertexBufferHandle _handle) override;

    void createShader(ShaderHandle _handle, const Memory *_mem) override;

    void destroyShader(ShaderHandle _handle) override;

    void createProgram(ProgramHandle _handle, ShaderHandle _vsh, ShaderHandle _fsh) override;

    void destroyProgram(ProgramHandle _handle) override;

    void *createTexture(TextureHandle _handle, const Memory *_mem, uint64_t _flags, uint8_t _skip) override;

    void updateTextureBegin(TextureHandle /*_handle*/, uint8_t /*_side*/, uint8_t /*_mip*/) override;

    void updateTexture(TextureHandle _handle, uint8_t _side, uint8_t _mip, const Rect &_rect, uint16_t _z,
                       uint16_t _depth, uint16_t _pitch, const Memory *_mem) override;

    void updateTextureEnd() override;

    void readTexture(TextureHandle _handle, void *_data, uint8_t _mip) override;

    void resizeTexture(TextureHandle _handle, uint16_t _width, uint16_t _height, uint8_t _numMips,
                       uint16_t _numLayers) override;

    void overrideInternal(TextureHandle _handle, uintptr_t _ptr) override;

    uintptr_t getInternal(TextureHandle _handle) override;

    void destroyTexture(TextureHandle _handle) override;

    void createFrameBuffer(FrameBufferHandle _handle, uint8_t _num, const Attachment *_attachment) override;

    void createFrameBuffer(FrameBufferHandle _handle, void *_nwh, uint32_t _width, uint32_t _height,
                           TextureFormat::Enum _format, TextureFormat::Enum _depthFormat) override;

    void destroyFrameBuffer(FrameBufferHandle _handle) override;

    void createUniform(UniformHandle _handle, UniformType::Enum _type, uint16_t _num, const char *_name) override;

    void destroyUniform(UniformHandle _handle) override;

    void requestScreenShot(FrameBufferHandle _handle, const char *_filePath) override;

    void updateViewName(ViewId _id, const char *_name) override;

    void updateUniform(uint16_t _loc, const void *_data, uint32_t _size) override;

    void invalidateOcclusionQuery(OcclusionQueryHandle _handle) override;

    void setMarker(const char *_marker, uint16_t _len) override;

    virtual void setName(Handle _handle, const char *_name, uint16_t _len) override;
    ;

    void submitBlit(BlitState &_bs, uint16_t _view);

    void submit(Frame *_render, ClearQuad &_clearQuad, TextVideoMemBlitter &_textVideoMemBlitter) override;

    void blitSetup(TextVideoMemBlitter &_blitter) override;

    void blitRender(TextVideoMemBlitter &_blitter, uint32_t _numIndices) override;

    void preReset();

    void postReset();

    void invalidateCache();

    void updateMsaa(DXGI_FORMAT _format) const;

    IUnknown *getDeviceForSwapChain() const;

    bool updateResolution(const Resolution &_resolution);

    void setShaderUniform(uint8_t _flags, uint32_t _regIndex, const void *_val, uint32_t _numRegs);

    void setShaderUniform4f(uint8_t _flags, uint32_t _regIndex, const void *_val, uint32_t _numRegs);

    void setShaderUniform4x4f(uint8_t _flags, uint32_t _regIndex, const void *_val, uint32_t _numRegs);

    void commitShaderConstants(ProgramHandle _program, D3D12_GPU_VIRTUAL_ADDRESS &_gpuAddress);

    D3D12_CPU_DESCRIPTOR_HANDLE getRtv(FrameBufferHandle _fbh);

    D3D12_CPU_DESCRIPTOR_HANDLE getRtv(FrameBufferHandle _fbh, uint8_t _attachment);

    D3D12_CPU_DESCRIPTOR_HANDLE getDsv(FrameBufferHandle _fbh) const;

    void setFrameBuffer(FrameBufferHandle _fbh, bool _msaa = true);

    void setBlendState(D3D12_BLEND_DESC &_desc, uint64_t _state, uint32_t _rgba = 0);

    void setRasterizerState(D3D12_RASTERIZER_DESC &_desc, uint64_t _state, bool _wireframe = false);

    void setDepthStencilState(D3D12_DEPTH_STENCIL_DESC &_desc, uint64_t _state, uint64_t _stencil = 0);

    uint32_t setInputLayout(D3D12_INPUT_ELEMENT_DESC *_vertexElements, uint8_t _numStreams,
                            const VertexLayout **_layouts, const ProgramD3D12 &_program, uint16_t _numInstanceData);

    uint32_t setInputLayout(D3D12_INPUT_ELEMENT_DESC *_vertexElements, const VertexLayout &_layout,
                            const ProgramD3D12 &_program, uint16_t _numInstanceData);

    static void patchCb0(DxbcInstruction &_instruction, void *_userData);

    ID3D12PipelineState *getPipelineState(ProgramHandle _program);

    ID3D12PipelineState *getPipelineState(uint64_t _state, uint64_t _stencil, uint8_t _numStreams,
                                          const VertexLayout **_layouts, ProgramHandle _program,
                                          uint8_t _numInstanceData);

    uint16_t getSamplerState(const uint32_t *_flags, uint32_t _num, const float _palette[][4]);

    bool isVisible(Frame *_render, OcclusionQueryHandle _handle, bool _visible);

    void commit(UniformBuffer &_uniformBuffer);

    void clear(const Clear &_clear, const float _palette[][4], const D3D12_RECT *_rect = NULL, uint32_t _num = 0);

    void clearQuad(const Rect &_rect, const Clear &_clear, const float _palette[][4]);

    uint64_t kick();

    void finish();

    void finishAll(bool _alloc = false);

    Dxgi m_dxgi;
    NvApi m_nvapi;

    void *m_kernel32Dll;
    void *m_d3d12Dll;
    void *m_renderDocDll;
    void *m_winPixEvent;

    D3D_FEATURE_LEVEL m_featureLevel;

    D3D_DRIVER_TYPE m_driverType;
    D3D12_FEATURE_DATA_ARCHITECTURE m_architecture;
    D3D12_FEATURE_DATA_D3D12_OPTIONS m_options;

    Dxgi::SwapChainI *m_swapChain;
    ID3D12Resource *m_msaaRt;

#if BX_PLATFORM_WINDOWS
    ID3D12InfoQueue *m_infoQueue;
#endif // BX_PLATFORM_WINDOWS

    int64_t m_presentElapsed;
    uint16_t m_numWindows;
    FrameBufferHandle m_windows[BGFX_CONFIG_MAX_FRAME_BUFFERS];

    ID3D12Device *m_device;
    TimerQueryD3D12 m_gpuTimer;
    OcclusionQueryD3D12 m_occlusionQuery;

    uint32_t m_deviceInterfaceVersion;

    ID3D12DescriptorHeap *m_rtvDescriptorHeap;
    ID3D12DescriptorHeap *m_dsvDescriptorHeap;
    D3D12_CPU_DESCRIPTOR_HANDLE m_rtvHandle;
    D3D12_CPU_DESCRIPTOR_HANDLE m_dsvHandle;
    D3D12_CPU_DESCRIPTOR_HANDLE *m_currentColor;
    D3D12_CPU_DESCRIPTOR_HANDLE *m_currentDepthStencil;
    ID3D12Resource *m_backBufferColor[BGFX_CONFIG_MAX_BACK_BUFFERS];
    uint64_t m_backBufferColorFence[BGFX_CONFIG_MAX_BACK_BUFFERS];
    ID3D12Resource *m_backBufferDepthStencil;

    ScratchBufferD3D12 m_scratchBuffer[BGFX_CONFIG_MAX_BACK_BUFFERS];
    DescriptorAllocatorD3D12 m_samplerAllocator;

    ID3D12RootSignature *m_rootSignature;
    ID3D12CommandSignature *m_commandSignature[3];

    CommandQueueD3D12 m_cmd;
    BatchD3D12 m_batch;
    ID3D12GraphicsCommandList *m_commandList;

    Resolution m_resolution;
    bool m_wireframe;
    bool m_lost;

    SwapChainDesc m_scd;
    uint32_t m_maxAnisotropy;
    bool m_depthClamp;

    BufferD3D12 m_indexBuffers[BGFX_CONFIG_MAX_INDEX_BUFFERS];
    VertexBufferD3D12 m_vertexBuffers[BGFX_CONFIG_MAX_VERTEX_BUFFERS];
    ShaderD3D12 m_shaders[BGFX_CONFIG_MAX_SHADERS];
    ProgramD3D12 m_program[BGFX_CONFIG_MAX_PROGRAMS];
    TextureD3D12 m_textures[BGFX_CONFIG_MAX_TEXTURES];
    VertexLayout m_vertexLayouts[BGFX_CONFIG_MAX_VERTEX_LAYOUTS];
    FrameBufferD3D12 m_frameBuffers[BGFX_CONFIG_MAX_FRAME_BUFFERS];
    void *m_uniforms[BGFX_CONFIG_MAX_UNIFORMS];
    Matrix4 m_predefinedUniforms[PredefinedUniform::Count];
    UniformRegistry m_uniformReg;

    StateCacheT<ID3D12PipelineState> m_pipelineStateCache;
    StateCache m_samplerStateCache;

    TextVideoMem m_textVideoMem;

    uint8_t m_fsScratch[64 << 10];
    uint8_t m_vsScratch[64 << 10];

    FrameBufferHandle m_fbh;
    uint32_t m_backBufferColorIdx;
    bool m_rtMsaa;
    bool m_directAccessSupport;
};