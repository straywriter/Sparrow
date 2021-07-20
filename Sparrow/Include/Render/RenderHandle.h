#pragma once

#include "RenderVertexLayout.h"
#include "Render/RenderConfig.h"


///
#define BGFX_HANDLE(_name)                                                                                             \
    struct _name                                                                                                       \
    {                                                                                                                  \
        uint16_t idx;                                                                                                  \
    };                                                                                                                 \
    inline bool isValid(_name _handle)                                                                                 \
    {                                                                                                                  \
        return bgfx::kInvalidHandle != _handle.idx;                                                                    \
    }

#define BGFX_INVALID_HANDLE                                                                                            \
    {                                                                                                                  \
        bgfx::kInvalidHandle                                                                                           \
    }

namespace bgfx
{

static const uint16_t kInvalidHandle = UINT16_MAX;

BGFX_HANDLE(DynamicIndexBufferHandle)
BGFX_HANDLE(DynamicVertexBufferHandle)
BGFX_HANDLE(FrameBufferHandle)
BGFX_HANDLE(IndexBufferHandle)
BGFX_HANDLE(IndirectBufferHandle)
BGFX_HANDLE(OcclusionQueryHandle)
BGFX_HANDLE(ProgramHandle)
BGFX_HANDLE(ShaderHandle)
BGFX_HANDLE(TextureHandle)
BGFX_HANDLE(UniformHandle)
BGFX_HANDLE(VertexBufferHandle)
BGFX_HANDLE(VertexLayoutHandle)

struct Handle
{
    enum Enum
    {
        IndexBuffer,
        Shader,
        Texture,
        VertexBuffer,

        Count
    };

    uint16_t type;
    uint16_t idx;
};

#define CONVERT_HANDLE(_name)                                                                                          \
    inline Handle convert(_name##Handle _handle)                                                                       \
    {                                                                                                                  \
        Handle handle = {Handle::_name, _handle.idx};                                                                  \
        return handle;                                                                                                 \
    }

CONVERT_HANDLE(IndexBuffer);
CONVERT_HANDLE(Shader);
CONVERT_HANDLE(Texture);
CONVERT_HANDLE(VertexBuffer);

#undef CONVERT_HANDLE

const char *getTypeName(Handle _handle);

inline bool isValid(const VertexLayout &_layout)
{
    return 0 != _layout.m_stride;
}

// Check handle, cannot be bgfx::kInvalidHandle and must be valid.
#define BGFX_CHECK_HANDLE(_desc, _handleAlloc, _handle)                                                                \
    BX_ASSERT(isValid(_handle) && _handleAlloc.isValid(_handle.idx), "Invalid handle. %s handle: %d (max %d)", _desc,  \
              _handle.idx, _handleAlloc.getMaxHandles())

// Check handle, it's ok to be bgfx::kInvalidHandle or must be valid.
#define BGFX_CHECK_HANDLE_INVALID_OK(_desc, _handleAlloc, _handle)                                                     \
    BX_ASSERT(!isValid(_handle) || _handleAlloc.isValid(_handle.idx), "Invalid handle. %s handle: %d (max %d)", _desc, \
              _handle.idx, _handleAlloc.getMaxHandles())

inline bool operator==(const VertexLayoutHandle &_lhs, const VertexLayoutHandle &_rhs)
{
    return _lhs.idx == _rhs.idx;
}
inline bool operator==(const UniformHandle &_lhs, const UniformHandle &_rhs)
{
    return _lhs.idx == _rhs.idx;
}


} // namespace bgfx