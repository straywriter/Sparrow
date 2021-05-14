#include "Render/Detail/RenderRequirement.h

#incldue "Render/Detail/RenderStructInternal.h"

BX_ALIGN_DECL_CACHE_LINE(struct) View
{
    void reset()
    {
        setRect(0, 0, 1, 1);
        setScissor(0, 0, 0, 0);
        setClear(BGFX_CLEAR_NONE, 0, 0.0f, 0);
        setMode(ViewMode::Default);
        setFrameBuffer(BGFX_INVALID_HANDLE);
        setTransform(NULL, NULL);
    }

    void setRect(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height)
    {
        m_rect.m_x = uint16_t(bx::max<int16_t>(int16_t(_x), 0));
        m_rect.m_y = uint16_t(bx::max<int16_t>(int16_t(_y), 0));
        m_rect.m_width = bx::max<uint16_t>(_width, 1);
        m_rect.m_height = bx::max<uint16_t>(_height, 1);
    }

    void setScissor(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height)
    {
        m_scissor.m_x = _x;
        m_scissor.m_y = _y;
        m_scissor.m_width = _width;
        m_scissor.m_height = _height;
    }

    void setClear(uint16_t _flags, uint32_t _rgba, float _depth, uint8_t _stencil)
    {
        m_clear.set(_flags, _rgba, _depth, _stencil);
    }

    void setClear(uint16_t _flags, float _depth, uint8_t _stencil, uint8_t _0, uint8_t _1, uint8_t _2, uint8_t _3,
                  uint8_t _4, uint8_t _5, uint8_t _6, uint8_t _7)
    {
        m_clear.set(_flags, _depth, _stencil, _0, _1, _2, _3, _4, _5, _6, _7);
    }

    void setMode(ViewMode::Enum _mode)
    {
        m_mode = uint8_t(_mode);
    }

    void setFrameBuffer(FrameBufferHandle _handle)
    {
        m_fbh = _handle;
    }

    void setTransform(const void *_view, const void *_proj)
    {
        if (NULL != _view)
        {
            bx::memCopy(m_view.un.val, _view, sizeof(Matrix4));
        }
        else
        {
            m_view.setIdentity();
        }

        if (NULL != _proj)
        {
            bx::memCopy(m_proj.un.val, _proj, sizeof(Matrix4));
        }
        else
        {
            m_proj.setIdentity();
        }
    }

    Clear m_clear;
    Rect m_rect;
    Rect m_scissor;
    Matrix4 m_view;
    Matrix4 m_proj;
    FrameBufferHandle m_fbh;
    uint8_t m_mode;
};
