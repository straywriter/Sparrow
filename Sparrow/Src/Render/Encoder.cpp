#include "Render/Encoder.h"

#include "RenderContext.h"

namespace bgfx
{

extern Context *s_ctx;
Encoder *begin(bool _forThread)
{
    return s_ctx->begin(_forThread);
}

#define BGFX_ENCODER(_func) reinterpret_cast<EncoderImpl *>(this)->_func

void Encoder::setMarker(const char *_marker)
{
    BGFX_ENCODER(setMarker(_marker));
}

void Encoder::setState(uint64_t _state, uint32_t _rgba)
{
    BX_ASSERT(0 == (_state & BGFX_STATE_RESERVED_MASK), "Do not set state reserved flags!");
    BGFX_ENCODER(setState(_state, _rgba));
}

void Encoder::setCondition(OcclusionQueryHandle _handle, bool _visible)
{
    BGFX_CHECK_CAPS(BGFX_CAPS_OCCLUSION_QUERY, "Occlusion query is not supported!");
    BGFX_ENCODER(setCondition(_handle, _visible));
}

void Encoder::setStencil(uint32_t _fstencil, uint32_t _bstencil)
{
    BGFX_ENCODER(setStencil(_fstencil, _bstencil));
}

uint16_t Encoder::setScissor(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height)
{
    return BGFX_ENCODER(setScissor(_x, _y, _width, _height));
}

void Encoder::setScissor(uint16_t _cache)
{
    BGFX_ENCODER(setScissor(_cache));
}

uint32_t Encoder::setTransform(const void *_mtx, uint16_t _num)
{
    return BGFX_ENCODER(setTransform(_mtx, _num));
}

uint32_t Encoder::allocTransform(Transform *_transform, uint16_t _num)
{
    return BGFX_ENCODER(allocTransform(_transform, _num));
}

void Encoder::setTransform(uint32_t _cache, uint16_t _num)
{
    BGFX_ENCODER(setTransform(_cache, _num));
}

void Encoder::setUniform(UniformHandle _handle, const void *_value, uint16_t _num)
{
    BGFX_CHECK_HANDLE("setUniform", s_ctx->m_uniformHandle, _handle);
    const UniformRef &uniform = s_ctx->m_uniformRef[_handle.idx];
    BX_ASSERT(isValid(_handle) && 0 < uniform.m_refCount, "Setting invalid uniform (handle %3d)!", _handle.idx);
    BX_ASSERT(_num == UINT16_MAX || uniform.m_num >= _num, "Truncated uniform update. %d (max: %d)", _num,
              uniform.m_num);
    BGFX_ENCODER(setUniform(uniform.m_type, _handle, _value, UINT16_MAX != _num ? _num : uniform.m_num));
}

void Encoder::setIndexBuffer(IndexBufferHandle _handle)
{
    setIndexBuffer(_handle, 0, UINT32_MAX);
}

void Encoder::setIndexBuffer(IndexBufferHandle _handle, uint32_t _firstIndex, uint32_t _numIndices)
{
    BGFX_CHECK_HANDLE("setIndexBuffer", s_ctx->m_indexBufferHandle, _handle);
    BGFX_ENCODER(setIndexBuffer(_handle, _firstIndex, _numIndices));
}

void Encoder::setIndexBuffer(DynamicIndexBufferHandle _handle)
{
    setIndexBuffer(_handle, 0, UINT32_MAX);
}

void Encoder::setIndexBuffer(DynamicIndexBufferHandle _handle, uint32_t _firstIndex, uint32_t _numIndices)
{
    BGFX_CHECK_HANDLE("setIndexBuffer", s_ctx->m_dynamicIndexBufferHandle, _handle);
    const DynamicIndexBuffer &dib = s_ctx->m_dynamicIndexBuffers[_handle.idx];
    BGFX_ENCODER(setIndexBuffer(dib, _firstIndex, _numIndices));
}

void Encoder::setIndexBuffer(const TransientIndexBuffer *_tib)
{
    setIndexBuffer(_tib, 0, UINT32_MAX);
}

void Encoder::setIndexBuffer(const TransientIndexBuffer *_tib, uint32_t _firstIndex, uint32_t _numIndices)
{
    BX_ASSERT(NULL != _tib, "_tib can't be NULL");
    BGFX_CHECK_HANDLE("setIndexBuffer", s_ctx->m_indexBufferHandle, _tib->handle);
    BGFX_ENCODER(setIndexBuffer(_tib, _firstIndex, _numIndices));
}

void Encoder::setVertexBuffer(uint8_t _stream, VertexBufferHandle _handle, uint32_t _startVertex, uint32_t _numVertices,
                              VertexLayoutHandle _layoutHandle)
{
    BGFX_CHECK_HANDLE("setVertexBuffer", s_ctx->m_vertexBufferHandle, _handle);
    BGFX_CHECK_HANDLE_INVALID_OK("setVertexBuffer", s_ctx->m_layoutHandle, _layoutHandle);
    BGFX_ENCODER(setVertexBuffer(_stream, _handle, _startVertex, _numVertices, _layoutHandle));
}

void Encoder::setVertexBuffer(uint8_t _stream, VertexBufferHandle _handle)
{
    setVertexBuffer(_stream, _handle, 0, UINT32_MAX);
}

void Encoder::setVertexBuffer(uint8_t _stream, DynamicVertexBufferHandle _handle, uint32_t _startVertex,
                              uint32_t _numVertices, VertexLayoutHandle _layoutHandle)
{
    BGFX_CHECK_HANDLE("setVertexBuffer", s_ctx->m_dynamicVertexBufferHandle, _handle);
    BGFX_CHECK_HANDLE_INVALID_OK("setVertexBuffer", s_ctx->m_layoutHandle, _layoutHandle);
    const DynamicVertexBuffer &dvb = s_ctx->m_dynamicVertexBuffers[_handle.idx];
    BGFX_ENCODER(setVertexBuffer(_stream, dvb, _startVertex, _numVertices, _layoutHandle));
}

void Encoder::setVertexBuffer(uint8_t _stream, DynamicVertexBufferHandle _handle)
{
    setVertexBuffer(_stream, _handle, 0, UINT32_MAX);
}

void Encoder::setVertexBuffer(uint8_t _stream, const TransientVertexBuffer *_tvb, uint32_t _startVertex,
                              uint32_t _numVertices, VertexLayoutHandle _layoutHandle)
{
    BX_ASSERT(NULL != _tvb, "_tvb can't be NULL");
    BGFX_CHECK_HANDLE("setVertexBuffer", s_ctx->m_vertexBufferHandle, _tvb->handle);
    BGFX_CHECK_HANDLE_INVALID_OK("setVertexBuffer", s_ctx->m_layoutHandle, _layoutHandle);
    BGFX_ENCODER(setVertexBuffer(_stream, _tvb, _startVertex, _numVertices, _layoutHandle));
}

void Encoder::setVertexBuffer(uint8_t _stream, const TransientVertexBuffer *_tvb)
{
    setVertexBuffer(_stream, _tvb, 0, UINT32_MAX);
}

void Encoder::setVertexCount(uint32_t _numVertices)
{
    BGFX_CHECK_CAPS(BGFX_CAPS_VERTEX_ID, "Auto generated vertices are not supported!");
    BGFX_ENCODER(setVertexCount(_numVertices));
}

void Encoder::setInstanceDataBuffer(const InstanceDataBuffer *_idb)
{
    setInstanceDataBuffer(_idb, 0, UINT32_MAX);
}

void Encoder::setInstanceDataBuffer(const InstanceDataBuffer *_idb, uint32_t _start, uint32_t _num)
{
    BX_ASSERT(NULL != _idb, "_idb can't be NULL");
    BGFX_ENCODER(setInstanceDataBuffer(_idb, _start, _num));
}

void Encoder::setInstanceDataBuffer(VertexBufferHandle _handle, uint32_t _startVertex, uint32_t _num)
{
    BGFX_CHECK_HANDLE("setInstanceDataBuffer", s_ctx->m_vertexBufferHandle, _handle);
    const VertexBuffer &vb = s_ctx->m_vertexBuffers[_handle.idx];
    BGFX_ENCODER(setInstanceDataBuffer(_handle, _startVertex, _num, vb.m_stride));
}

void Encoder::setInstanceDataBuffer(DynamicVertexBufferHandle _handle, uint32_t _startVertex, uint32_t _num)
{
    BGFX_CHECK_HANDLE("setInstanceDataBuffer", s_ctx->m_dynamicVertexBufferHandle, _handle);
    const DynamicVertexBuffer &dvb = s_ctx->m_dynamicVertexBuffers[_handle.idx];
    BGFX_ENCODER(setInstanceDataBuffer(dvb.m_handle, dvb.m_startVertex + _startVertex, _num, dvb.m_stride));
}

void Encoder::setInstanceCount(uint32_t _numInstances)
{
    BGFX_CHECK_CAPS(BGFX_CAPS_VERTEX_ID, "Auto generated instances are not supported!");
    BGFX_ENCODER(setInstanceCount(_numInstances));
}

void Encoder::setTexture(uint8_t _stage, UniformHandle _sampler, TextureHandle _handle, uint32_t _flags)
{
    BGFX_CHECK_HANDLE("setTexture/UniformHandle", s_ctx->m_uniformHandle, _sampler);
    BGFX_CHECK_HANDLE_INVALID_OK("setTexture/TextureHandle", s_ctx->m_textureHandle, _handle);
    BX_ASSERT(_stage < g_caps.limits.maxTextureSamplers, "Invalid stage %d (max %d).", _stage,
              g_caps.limits.maxTextureSamplers);
    BGFX_ENCODER(setTexture(_stage, _sampler, _handle, _flags));
}

void Encoder::touch(ViewId _id)
{
    ProgramHandle handle = BGFX_INVALID_HANDLE;
    submit(_id, handle);
}

void Encoder::submit(ViewId _id, ProgramHandle _program, uint32_t _depth, uint8_t _flags)
{
    OcclusionQueryHandle handle = BGFX_INVALID_HANDLE;
    submit(_id, _program, handle, _depth, _flags);
}

void Encoder::submit(ViewId _id, ProgramHandle _program, OcclusionQueryHandle _occlusionQuery, uint32_t _depth,
                     uint8_t _flags)
{
    BX_ASSERT(false || !isValid(_occlusionQuery) || 0 != (g_caps.supported & BGFX_CAPS_OCCLUSION_QUERY),
              "Occlusion query is not supported! Use bgfx::getCaps to check BGFX_CAPS_OCCLUSION_QUERY backend renderer "
              "capabilities.");
    BGFX_CHECK_HANDLE_INVALID_OK("submit", s_ctx->m_programHandle, _program);
    BGFX_CHECK_HANDLE_INVALID_OK("submit", s_ctx->m_occlusionQueryHandle, _occlusionQuery);
    BGFX_ENCODER(submit(_id, _program, _occlusionQuery, _depth, _flags));
}

void Encoder::submit(ViewId _id, ProgramHandle _program, IndirectBufferHandle _indirectHandle, uint16_t _start,
                     uint16_t _num, uint32_t _depth, uint8_t _flags)
{
    BGFX_CHECK_HANDLE_INVALID_OK("submit", s_ctx->m_programHandle, _program);
    BGFX_CHECK_HANDLE("submit", s_ctx->m_vertexBufferHandle, _indirectHandle);
    BGFX_CHECK_CAPS(BGFX_CAPS_DRAW_INDIRECT, "Draw indirect is not supported!");
    BGFX_ENCODER(submit(_id, _program, _indirectHandle, _start, _num, _depth, _flags));
}

void Encoder::setBuffer(uint8_t _stage, IndexBufferHandle _handle, Access::Enum _access)
{
    BX_ASSERT(_stage < g_caps.limits.maxComputeBindings, "Invalid stage %d (max %d).", _stage,
              g_caps.limits.maxComputeBindings);
    BGFX_CHECK_HANDLE("setBuffer", s_ctx->m_indexBufferHandle, _handle);
    BGFX_ENCODER(setBuffer(_stage, _handle, _access));
}

void Encoder::setBuffer(uint8_t _stage, VertexBufferHandle _handle, Access::Enum _access)
{
    BX_ASSERT(_stage < g_caps.limits.maxComputeBindings, "Invalid stage %d (max %d).", _stage,
              g_caps.limits.maxComputeBindings);
    BGFX_CHECK_HANDLE("setBuffer", s_ctx->m_vertexBufferHandle, _handle);
    BGFX_ENCODER(setBuffer(_stage, _handle, _access));
}

void Encoder::setBuffer(uint8_t _stage, DynamicIndexBufferHandle _handle, Access::Enum _access)
{
    BX_ASSERT(_stage < g_caps.limits.maxComputeBindings, "Invalid stage %d (max %d).", _stage,
              g_caps.limits.maxComputeBindings);
    BGFX_CHECK_HANDLE("setBuffer", s_ctx->m_dynamicIndexBufferHandle, _handle);
    const DynamicIndexBuffer &dib = s_ctx->m_dynamicIndexBuffers[_handle.idx];
    BGFX_ENCODER(setBuffer(_stage, dib.m_handle, _access));
}

void Encoder::setBuffer(uint8_t _stage, DynamicVertexBufferHandle _handle, Access::Enum _access)
{
    BX_ASSERT(_stage < g_caps.limits.maxComputeBindings, "Invalid stage %d (max %d).", _stage,
              g_caps.limits.maxComputeBindings);
    BGFX_CHECK_HANDLE("setBuffer", s_ctx->m_dynamicVertexBufferHandle, _handle);
    const DynamicVertexBuffer &dvb = s_ctx->m_dynamicVertexBuffers[_handle.idx];
    BGFX_ENCODER(setBuffer(_stage, dvb.m_handle, _access));
}

void Encoder::setBuffer(uint8_t _stage, IndirectBufferHandle _handle, Access::Enum _access)
{
    BX_ASSERT(_stage < g_caps.limits.maxComputeBindings, "Invalid stage %d (max %d).", _stage,
              g_caps.limits.maxComputeBindings);
    BGFX_CHECK_HANDLE("setBuffer", s_ctx->m_vertexBufferHandle, _handle);
    VertexBufferHandle handle = {_handle.idx};
    BGFX_ENCODER(setBuffer(_stage, handle, _access));
}

void Encoder::setImage(uint8_t _stage, TextureHandle _handle, uint8_t _mip, Access::Enum _access,
                       TextureFormat::Enum _format)
{
    BX_ASSERT(_stage < g_caps.limits.maxComputeBindings, "Invalid stage %d (max %d).", _stage,
              g_caps.limits.maxComputeBindings);
    BGFX_CHECK_HANDLE_INVALID_OK("setImage/TextureHandle", s_ctx->m_textureHandle, _handle);
    _format =
        TextureFormat::Count == _format ? TextureFormat::Enum(s_ctx->m_textureRef[_handle.idx].m_format) : _format;
    BX_ASSERT(_format != TextureFormat::BGRA8,
              "Can't use TextureFormat::BGRA8 with compute, use TextureFormat::RGBA8 instead.");
    BGFX_ENCODER(setImage(_stage, _handle, _mip, _access, _format));
}

void Encoder::dispatch(ViewId _id, ProgramHandle _program, uint32_t _numX, uint32_t _numY, uint32_t _numZ,
                       uint8_t _flags)
{
    BGFX_CHECK_CAPS(BGFX_CAPS_COMPUTE, "Compute is not supported!");
    BGFX_CHECK_HANDLE_INVALID_OK("dispatch", s_ctx->m_programHandle, _program);
    BGFX_ENCODER(dispatch(_id, _program, _numX, _numY, _numZ, _flags));
}

void Encoder::dispatch(ViewId _id, ProgramHandle _program, IndirectBufferHandle _indirectHandle, uint16_t _start,
                       uint16_t _num, uint8_t _flags)
{
    BGFX_CHECK_CAPS(BGFX_CAPS_DRAW_INDIRECT, "Dispatch indirect is not supported!");
    BGFX_CHECK_CAPS(BGFX_CAPS_COMPUTE, "Compute is not supported!");
    BGFX_CHECK_HANDLE_INVALID_OK("dispatch", s_ctx->m_programHandle, _program);
    BGFX_CHECK_HANDLE("dispatch", s_ctx->m_vertexBufferHandle, _indirectHandle);
    BGFX_ENCODER(dispatch(_id, _program, _indirectHandle, _start, _num, _flags));
}

void Encoder::discard(uint8_t _flags)
{
    BGFX_ENCODER(discard(_flags));
}

void Encoder::blit(ViewId _id, TextureHandle _dst, uint16_t _dstX, uint16_t _dstY, TextureHandle _src, uint16_t _srcX,
                   uint16_t _srcY, uint16_t _width, uint16_t _height)
{
    blit(_id, _dst, 0, _dstX, _dstY, 0, _src, 0, _srcX, _srcY, 0, _width, _height, 0);
}

void Encoder::blit(ViewId _id, TextureHandle _dst, uint8_t _dstMip, uint16_t _dstX, uint16_t _dstY, uint16_t _dstZ,
                   TextureHandle _src, uint8_t _srcMip, uint16_t _srcX, uint16_t _srcY, uint16_t _srcZ, uint16_t _width,
                   uint16_t _height, uint16_t _depth)
{
    BGFX_CHECK_CAPS(BGFX_CAPS_TEXTURE_BLIT, "Texture blit is not supported!");
    BGFX_CHECK_HANDLE("blit/src TextureHandle", s_ctx->m_textureHandle, _src);
    BGFX_CHECK_HANDLE("blit/dst TextureHandle", s_ctx->m_textureHandle, _dst);
    const TextureRef &src = s_ctx->m_textureRef[_src.idx];
    const TextureRef &dst = s_ctx->m_textureRef[_dst.idx];
    BX_ASSERT(src.m_format == dst.m_format, "Texture format must match (src %s, dst %s).",
              bimg::getName(bimg::TextureFormat::Enum(src.m_format)),
              bimg::getName(bimg::TextureFormat::Enum(dst.m_format)));
    BX_UNUSED(src, dst);
    BGFX_ENCODER(
        blit(_id, _dst, _dstMip, _dstX, _dstY, _dstZ, _src, _srcMip, _srcX, _srcY, _srcZ, _width, _height, _depth));
}

#undef BGFX_ENCODER
} // namespace bgfx