
#pragma once

#include "Render/Detail/RenderEnum.h"

namespace Render
{

struct VertexLayout
{
    VertexLayout();

    /// Start VertexLayout.
    ///
    /// @attention C99 equivalent is `bgfx_vertex_layout_begin`.
    ///
    VertexLayout &begin(RendererType::Enum _renderer = RendererType::Noop);

    /// End VertexLayout.
    ///
    /// @attention C99 equivalent is `bgfx_vertex_layout_end`.
    ///
    void end();

    /// Add attribute to VertexLayout.
    ///
    /// @param[in] _attrib Attribute semantics. See: `bgfx::Attrib`
    /// @param[in] _num Number of elements 1, 2, 3 or 4.
    /// @param[in] _type Element type.
    /// @param[in] _normalized When using fixed point AttribType (f.e. Uint8)
    ///   value will be normalized for vertex shader usage. When normalized
    ///   is set to true, AttribType::Uint8 value in range 0-255 will be
    ///   in range 0.0-1.0 in vertex shader.
    /// @param[in] _asInt Packaging rule for vertexPack, vertexUnpack, and
    ///   vertexConvert for AttribType::Uint8 and AttribType::Int16.
    ///   Unpacking code must be implemented inside vertex shader.
    ///
    /// @remarks
    ///   Must be called between begin/end.
    ///
    /// @attention C99 equivalent is `bgfx_vertex_layout_add`.
    ///
    VertexLayout &add(Attrib::Enum _attrib, uint8_t _num, AttribType::Enum _type, bool _normalized = false,
                      bool _asInt = false);

    /// Skip _num bytes in vertex stream.
    ///
    /// @attention C99 equivalent is `bgfx_vertex_layout_skip`.
    ///
    VertexLayout &skip(uint8_t _num);

    /// Decode attribute.
    ///
    /// @attention C99 equivalent is `bgfx_vertex_layout_decode`.
    ///
    void decode(Attrib::Enum _attrib, uint8_t &_num, AttribType::Enum &_type, bool &_normalized, bool &_asInt) const;

    /// Returns true if VertexLayout contains attribute.
    ///
    /// @attention C99 equivalent is `bgfx_vertex_layout_has`.
    ///
    bool has(Attrib::Enum _attrib) const
    {
        return UINT16_MAX != m_attributes[_attrib];
    }

    /// Returns relative attribute offset from the vertex.
    uint16_t getOffset(Attrib::Enum _attrib) const
    {
        return m_offset[_attrib];
    }

    /// Returns vertex stride.
    uint16_t getStride() const
    {
        return m_stride;
    }

    /// Returns size of vertex buffer for number of vertices.
    uint32_t getSize(uint32_t _num) const
    {
        return _num * m_stride;
    }

    uint32_t m_hash;
    uint16_t m_stride;
    uint16_t m_offset[Attrib::Count];
    uint16_t m_attributes[Attrib::Count];
};

///
void initAttribTypeSizeTable(RendererType::Enum _type);

/// Returns attribute name.
const char *getAttribName(Attrib::Enum _attr);

///
const char *getAttribNameShort(Attrib::Enum _attr);

///
Attrib::Enum idToAttrib(uint16_t id);

///
uint16_t attribToId(Attrib::Enum _attr);

///
AttribType::Enum idToAttribType(uint16_t id);

///
int32_t write(bx::WriterI *_writer, const bgfx::VertexLayout &_layout, bx::Error *_err = NULL);

///
int32_t read(bx::ReaderI *_reader, bgfx::VertexLayout &_layout, bx::Error *_err = NULL);

///
uint32_t weldVertices(void *_output, const VertexLayout &_layout, const void *_data, uint32_t _num, bool _index32,
                      float _epsilon, bx::AllocatorI *_allocator);

} // namespace Render
