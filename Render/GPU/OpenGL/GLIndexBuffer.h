

struct IndexBufferGL
{
    void create(uint32_t _size, void *_data, uint16_t _flags)
    {
        m_size = _size;
        m_flags = _flags;

        GL_CHECK(glGenBuffers(1, &m_id));
        BX_ASSERT(0 != m_id, "Failed to generate buffer id.");
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
        GL_CHECK(
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, _data, (NULL == _data) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    void update(uint32_t _offset, uint32_t _size, void *_data, bool _discard = false)
    {
        BX_ASSERT(0 != m_id, "Updating invalid index buffer.");

        if (_discard)
        {
            // orphan buffer...
            destroy();
            create(m_size, NULL, m_flags);
        }

        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
        GL_CHECK(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, _offset, _size, _data));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    void destroy();

    GLuint m_id;
    uint32_t m_size;
    uint16_t m_flags;
};