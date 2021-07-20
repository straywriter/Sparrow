#pragma once

#include "Render/RenderConfig.h"

namespace bgfx
{

    class CommandBuffer
{
    BX_CLASS(CommandBuffer, NO_COPY, NO_ASSIGNMENT);

  public:
    CommandBuffer() : m_buffer(NULL), m_pos(0), m_size(0), m_minCapacity(0)
    {
        resize();
        finish();
    }

    ~CommandBuffer()
    {
        BX_FREE(g_allocator, m_buffer);
    }

    void init(uint32_t _minCapacity)
    {
        m_minCapacity = bx::alignUp(_minCapacity, 1024);
        resize();
    }

    enum Enum
    {
        RendererInit,
        RendererShutdownBegin,
        CreateVertexLayout,
        CreateIndexBuffer,
        CreateVertexBuffer,
        CreateDynamicIndexBuffer,
        UpdateDynamicIndexBuffer,
        CreateDynamicVertexBuffer,
        UpdateDynamicVertexBuffer,
        CreateShader,
        CreateProgram,
        CreateTexture,
        UpdateTexture,
        ResizeTexture,
        CreateFrameBuffer,
        CreateUniform,
        UpdateViewName,
        InvalidateOcclusionQuery,
        SetName,
        End,
        RendererShutdownEnd,
        DestroyVertexLayout,
        DestroyIndexBuffer,
        DestroyVertexBuffer,
        DestroyDynamicIndexBuffer,
        DestroyDynamicVertexBuffer,
        DestroyShader,
        DestroyProgram,
        DestroyTexture,
        DestroyFrameBuffer,
        DestroyUniform,
        ReadTexture,
        RequestScreenShot,
    };

    void resize(uint32_t _capacity = 0)
    {
        m_capacity = bx::alignUp(bx::max(_capacity, m_minCapacity), 1024);
        m_buffer = (uint8_t *)BX_REALLOC(g_allocator, m_buffer, m_capacity);
    }

    void write(const void *_data, uint32_t _size)
    {
        BX_ASSERT(m_size == 0, "Called write outside start/finish (m_size: %d)?", m_size);
        if (m_pos + _size > m_capacity)
        {
            resize(m_capacity + (16 << 10));
        }

        bx::memCopy(&m_buffer[m_pos], _data, _size);
        m_pos += _size;
    }

    template <typename Type> void write(const Type &_in)
    {
        align(BX_ALIGNOF(Type));
        write(reinterpret_cast<const uint8_t *>(&_in), sizeof(Type));
    }

    void read(void *_data, uint32_t _size)
    {
        BX_ASSERT(m_pos + _size <= m_size, "CommandBuffer::read error (pos: %d-%d, size: %d).", m_pos, m_pos + _size,
                  m_size);
        bx::memCopy(_data, &m_buffer[m_pos], _size);
        m_pos += _size;
    }

    template <typename Type> void read(Type &_in)
    {
        align(BX_ALIGNOF(Type));
        read(reinterpret_cast<uint8_t *>(&_in), sizeof(Type));
    }

    const uint8_t *skip(uint32_t _size)
    {
        BX_ASSERT(m_pos + _size <= m_size, "CommandBuffer::skip error (pos: %d-%d, size: %d).", m_pos, m_pos + _size,
                  m_size);
        const uint8_t *result = &m_buffer[m_pos];
        m_pos += _size;
        return result;
    }

    template <typename Type> void skip()
    {
        align(BX_ALIGNOF(Type));
        skip(sizeof(Type));
    }

    void align(uint32_t _alignment)
    {
        const uint32_t mask = _alignment - 1;
        const uint32_t pos = (m_pos + mask) & (~mask);
        m_pos = pos;
    }

    void reset()
    {
        m_pos = 0;
    }

    void start()
    {
        m_pos = 0;
        m_size = 0;
    }

    void finish()
    {
        uint8_t cmd = End;
        write(cmd);
        m_size = m_pos;
        m_pos = 0;

        if (m_size < m_minCapacity && m_capacity != m_minCapacity)
        {
            resize();
        }
    }

    uint8_t *m_buffer;
    uint32_t m_pos;
    uint32_t m_size;
    uint32_t m_capacity;
    uint32_t m_minCapacity;
};
}