

struct OcclusionQueryD3D11
{
    OcclusionQueryD3D11() : m_control(BX_COUNTOF(m_query))
    {
    }

    void postReset();
    void preReset();
    void begin(Frame *_render, OcclusionQueryHandle _handle);
    void end();
    void resolve(Frame *_render, bool _wait = false);
    void invalidate(OcclusionQueryHandle _handle);

    struct Query
    {
        ID3D11Query *m_ptr;
        OcclusionQueryHandle m_handle;
    };

    Query m_query[BGFX_CONFIG_MAX_OCCLUSION_QUERIES];
    bx::RingBufferControl m_control;
};