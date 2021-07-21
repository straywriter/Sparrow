

#pragma once

#include <stdint.h>
// #include "RenderStruct.h"
// #include "RenderEnum.h"
#include "Render/RenderHandle.h"
#include "Render/RenderDefine.h"
#include "Render/RenderStruct.h"

namespace bgfx
{

///
typedef uint16_t ViewId;

/// View stats.
///
/// @attention C99 equivalent is `bgfx_view_stats_t`.
///
struct ViewStats
{
    char name[256];       //!< View name.
    ViewId view;          //!< View id.
    int64_t cpuTimeBegin; //!< CPU (submit) begin time.
    int64_t cpuTimeEnd;   //!< CPU (submit) end time.
    int64_t gpuTimeBegin; //!< GPU begin time.
    int64_t gpuTimeEnd;   //!< GPU end time.
};

/// Encoder stats.
///
/// @attention C99 equivalent is `bgfx_encoder_stats_t`.
///
struct EncoderStats
{
    int64_t cpuTimeBegin; //!< Encoder thread CPU submit begin time.
    int64_t cpuTimeEnd;   //!< Encoder thread CPU submit end time.
};

/// Renderer statistics data.
///
/// @attention C99 equivalent is `bgfx_stats_t`.
///
/// @remarks All time values are high-resolution timestamps, while
///   time frequencies define timestamps-per-second for that hardware.
struct Stats
{
    int64_t cpuTimeFrame; //!< CPU time between two `bgfx::frame` calls.
    int64_t cpuTimeBegin; //!< Render thread CPU submit begin time.
    int64_t cpuTimeEnd;   //!< Render thread CPU submit end time.
    int64_t cpuTimerFreq; //!< CPU timer frequency. Timestamps-per-second

    int64_t gpuTimeBegin; //!< GPU frame begin time.
    int64_t gpuTimeEnd;   //!< GPU frame end time.
    int64_t gpuTimerFreq; //!< GPU timer frequency.

    int64_t waitRender; //!< Time spent waiting for render backend thread to finish issuing
                        //!  draw commands to underlying graphics API.
    int64_t waitSubmit; //!< Time spent waiting for submit thread to advance to next frame.

    uint32_t numDraw;       //!< Number of draw calls submitted.
    uint32_t numCompute;    //!< Number of compute calls submitted.
    uint32_t numBlit;       //!< Number of blit calls submitted.
    uint32_t maxGpuLatency; //!< GPU driver latency.

    uint16_t numDynamicIndexBuffers;  //!< Number of used dynamic index buffers.
    uint16_t numDynamicVertexBuffers; //!< Number of used dynamic vertex buffers.
    uint16_t numFrameBuffers;         //!< Number of used frame buffers.
    uint16_t numIndexBuffers;         //!< Number of used index buffers.
    uint16_t numOcclusionQueries;     //!< Number of used occlusion queries.
    uint16_t numPrograms;             //!< Number of used programs.
    uint16_t numShaders;              //!< Number of used shaders.
    uint16_t numTextures;             //!< Number of used textures.
    uint16_t numUniforms;             //!< Number of used uniforms.
    uint16_t numVertexBuffers;        //!< Number of used vertex buffers.
    uint16_t numVertexLayouts;        //!< Number of used vertex layouts.

    int64_t textureMemoryUsed; //!< Estimate of texture memory used.
    int64_t rtMemoryUsed;      //!< Estimate of render target memory used.
    int32_t transientVbUsed;   //!< Amount of transient vertex buffer used.
    int32_t transientIbUsed;   //!< Amount of transient index buffer used.

    uint32_t numPrims[Topology::Count]; //!< Number of primitives rendered.

    int64_t gpuMemoryMax;  //!< Maximum available GPU memory for application.
    int64_t gpuMemoryUsed; //!< Amount of GPU memory used by the application.

    uint16_t width;      //!< Backbuffer width in pixels.
    uint16_t height;     //!< Backbuffer height in pixels.
    uint16_t textWidth;  //!< Debug text width in characters.
    uint16_t textHeight; //!< Debug text height in characters.

    uint16_t numViews;    //!< Number of view stats.
    ViewStats *viewStats; //!< Array of View stats.

    uint8_t numEncoders;        //!< Number of encoders used during frame.
    EncoderStats *encoderStats; //!< Array of encoder stats.
};

/// Encoders are used for submitting draw calls from multiple threads. Only one encoder
/// per thread should be used. Use `bgfx::begin()` to obtain an encoder for a thread.
///
/// @attention C99 equivalent is `bgfx_encoder`.
///
struct Encoder
{
    /// Sets a debug marker. This allows you to group
    /// graphics calls together for easy browsing in
    /// graphics debugging tools.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_marker`.
    ///
    void setMarker(const char *_marker);

    /// Set render states for draw primitive.
    ///
    /// @param[in] _state State flags. Default state for primitive type is
    ///   triangles. See: `BGFX_STATE_DEFAULT`.
    ///   - `BGFX_STATE_DEPTH_TEST_*` - Depth test function.
    ///   - `BGFX_STATE_BLEND_*` - See remark 1 about BGFX_STATE_BLEND_FUNC.
    ///   - `BGFX_STATE_BLEND_EQUATION_*` - See remark 2.
    ///   - `BGFX_STATE_CULL_*` - Backface culling mode.
    ///   - `BGFX_STATE_WRITE_*` - Enable R, G, B, A or Z write.
    ///   - `BGFX_STATE_MSAA` - Enable hardware multisample antialiasing.
    ///   - `BGFX_STATE_PT_[TRISTRIP/LINES/POINTS]` - Primitive type.
    ///
    /// @param[in] _rgba Sets blend factor used by `BGFX_STATE_BLEND_FACTOR` and
    ///   `BGFX_STATE_BLEND_INV_FACTOR` blend modes.
    ///
    /// @remarks
    ///   1. To setup more complex states use:
    ///      `BGFX_STATE_ALPHA_REF(_ref)`,
    ///      `BGFX_STATE_POINT_SIZE(_size)`,
    ///      `BGFX_STATE_BLEND_FUNC(_src, _dst)`,
    ///      `BGFX_STATE_BLEND_FUNC_SEPARATE(_srcRGB, _dstRGB, _srcA, _dstA)`,
    ///      `BGFX_STATE_BLEND_EQUATION(_equation)`,
    ///      `BGFX_STATE_BLEND_EQUATION_SEPARATE(_equationRGB, _equationA)`
    ///   2. `BGFX_STATE_BLEND_EQUATION_ADD` is set when no other blend
    ///      equation is specified.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_state`.
    ///
    void setState(uint64_t _state, uint32_t _rgba = 0);

    /// Set condition for rendering.
    ///
    /// @param[in] _handle Occlusion query handle.
    /// @param[in] _visible Render if occlusion query is visible.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_condition`.
    ///
    void setCondition(OcclusionQueryHandle _handle, bool _visible);


    /// Set stencil test state.
    ///
    /// @param[in] _fstencil Front stencil state.
    /// @param[in] _bstencil Back stencil state. If back is set to `BGFX_STENCIL_NONE`
    ///   _fstencil is applied to both front and back facing primitives.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_stencil`.
    ///
    void setStencil(uint32_t _fstencil, uint32_t _bstencil = BGFX_STENCIL_NONE);

    /// Set scissor for draw primitive. To scissor for all primitives in
    /// view see `bgfx::setViewScissor`.
    ///
    /// @param[in] _x Position x from the left side of the window.
    /// @param[in] _y Position y from the top of the window.
    /// @param[in] _width Width of scissor region.
    /// @param[in] _height Height of scissor region.
    /// @returns Scissor cache index.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_scissor`.
    ///
    uint16_t setScissor(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height);

    /// Set scissor from cache for draw primitive.
    ///
    /// @param[in] _cache Index in scissor cache.
    ///   Pass UINT16_MAX to have primitive use view scissor instead.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_scissor_cached`.
    ///
    void setScissor(uint16_t _cache = UINT16_MAX);

    /// Set model matrix for draw primitive. If it is not called, model will
    /// be rendered with identity model matrix.
    ///
    /// @param[in] _mtx Pointer to first matrix in array.
    /// @param[in] _num Number of matrices in array.
    /// @returns Index into matrix cache in case the same model matrix has
    ///   to be used for other draw primitive call.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_transform`.
    ///
    uint32_t setTransform(const void *_mtx, uint16_t _num = 1);

    /// Reserve `_num` matrices in internal matrix cache.
    ///
    /// @param[in] _transform Pointer to `Transform` structure.
    /// @param[in] _num Number of matrices.
    /// @returns Index into matrix cache.
    ///
    /// @attention Pointer returned can be modifed until `bgfx::frame` is called.
    /// @attention C99 equivalent is `bgfx_encoder_alloc_transform`.
    ///
    uint32_t allocTransform(Transform *_transform, uint16_t _num);

    /// Set model matrix from matrix cache for draw primitive.
    ///
    /// @param[in] _cache Index in matrix cache.
    /// @param[in] _num Number of matrices from cache.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_transform_cached`.
    ///
    void setTransform(uint32_t _cache, uint16_t _num = 1);

    /// Set shader uniform parameter for draw primitive.
    ///
    /// @param[in] _handle Uniform.
    /// @param[in] _value Pointer to uniform data.
    /// @param[in] _num Number of elements. Passing `UINT16_MAX` will
    ///   use the _num passed on uniform creation.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_uniform`.
    ///
    void setUniform(UniformHandle _handle, const void *_value, uint16_t _num = 1);

    /// Set index buffer for draw primitive.
    ///
    /// @param[in] _handle Index buffer.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_index_buffer`.
    ///
    void setIndexBuffer(IndexBufferHandle _handle);

    /// Set index buffer for draw primitive.
    ///
    /// @param[in] _handle Index buffer.
    /// @param[in] _firstIndex First index to render.
    /// @param[in] _numIndices Number of indices to render.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_index_buffer`.
    ///
    void setIndexBuffer(IndexBufferHandle _handle, uint32_t _firstIndex, uint32_t _numIndices);

    /// Set index buffer for draw primitive.
    ///
    /// @param[in] _handle Dynamic index buffer.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_dynamic_index_buffer`.
    ///
    void setIndexBuffer(DynamicIndexBufferHandle _handle);

    /// Set index buffer for draw primitive.
    ///
    /// @param[in] _handle Dynamic index buffer.
    /// @param[in] _firstIndex First index to render.
    /// @param[in] _numIndices Number of indices to render.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_dynamic_index_buffer`.
    ///
    void setIndexBuffer(DynamicIndexBufferHandle _handle, uint32_t _firstIndex, uint32_t _numIndices);

    /// Set index buffer for draw primitive.
    ///
    /// @param[in] _tib Transient index buffer.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_transient_index_buffer`.
    ///
    void setIndexBuffer(const TransientIndexBuffer *_tib);

    /// Set index buffer for draw primitive.
    ///
    /// @param[in] _tib Transient index buffer.
    /// @param[in] _firstIndex First index to render.
    /// @param[in] _numIndices Number of indices to render.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_transient_index_buffer`.
    ///
    void setIndexBuffer(const TransientIndexBuffer *_tib, uint32_t _firstIndex, uint32_t _numIndices);

    /// Set vertex buffer for draw primitive.
    ///
    /// @param[in] _stream Vertex stream.
    /// @param[in] _handle Vertex buffer.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_vertex_buffer`.
    ///
    void setVertexBuffer(uint8_t _stream, VertexBufferHandle _handle);

    /// Set vertex buffer for draw primitive.
    ///
    /// @param[in] _stream Vertex stream.
    /// @param[in] _handle Vertex buffer.
    /// @param[in] _startVertex First vertex to render.
    /// @param[in] _numVertices Number of vertices to render.
    /// @param[in] _layoutHandle Vertex layout for aliasing vertex buffer. If invalid handle is
    ///   used, vertex layout used for creation of vertex buffer will be used.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_vertex_buffer`.
    ///
    void setVertexBuffer(uint8_t _stream, VertexBufferHandle _handle, uint32_t _startVertex, uint32_t _numVertices,
                         VertexLayoutHandle _layoutHandle = BGFX_INVALID_HANDLE);

    /// Set vertex buffer for draw primitive.
    ///
    /// @param[in] _stream Vertex stream.
    /// @param[in] _handle Dynamic vertex buffer.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_dynamic_vertex_buffer`.
    ///
    void setVertexBuffer(uint8_t _stream, DynamicVertexBufferHandle _handle);

    /// Set vertex buffer for draw primitive.
    ///
    /// @param[in] _stream Vertex stream.
    /// @param[in] _handle Dynamic vertex buffer.
    /// @param[in] _startVertex First vertex to render.
    /// @param[in] _numVertices Number of vertices to render.
    /// @param[in] _layoutHandle Vertex layout for aliasing vertex buffer. If invalid handle is
    ///   used, vertex layout used for creation of vertex buffer will be used.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_dynamic_vertex_buffer`.
    ///
    void setVertexBuffer(uint8_t _stream, DynamicVertexBufferHandle _handle, uint32_t _startVertex,
                         uint32_t _numVertices, VertexLayoutHandle _layoutHandle = BGFX_INVALID_HANDLE);

    /// Set vertex buffer for draw primitive.
    ///
    /// @param[in] _stream Vertex stream.
    /// @param[in] _tvb Transient vertex buffer.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_transient_vertex_buffer`.
    ///
    void setVertexBuffer(uint8_t _stream, const TransientVertexBuffer *_tvb);

    /// Set vertex buffer for draw primitive.
    ///
    /// @param[in] _stream Vertex stream.
    /// @param[in] _tvb Transient vertex buffer.
    /// @param[in] _startVertex First vertex to render.
    /// @param[in] _numVertices Number of vertices to render.
    /// @param[in] _layoutHandle Vertex layout for aliasing vertex buffer. If invalid handle is
    ///   used, vertex layout used for creation of vertex buffer will be used.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_transient_vertex_buffer`.
    ///
    void setVertexBuffer(uint8_t _stream, const TransientVertexBuffer *_tvb, uint32_t _startVertex,
                         uint32_t _numVertices, VertexLayoutHandle _layoutHandle = BGFX_INVALID_HANDLE);

    /// Set number of vertices for auto generated vertices use in conjuction
    /// with gl_VertexID.
    ///
    /// @param[in] _numVertices Number of vertices.
    ///
    /// @attention Availability depends on: `BGFX_CAPS_VERTEX_ID`.
    /// @attention C99 equivalent is `bgfx_encoder_set_vertex_count`.
    ///
    void setVertexCount(uint32_t _numVertices);

    /// Set instance data buffer for draw primitive.
    ///
    /// @param[in] _idb Transient instance data buffer.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_instance_data_buffer`.
    ///
    void setInstanceDataBuffer(const InstanceDataBuffer *_idb);

    /// Set instance data buffer for draw primitive.
    ///
    /// @param[in] _idb Transient instance data buffer.
    /// @param[in] _start First instance data.
    /// @param[in] _num Number of data instances.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_instance_data_buffer`.
    ///
    void setInstanceDataBuffer(const InstanceDataBuffer *_idb, uint32_t _start, uint32_t _num);

    /// Set instance data buffer for draw primitive.
    ///
    /// @param[in] _handle Vertex buffer.
    /// @param[in] _start First instance data.
    /// @param[in] _num Number of data instances.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_instance_data_from_vertex_buffer`.
    ///
    void setInstanceDataBuffer(VertexBufferHandle _handle, uint32_t _start, uint32_t _num);

    /// Set instance data buffer for draw primitive.
    ///
    /// @param[in] _handle Vertex buffer.
    /// @param[in] _start First instance data.
    /// @param[in] _num Number of data instances.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_instance_data_from_dynamic_vertex_buffer`.
    ///
    void setInstanceDataBuffer(DynamicVertexBufferHandle _handle, uint32_t _start, uint32_t _num);

    /// Set number of instances for auto generated instances use in conjuction
    /// with gl_InstanceID.
    ///
    /// @param[in] _numInstances Number of instances.
    ///
    /// @attention Availability depends on: `BGFX_CAPS_VERTEX_ID`.
    /// @attention C99 equivalent is `bgfx_encoder_set_instance_count`.
    ///
    void setInstanceCount(uint32_t _numInstances);

    /// Set texture stage for draw primitive.
    ///
    /// @param[in] _stage Texture unit.
    /// @param[in] _sampler Program sampler.
    /// @param[in] _handle Texture handle.
    /// @param[in] _flags Texture sampling mode. Default value UINT32_MAX uses
    ///   texture sampling settings from the texture.
    ///   - `BGFX_SAMPLER_[U/V/W]_[MIRROR/CLAMP]` - Mirror or clamp to edge wrap
    ///     mode.
    ///   - `BGFX_SAMPLER_[MIN/MAG/MIP]_[POINT/ANISOTROPIC]` - Point or anisotropic
    ///     sampling.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_texture`.
    ///
    void setTexture(uint8_t _stage, UniformHandle _sampler, TextureHandle _handle, uint32_t _flags = UINT32_MAX);

    /// Submit an empty primitive for rendering. Uniforms and draw state
    /// will be applied but no geometry will be submitted. Useful in cases
    /// when no other draw/compute primitive is submitted to view, but it's
    /// desired to execute clear view.
    ///
    /// These empty draw calls will sort before ordinary draw calls.
    ///
    /// @param[in] _id View id.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_touch`.
    ///
    void touch(ViewId _id);

    /// Submit primitive for rendering.
    ///
    /// @param[in] _id View id.
    /// @param[in] _program Program.
    /// @param[in] _depth Depth for sorting.
    /// @param[in] _flags Discard or preserve states. See `BGFX_DISCARD_*`.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_submit`.
    ///
    void submit(ViewId _id, ProgramHandle _program, uint32_t _depth = 0, uint8_t _flags = BGFX_DISCARD_ALL);

    /// Submit primitive with occlusion query for rendering.
    ///
    /// @param[in] _id View id.
    /// @param[in] _program Program.
    /// @param[in] _occlusionQuery Occlusion query.
    /// @param[in] _depth Depth for sorting.
    /// @param[in] _flags Discard or preserve states. See `BGFX_DISCARD_*`.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_submit_occlusion_query`.
    ///
    void submit(ViewId _id, ProgramHandle _program, OcclusionQueryHandle _occlusionQuery, uint32_t _depth = 0,
                uint8_t _flags = BGFX_DISCARD_ALL);

    /// Submit primitive for rendering with index and instance data info from
    /// indirect buffer.
    ///
    /// @param[in] _id View id.
    /// @param[in] _program Program.
    /// @param[in] _indirectHandle Indirect buffer.
    /// @param[in] _start First element in indirect buffer.
    /// @param[in] _num Number of dispatches.
    /// @param[in] _depth Depth for sorting.
    /// @param[in] _flags Discard or preserve states. See `BGFX_DISCARD_*`.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_submit_indirect`.
    ///
    void submit(ViewId _id, ProgramHandle _program, IndirectBufferHandle _indirectHandle, uint16_t _start = 0,
                uint16_t _num = 1, uint32_t _depth = 0, uint8_t _flags = BGFX_DISCARD_ALL);

    /// Set compute index buffer.
    ///
    /// @param[in] _stage Compute stage.
    /// @param[in] _handle Index buffer handle.
    /// @param[in] _access Buffer access. See `Access::Enum`.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_compute_index_buffer`.
    ///
    void setBuffer(uint8_t _stage, IndexBufferHandle _handle, Access::Enum _access);

    /// Set compute vertex buffer.
    ///
    /// @param[in] _stage Compute stage.
    /// @param[in] _handle Vertex buffer handle.
    /// @param[in] _access Buffer access. See `Access::Enum`.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_compute_vertex_buffer`.
    ///
    void setBuffer(uint8_t _stage, VertexBufferHandle _handle, Access::Enum _access);

    /// Set compute dynamic index buffer.
    ///
    /// @param[in] _stage Compute stage.
    /// @param[in] _handle Dynamic index buffer handle.
    /// @param[in] _access Buffer access. See `Access::Enum`.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_compute_dynamic_index_buffer`.
    ///
    void setBuffer(uint8_t _stage, DynamicIndexBufferHandle _handle, Access::Enum _access);

    /// Set compute dynamic vertex buffer.
    ///
    /// @param[in] _stage Compute stage.
    /// @param[in] _handle Dynamic vertex buffer handle.
    /// @param[in] _access Buffer access. See `Access::Enum`.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_compute_dynamic_vertex_buffer`.
    ///
    void setBuffer(uint8_t _stage, DynamicVertexBufferHandle _handle, Access::Enum _access);

    /// Set compute indirect buffer.
    ///
    /// @param[in] _stage Compute stage.
    /// @param[in] _handle Indirect buffer handle.
    /// @param[in] _access Buffer access. See `Access::Enum`.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_compute_indirect_buffer`.
    ///
    void setBuffer(uint8_t _stage, IndirectBufferHandle _handle, Access::Enum _access);

    /// Set compute image from texture.
    ///
    /// @param[in] _stage Texture unit.
    /// @param[in] _handle Texture handle.
    /// @param[in] _mip Mip level.
    /// @param[in] _access Texture access. See `Access::Enum`.
    /// @param[in] _format Texture format. See: `TextureFormat::Enum`.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_set_image`.
    ///
    void setImage(uint8_t _stage, TextureHandle _handle, uint8_t _mip, Access::Enum _access,
                  TextureFormat::Enum _format = TextureFormat::Count);

    /// Dispatch compute.
    ///
    /// @param[in] _id View id.
    /// @param[in] _handle Compute program.
    /// @param[in] _numX Number of groups X.
    /// @param[in] _numY Number of groups Y.
    /// @param[in] _numZ Number of groups Z.
    /// @param[in] _flags Discard or preserve states. See `BGFX_DISCARD_*`.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_dispatch`.
    ///
    void dispatch(ViewId _id, ProgramHandle _handle, uint32_t _numX = 1, uint32_t _numY = 1, uint32_t _numZ = 1,
                  uint8_t _flags = BGFX_DISCARD_ALL);

    /// Dispatch compute indirect.
    ///
    /// @param[in] _id View id.
    /// @param[in] _handle Compute program.
    /// @param[in] _indirectHandle Indirect buffer.
    /// @param[in] _start First element in indirect buffer.
    /// @param[in] _num Number of dispatches.
    /// @param[in] _flags Discard or preserve states. See `BGFX_DISCARD_*`.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_dispatch_indirect`.
    ///
    void dispatch(ViewId _id, ProgramHandle _handle, IndirectBufferHandle _indirectHandle, uint16_t _start = 0,
                  uint16_t _num = 1, uint8_t _flags = BGFX_DISCARD_ALL);

    /// Discard all previously set state for draw or compute call.
    ///
    /// @param[in] _flags Draw/compute states to discard.
    ///
    /// @attention C99 equivalent is `bgfx_encoder_discard`.
    ///
    void discard(uint8_t _flags = BGFX_DISCARD_ALL);

    /// Blit texture 2D region between two 2D textures.
    ///
    /// @param[in] _id View id.
    /// @param[in] _dst Destination texture handle.
    /// @param[in] _dstX Destination texture X position.
    /// @param[in] _dstY Destination texture Y position.
    /// @param[in] _src Source texture handle.
    /// @param[in] _srcX Source texture X position.
    /// @param[in] _srcY Source texture Y position.
    /// @param[in] _width Width of region.
    /// @param[in] _height Height of region.
    ///
    /// @attention Destination texture must be created with `BGFX_TEXTURE_BLIT_DST` flag.
    /// @attention Availability depends on: `BGFX_CAPS_TEXTURE_BLIT`.
    /// @attention C99 equivalent is `bgfx_encoder_blit`.
    ///
    void blit(ViewId _id, TextureHandle _dst, uint16_t _dstX, uint16_t _dstY, TextureHandle _src, uint16_t _srcX = 0,
              uint16_t _srcY = 0, uint16_t _width = UINT16_MAX, uint16_t _height = UINT16_MAX);

    /// Blit texture region between two textures.
    ///
    /// @param[in] _id View id.
    /// @param[in] _dst Destination texture handle.
    /// @param[in] _dstMip Destination texture mip level.
    /// @param[in] _dstX Destination texture X position.
    /// @param[in] _dstY Destination texture Y position.
    /// @param[in] _dstZ If texture is 2D this argument should be 0. If destination texture is cube
    ///   this argument represents destination texture cube face. For 3D texture this argument
    ///   represents destination texture Z position.
    /// @param[in] _src Source texture handle.
    /// @param[in] _srcMip Source texture mip level.
    /// @param[in] _srcX Source texture X position.
    /// @param[in] _srcY Source texture Y position.
    /// @param[in] _srcZ If texture is 2D this argument should be 0. If source texture is cube
    ///   this argument represents source texture cube face. For 3D texture this argument
    ///   represents source texture Z position.
    /// @param[in] _width Width of region.
    /// @param[in] _height Height of region.
    /// @param[in] _depth If texture is 3D this argument represents depth of region, otherwise it's
    ///   unused.
    ///
    /// @attention Destination texture must be created with `BGFX_TEXTURE_BLIT_DST` flag.
    /// @attention Availability depends on: `BGFX_CAPS_TEXTURE_BLIT`.
    /// @attention C99 equivalent is `bgfx_encoder_blit`.
    ///
    void blit(ViewId _id, TextureHandle _dst, uint8_t _dstMip, uint16_t _dstX, uint16_t _dstY, uint16_t _dstZ,
              TextureHandle _src, uint8_t _srcMip = 0, uint16_t _srcX = 0, uint16_t _srcY = 0, uint16_t _srcZ = 0,
              uint16_t _width = UINT16_MAX, uint16_t _height = UINT16_MAX, uint16_t _depth = UINT16_MAX);
};

} // namespace bfgx