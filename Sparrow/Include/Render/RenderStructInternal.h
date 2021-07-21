#pragma once

#include <bx/allocator.h>
#include <bx/bx.h>
#include <bx/cpu.h>
#include <bx/debug.h>
#include <bx/endian.h>
#include <bx/float4x4_t.h>
#include <bx/handlealloc.h>
#include <bx/hash.h>
#include <bx/math.h>
#include <bx/mutex.h>
#include <bx/os.h>
#include <bx/readerwriter.h>
#include <bx/ringbuffer.h>
#include <bx/sort.h>
#include <bx/string.h>
#include <bx/thread.h>
#include <bx/timer.h>
#include <bx/uint32_t.h>

#include "RenderEnum.h"
#include "Render/RenderConfig.h"
#include "RenderDefine.h"
#include "RenderStruct.h"
#include "RenderHandle.h"
// #include "RenderInternal.h"

namespace bgfx
{
extern bx::AllocatorI *g_allocator;

struct Clear
{
    void set(uint16_t _flags, uint32_t _rgba, float _depth, uint8_t _stencil)
    {
        m_flags = _flags;
        m_index[0] = uint8_t(_rgba >> 24);
        m_index[1] = uint8_t(_rgba >> 16);
        m_index[2] = uint8_t(_rgba >> 8);
        m_index[3] = uint8_t(_rgba >> 0);
        m_depth = _depth;
        m_stencil = _stencil;
    }

    void set(uint16_t _flags, float _depth, uint8_t _stencil, uint8_t _0, uint8_t _1, uint8_t _2, uint8_t _3,
             uint8_t _4, uint8_t _5, uint8_t _6, uint8_t _7)
    {
        m_flags =
            (_flags & ~BGFX_CLEAR_COLOR) |
            (0xff != (_0 & _1 & _2 & _3 & _4 & _5 & _6 & _7) ? BGFX_CLEAR_COLOR | BGFX_CLEAR_COLOR_USE_PALETTE : 0);
        m_index[0] = _0;
        m_index[1] = _1;
        m_index[2] = _2;
        m_index[3] = _3;
        m_index[4] = _4;
        m_index[5] = _5;
        m_index[6] = _6;
        m_index[7] = _7;
        m_depth = _depth;
        m_stencil = _stencil;
    }

    uint8_t m_index[8];
    float m_depth;
    uint8_t m_stencil;
    uint16_t m_flags;
};

struct Rect
{
    Rect()
    {
    }

    Rect(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height)
        : m_x(_x), m_y(_y), m_width(_width), m_height(_height)
    {
    }

    void clear()
    {
        m_x = m_y = m_width = m_height = 0;
    }

    bool isZero() const
    {
        uint64_t ui64 = *((uint64_t *)this);
        return UINT64_C(0) == ui64;
    }

    bool isZeroArea() const
    {
        return 0 == m_width || 0 == m_height;
    }

    void set(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height)
    {
        m_x = _x;
        m_y = _y;
        m_width = _width;
        m_height = _height;
    }

    void setIntersect(const Rect &_a, const Rect &_b)
    {
        const uint16_t sx = bx::max<uint16_t>(_a.m_x, _b.m_x);
        const uint16_t sy = bx::max<uint16_t>(_a.m_y, _b.m_y);
        const uint16_t ex = bx::min<uint16_t>(_a.m_x + _a.m_width, _b.m_x + _b.m_width);
        const uint16_t ey = bx::min<uint16_t>(_a.m_y + _a.m_height, _b.m_y + _b.m_height);
        m_x = sx;
        m_y = sy;
        m_width = (uint16_t)bx::uint32_satsub(ex, sx);
        m_height = (uint16_t)bx::uint32_satsub(ey, sy);
    }

    void intersect(const Rect &_a)
    {
        setIntersect(*this, _a);
    }

    uint16_t m_x;
    uint16_t m_y;
    uint16_t m_width;
    uint16_t m_height;
};

struct TextureCreate
{
    TextureFormat::Enum m_format;
    uint16_t m_width;
    uint16_t m_height;
    uint16_t m_depth;
    uint16_t m_numLayers;
    uint8_t m_numMips;
    bool m_cubeMap;
    const Memory *m_mem;
};

struct TextVideoMem
{
    TextVideoMem() : m_mem(NULL), m_size(0), m_width(0), m_height(0), m_small(false)
    {
        resize(false, 1, 1);
        clear();
    }

    ~TextVideoMem()
    {
        BX_FREE(g_allocator, m_mem);
    }

    void resize(bool _small, uint32_t _width, uint32_t _height)
    {
        uint32_t width = bx::uint32_imax(1, _width / 8);
        uint32_t height = bx::uint32_imax(1, _height / (_small ? 8 : 16));

        if (NULL == m_mem || m_width != width || m_height != height || m_small != _small)
        {
            m_small = _small;
            m_width = (uint16_t)width;
            m_height = (uint16_t)height;

            uint32_t size = m_size;
            m_size = m_width * m_height;

            m_mem = (MemSlot *)BX_REALLOC(g_allocator, m_mem, m_size * sizeof(MemSlot));

            if (size < m_size)
            {
                bx::memSet(&m_mem[size], 0, (m_size - size) * sizeof(MemSlot));
            }
        }
    }

    void clear(uint8_t _attr = 0)
    {
        MemSlot *mem = m_mem;
        bx::memSet(mem, 0, m_size * sizeof(MemSlot));
        if (_attr != 0)
        {
            for (uint32_t ii = 0, num = m_size; ii < num; ++ii)
            {
                mem[ii].attribute = _attr;
            }
        }
    }

    void printfVargs(uint16_t _x, uint16_t _y, uint8_t _attr, const char *_format, va_list _argList);

    void printf(uint16_t _x, uint16_t _y, uint8_t _attr, const char *_format, ...)
    {
        va_list argList;
        va_start(argList, _format);
        printfVargs(_x, _y, _attr, _format, argList);
        va_end(argList);
    }

    void image(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height, const void *_data, uint16_t _pitch)
    {
        if (_x < m_width && _y < m_height)
        {
            MemSlot *dst = &m_mem[_y * m_width + _x];
            const uint8_t *src = (const uint8_t *)_data;
            const uint32_t width = bx::min<uint32_t>(m_width, _width + _x) - _x;
            const uint32_t height = bx::min<uint32_t>(m_height, _height + _y) - _y;
            const uint32_t dstPitch = m_width;

            for (uint32_t ii = 0; ii < height; ++ii)
            {
                for (uint32_t jj = 0; jj < width; ++jj)
                {
                    dst[jj].character = src[jj * 2];
                    dst[jj].attribute = src[jj * 2 + 1];
                }

                src += _pitch;
                dst += dstPitch;
            }
        }
    }

    struct MemSlot
    {
        uint8_t attribute;
        uint8_t character;
    };

    MemSlot *m_mem;
    uint32_t m_size;
    uint16_t m_width;
    uint16_t m_height;
    bool m_small;
};

struct TextVideoMemBlitter
{
    void init();
    void shutdown();

    TextureHandle m_texture;
    TransientVertexBuffer *m_vb;
    TransientIndexBuffer *m_ib;
    VertexLayout m_layout;
    ProgramHandle m_program;
};


template <uint32_t maxKeys> struct UpdateBatchT
{
    UpdateBatchT() : m_num(0)
    {
    }

    void add(uint32_t _key, uint32_t _value)
    {
        uint32_t num = m_num++;
        m_keys[num] = _key;
        m_values[num] = _value;
    }

    bool sort()
    {
        if (0 < m_num)
        {
            uint32_t *tempKeys = (uint32_t *)alloca(sizeof(m_keys));
            uint32_t *tempValues = (uint32_t *)alloca(sizeof(m_values));
            bx::radixSort(m_keys, tempKeys, m_values, tempValues, m_num);
            return true;
        }

        return false;
    }

    bool isFull() const
    {
        return m_num >= maxKeys;
    }

    void reset()
    {
        m_num = 0;
    }

    uint32_t m_num;
    uint32_t m_keys[maxKeys];
    uint32_t m_values[maxKeys];
};

struct ClearQuad
{
    ClearQuad()
    {
        for (uint32_t ii = 0; ii < BX_COUNTOF(m_program); ++ii)
        {
            m_program[ii].idx = kInvalidHandle;
        }
    }

    void init();
    void shutdown();

    VertexBufferHandle m_vb;
    VertexLayout m_layout;
    ProgramHandle m_program[BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS];
};

struct PredefinedUniform
{
    enum Enum
    {
        ViewRect,
        ViewTexel,
        View,
        InvView,
        Proj,
        InvProj,
        ViewProj,
        InvViewProj,
        Model,
        ModelView,
        ModelViewProj,
        AlphaRef,
        Count
    };

    uint32_t m_loc;
    uint16_t m_count;
    uint8_t m_type;
};

void charsetFillTexture(const uint8_t *_charset, uint8_t *_rgba, uint32_t _height, uint32_t _pitch, uint32_t _bpp);

} // namespace bgfx