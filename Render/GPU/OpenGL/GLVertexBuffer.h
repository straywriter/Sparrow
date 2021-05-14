

struct VertexBufferGL
{
    void create(uint32_t _size, void *_data, VertexLayoutHandle _layoutHandle, uint16_t _flags)
    {
        m_size = _size;
        m_layoutHandle = _layoutHandle;
        const bool drawIndirect = 0 != (_flags & BGFX_BUFFER_DRAW_INDIRECT);

        m_target = drawIndirect ? GL_DRAW_INDIRECT_BUFFER : GL_ARRAY_BUFFER;

        GL_CHECK(glGenBuffers(1, &m_id));
        BX_ASSERT(0 != m_id, "Failed to generate buffer id.");
        GL_CHECK(glBindBuffer(m_target, m_id));
        GL_CHECK(glBufferData(m_target, _size, _data, (NULL == _data) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
        GL_CHECK(glBindBuffer(m_target, 0));
    }

    void update(uint32_t _offset, uint32_t _size, void *_data, bool _discard = false)
    {
        BX_ASSERT(0 != m_id, "Updating invalid vertex buffer.");

        if (_discard)
        {
            // orphan buffer...
            destroy();
            create(m_size, NULL, m_layoutHandle, 0);
        }

        GL_CHECK(glBindBuffer(m_target, m_id));
        GL_CHECK(glBufferSubData(m_target, _offset, _size, _data));
        GL_CHECK(glBindBuffer(m_target, 0));
    }

    void destroy();

    GLuint m_id;
    GLenum m_target;
    uint32_t m_size;
    VertexLayoutHandle m_layoutHandle;
};