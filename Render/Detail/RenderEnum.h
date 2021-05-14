#pragma once

namespace Render
{

/// Fatal error enum.
///
/// @attention C99 equivalent is `bgfx_fatal_t`.
///
struct Fatal
{
    enum Enum
    {
        DebugCheck,
        InvalidShader,
        UnableToInitialize,
        UnableToCreateTexture,
        DeviceLost,

        Count
    };
};

/// Renderer backend type enum.
///
/// @attention C99 equivalent is `bgfx_renderer_type_t`.
///
struct RendererType
{
    /// Renderer types:
    enum Enum
    {
        Noop,       //!< No rendering.
        Direct3D9,  //!< Direct3D 9.0
        Direct3D11, //!< Direct3D 11.0
        Direct3D12, //!< Direct3D 12.0
        Gnm,        //!< GNM
        Metal,      //!< Metal
        Nvn,        //!< NVN
        OpenGLES,   //!< OpenGL ES 2.0+
        OpenGL,     //!< OpenGL 2.1+
        Vulkan,     //!< Vulkan
        WebGPU,     //!< WebGPU

        Count
    };
};

/// Access mode enum.
///
/// @attention C99 equivalent is `bgfx_access_t`.
///
struct Access
{
    /// Access:
    enum Enum
    {
        Read,      //!< Read
        Write,     //!< Write
        ReadWrite, //!< Read and write

        Count
    };
};

/// Vertex attribute enum.
///
/// @attention C99 equivalent is `bgfx_attrib_t`.
///
struct Attrib
{
    /// Corresponds to vertex shader attribute.
    enum Enum
    {
        Position,  //!< a_position
        Normal,    //!< a_normal
        Tangent,   //!< a_tangent
        Bitangent, //!< a_bitangent
        Color0,    //!< a_color0
        Color1,    //!< a_color1
        Color2,    //!< a_color2
        Color3,    //!< a_color3
        Indices,   //!< a_indices
        Weight,    //!< a_weight
        TexCoord0, //!< a_texcoord0
        TexCoord1, //!< a_texcoord1
        TexCoord2, //!< a_texcoord2
        TexCoord3, //!< a_texcoord3
        TexCoord4, //!< a_texcoord4
        TexCoord5, //!< a_texcoord5
        TexCoord6, //!< a_texcoord6
        TexCoord7, //!< a_texcoord7

        Count
    };
};

/// Vertex attribute type enum.
///
/// @attention C99 equivalent is `bgfx_attrib_type_t`.
///
struct AttribType
{
    /// Attribute types:
    enum Enum
    {
        Uint8,  //!< Uint8
        Uint10, //!< Uint10, availability depends on: `BGFX_CAPS_VERTEX_ATTRIB_UINT10`.
        Int16,  //!< Int16
        Half,   //!< Half, availability depends on: `BGFX_CAPS_VERTEX_ATTRIB_HALF`.
        Float,  //!< Float

        Count
    };
};

/// Texture format enum.
///
/// Notation:
///
///       RGBA16S
///       ^   ^ ^
///       |   | +-- [ ]Unorm
///       |   |     [F]loat
///       |   |     [S]norm
///       |   |     [I]nt
///       |   |     [U]int
///       |   +---- Number of bits per component
///       +-------- Components
///
/// @attention Availability depends on Caps (see: formats).
///
/// @attention C99 equivalent is `bgfx_texture_format_t`.
///
struct TextureFormat
{
    /// Texture formats:
    enum Enum
    {
        BC1,      //!< DXT1 R5G6B5A1
        BC2,      //!< DXT3 R5G6B5A4
        BC3,      //!< DXT5 R5G6B5A8
        BC4,      //!< LATC1/ATI1 R8
        BC5,      //!< LATC2/ATI2 RG8
        BC6H,     //!< BC6H RGB16F
        BC7,      //!< BC7 RGB 4-7 bits per color channel, 0-8 bits alpha
        ETC1,     //!< ETC1 RGB8
        ETC2,     //!< ETC2 RGB8
        ETC2A,    //!< ETC2 RGBA8
        ETC2A1,   //!< ETC2 RGB8A1
        PTC12,    //!< PVRTC1 RGB 2BPP
        PTC14,    //!< PVRTC1 RGB 4BPP
        PTC12A,   //!< PVRTC1 RGBA 2BPP
        PTC14A,   //!< PVRTC1 RGBA 4BPP
        PTC22,    //!< PVRTC2 RGBA 2BPP
        PTC24,    //!< PVRTC2 RGBA 4BPP
        ATC,      //!< ATC RGB 4BPP
        ATCE,     //!< ATCE RGBA 8 BPP explicit alpha
        ATCI,     //!< ATCI RGBA 8 BPP interpolated alpha
        ASTC4x4,  //!< ASTC 4x4 8.0 BPP
        ASTC5x5,  //!< ASTC 5x5 5.12 BPP
        ASTC6x6,  //!< ASTC 6x6 3.56 BPP
        ASTC8x5,  //!< ASTC 8x5 3.20 BPP
        ASTC8x6,  //!< ASTC 8x6 2.67 BPP
        ASTC10x5, //!< ASTC 10x5 2.56 BPP

        Unknown, // Compressed formats above.

        R1,
        A8,
        R8,
        R8I,
        R8U,
        R8S,
        R16,
        R16I,
        R16U,
        R16F,
        R16S,
        R32I,
        R32U,
        R32F,
        RG8,
        RG8I,
        RG8U,
        RG8S,
        RG16,
        RG16I,
        RG16U,
        RG16F,
        RG16S,
        RG32I,
        RG32U,
        RG32F,
        RGB8,
        RGB8I,
        RGB8U,
        RGB8S,
        RGB9E5F,
        BGRA8,
        RGBA8,
        RGBA8I,
        RGBA8U,
        RGBA8S,
        RGBA16,
        RGBA16I,
        RGBA16U,
        RGBA16F,
        RGBA16S,
        RGBA32I,
        RGBA32U,
        RGBA32F,
        R5G6B5,
        RGBA4,
        RGB5A1,
        RGB10A2,
        RG11B10F,

        UnknownDepth, // Depth formats below.

        D16,
        D24,
        D24S8,
        D32,
        D16F,
        D24F,
        D32F,
        D0S8,

        Count
    };
};

/// Uniform type enum.
///
/// @attention C99 equivalent is `bgfx_uniform_type_t`.
///
struct UniformType
{
    /// Uniform types:
    enum Enum
    {
        Sampler, //!< Sampler.
        End,     //!< Reserved, do not use.

        Vec4, //!< 4 floats vector.
        Mat3, //!< 3x3 matrix.
        Mat4, //!< 4x4 matrix.

        Count
    };
};

/// Backbuffer ratio enum.
///
/// @attention C99 equivalent is `bgfx_backbuffer_ratio_t`.
///
struct BackbufferRatio
{
    /// Backbuffer ratios:
    enum Enum
    {
        Equal,     //!< Equal to backbuffer.
        Half,      //!< One half size of backbuffer.
        Quarter,   //!< One quarter size of backbuffer.
        Eighth,    //!< One eighth size of backbuffer.
        Sixteenth, //!< One sixteenth size of backbuffer.
        Double,    //!< Double size of backbuffer.

        Count
    };
};

/// Occlusion query result.
///
/// @attention C99 equivalent is `bgfx_occlusion_query_result_t`.
///
struct OcclusionQueryResult
{
    /// Occlusion query results:
    enum Enum
    {
        Invisible, //!< Query failed test.
        Visible,   //!< Query passed test.
        NoResult,  //!< Query result is not available yet.

        Count
    };
};

/// Primitive topology.
///
/// @attention C99 equivalent is `bgfx_topology_t`.
///
struct Topology
{
    /// Primitive topology:
    enum Enum
    {
        TriList,   //!< Triangle list.
        TriStrip,  //!< Triangle strip.
        LineList,  //!< Line list.
        LineStrip, //!< Line strip.
        PointList, //!< Point list.

        Count
    };
};

/// Topology conversion function.
///
/// @attention C99 equivalent is `bgfx_topology_convert_t`.
///
struct TopologyConvert
{
    /// Topology conversion functions:
    enum Enum
    {
        TriListFlipWinding,  //!< Flip winding order of triangle list.
        TriStripFlipWinding, //!< Flip winding order of trinagle strip.
        TriListToLineList,   //!< Convert triangle list to line list.
        TriStripToTriList,   //!< Convert triangle strip to triangle list.
        LineStripToLineList, //!< Convert line strip to line list.

        Count
    };
};

/// Topology sort order.
///
/// @attention C99 equivalent is `bgfx_topology_sort_t`.
///
struct TopologySort
{
    /// Topology sort order:
    enum Enum
    {
        DirectionFrontToBackMin, //!<
        DirectionFrontToBackAvg, //!<
        DirectionFrontToBackMax, //!<
        DirectionBackToFrontMin, //!<
        DirectionBackToFrontAvg, //!<
        DirectionBackToFrontMax, //!<
        DistanceFrontToBackMin,  //!<
        DistanceFrontToBackAvg,  //!<
        DistanceFrontToBackMax,  //!<
        DistanceBackToFrontMin,  //!<
        DistanceBackToFrontAvg,  //!<
        DistanceBackToFrontMax,  //!<

        Count
    };
};

/// View mode sets draw call sort order.
///
/// @attention C99 equivalent is `bgfx_view_mode_t`.
///
struct ViewMode
{
    /// View modes:
    enum Enum
    {
        Default,         //!< Default sort order.
        Sequential,      //!< Sort in the same order in which submit calls were called.
        DepthAscending,  //!< Sort draw call depth in ascending order.
        DepthDescending, //!< Sort draw call depth in descending order.

        Count
    };
};

} // namespace Render