/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#ifndef BGFX_P_H_HEADER_GUARD
#define BGFX_P_H_HEADER_GUARD

#include <bx/platform.h>

#ifndef BGFX_CONFIG_DEBUG
#define BGFX_CONFIG_DEBUG 0
#endif // BGFX_CONFIG_DEBUG

#if BGFX_CONFIG_DEBUG || BX_COMPILER_CLANG_ANALYZER
#define BX_TRACE _BX_TRACE
#define BX_WARN _BX_WARN
#define BX_ASSERT _BX_ASSERT
#define BX_CONFIG_ALLOCATOR_DEBUG 1
#endif // BGFX_CONFIG_DEBUG

#include <Render/Render.h>

// #include "config.h"

#include <inttypes.h>

#include "RenderDebug.h"

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

#include "shader.h"
// #include "vertexlayout.h"
#include "Render/Tools/VertexLayout/vertexlayout.h"
#include <Render/Platform.h>
#include <bimg/bimg.h>

#define BGFX_CHUNK_MAGIC_TEX BX_MAKEFOURCC('T', 'E', 'X', 0x0)

#define BGFX_CLEAR_COLOR_USE_PALETTE UINT16_C(0x8000)
#define BGFX_CLEAR_MASK (0 | BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH | BGFX_CLEAR_STENCIL | BGFX_CLEAR_COLOR_USE_PALETTE)

#if BGFX_CONFIG_USE_TINYSTL
namespace bgfx
{
struct TinyStlAllocator
{
    static void *static_allocate(size_t _bytes);
    static void static_deallocate(void *_ptr, size_t /*_bytes*/);
};
} // namespace bgfx
#define TINYSTL_ALLOCATOR bgfx::TinyStlAllocator
#include <tinystl/string.h>
#include <tinystl/unordered_map.h>
#include <tinystl/unordered_set.h>
#include <tinystl/vector.h>

namespace tinystl
{
template <typename T, typename Alloc = TINYSTL_ALLOCATOR> class list : public vector<T, Alloc>
{
  public:
    void push_front(const T &_value)
    {
        this->insert(this->begin(), _value);
    }

    void pop_front()
    {
        this->erase(this->begin());
    }

    void sort()
    {
        bx::quickSort(this->begin(), uint32_t(this->end() - this->begin()), sizeof(T),
                      [](const void *_a, const void *_b) -> int32_t {
                          const T &lhs = *(const T *)(_a);
                          const T &rhs = *(const T *)(_b);
                          return lhs < rhs ? -1 : 1;
                      });
    }
};
} // namespace tinystl
namespace stl = tinystl;
#else
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
namespace stl = std;
#endif // BGFX_CONFIG_USE_TINYSTL

#include "RenderStructInternal.h"

#include "RenderCommandBuffer.h"

#include "Render/RTemp.h"

#include "RenderObject.h"

#include "Render/IRenderContext.h"

#include "Render/RenderContext.h"

#endif // BGFX_P_H_HEADER_GUARD
