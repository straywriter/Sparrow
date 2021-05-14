
///
struct Dxgi
{
#if BX_PLATFORM_WINDOWS
    typedef ::IDXGIAdapter3 AdapterI;
    typedef ::IDXGIFactory5 FactoryI;
    typedef ::IDXGISwapChain3 SwapChainI;
    typedef ::IDXGIOutput OutputI;
#elif BX_PLATFORM_WINRT
    typedef ::IDXGIAdapter AdapterI;
    typedef ::IDXGIFactory4 FactoryI;
    typedef ::IDXGISwapChain1 SwapChainI;
    typedef ::IDXGIOutput OutputI;
#else
    typedef ::IDXGIAdapter AdapterI;
    typedef ::IDXGIFactory2 FactoryI;
    typedef ::IDXGISwapChain1 SwapChainI;
    typedef ::IDXGIOutput OutputI;
#endif // BX_PLATFORM_WINDOWS

    ///
    Dxgi();

    ///
    bool init(Caps &_caps);

    ///
    void shutdown();

    ///
    void update(IUnknown *_device);

    ///
    HRESULT createSwapChain(IUnknown *_device, const SwapChainDesc &_scd, SwapChainI **_swapChain);

#if BX_PLATFORM_WINRT
    ///
    HRESULT removeSwapChain(const SwapChainDesc &_scd, SwapChainI **_swapChain);
#endif

    ///
    void updateHdr10(SwapChainI *_swapChain, const SwapChainDesc &_scd);

    ///
    HRESULT resizeBuffers(SwapChainI *_swapChain, const SwapChainDesc &_scd, const uint32_t *_nodeMask = NULL,
                          IUnknown *const *_presentQueue = NULL);

    ///
    void trim();

    ///
    void *m_dxgiDll;
    void *m_dxgiDebugDll;

    D3D_DRIVER_TYPE m_driverType;
    DXGI_ADAPTER_DESC m_adapterDesc;
    FactoryI *m_factory;
    AdapterI *m_adapter;
    OutputI *m_output;
};