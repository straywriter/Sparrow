

struct OcclusionQueryGL
{
    OcclusionQueryGL() : m_control(BX_COUNTOF(m_query))
    {
    }

    void create();
    void destroy();
    void begin(Frame *_render, OcclusionQueryHandle _handle);
    void end();
    void resolve(Frame *_render, bool _wait = false);
    void invalidate(OcclusionQueryHandle _handle);

    struct Query
    {
        GLuint m_id;
        OcclusionQueryHandle m_handle;
    };

    Query m_query[BGFX_CONFIG_MAX_OCCLUSION_QUERIES];
    bx::RingBufferControl m_control;
};