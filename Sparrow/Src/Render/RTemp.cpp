#include "Render/RTemp.h"

namespace bgfx
{
bool windowsVersionIs(Condition::Enum _op, uint32_t _version)
{
#if BX_PLATFORM_WINDOWS
    static const uint8_t s_condition[] = {
        VER_LESS_EQUAL,
        VER_GREATER_EQUAL,
    };

    OSVERSIONINFOEXA ovi;
    bx::memSet(&ovi, 0, sizeof(ovi));
    ovi.dwOSVersionInfoSize = sizeof(ovi);
    // _WIN32_WINNT_WINBLUE 0x0603
    // _WIN32_WINNT_WIN8    0x0602
    // _WIN32_WINNT_WIN7    0x0601
    // _WIN32_WINNT_VISTA   0x0600
    ovi.dwMajorVersion = HIBYTE(_version);
    ovi.dwMinorVersion = LOBYTE(_version);
    DWORDLONG cond = 0;
    VER_SET_CONDITION(cond, VER_MAJORVERSION, s_condition[_op]);
    VER_SET_CONDITION(cond, VER_MINORVERSION, s_condition[_op]);
    return !!VerifyVersionInfoA(&ovi, VER_MAJORVERSION | VER_MINORVERSION, cond);
#else
    BX_UNUSED(_op, _version);
    return false;
#endif // BX_PLATFORM_WINDOWS
}

const char *getShaderTypeName(uint32_t _magic)
{
    if (isShaderType(_magic, 'C'))
    {
        return "Compute";
    }
    else if (isShaderType(_magic, 'F'))
    {
        return "Fragment";
    }
    else if (isShaderType(_magic, 'V'))
    {
        return "Vertex";
    }

    BX_ASSERT(false, "Invalid shader type!");

    return NULL;
}

const char *getName(TextureFormat::Enum _fmt)
{
    return bimg::getName(bimg::TextureFormat::Enum(_fmt));
}

const char *getName(UniformHandle _handle)
{
    return s_ctx->m_uniformRef[_handle.idx].m_name.getPtr();
}

const char *getName(ShaderHandle _handle)
{
    return s_ctx->m_shaderRef[_handle.idx].m_name.getPtr();
}

extern const char *s_topologyName[];
const char *getName(Topology::Enum _topology)
{
    return s_topologyName[bx::min(_topology, Topology::PointList)];
}

TextureFormat::Enum getViableTextureFormat(const bimg::ImageContainer &_imageContainer)
{
    const uint32_t formatCaps = g_caps.formats[_imageContainer.m_format];
    bool convert = 0 == formatCaps;

    if (_imageContainer.m_cubeMap)
    {
        convert |= 0 == (formatCaps & BGFX_CAPS_FORMAT_TEXTURE_CUBE) &&
                   0 != (formatCaps & BGFX_CAPS_FORMAT_TEXTURE_CUBE_EMULATED);
    }
    else if (_imageContainer.m_depth > 1)
    {
        convert |=
            0 == (formatCaps & BGFX_CAPS_FORMAT_TEXTURE_3D) && 0 != (formatCaps & BGFX_CAPS_FORMAT_TEXTURE_3D_EMULATED);
    }
    else
    {
        convert |=
            0 == (formatCaps & BGFX_CAPS_FORMAT_TEXTURE_2D) && 0 != (formatCaps & BGFX_CAPS_FORMAT_TEXTURE_2D_EMULATED);
    }

    if (convert)
    {
        return TextureFormat::BGRA8;
    }

    return TextureFormat::Enum(_imageContainer.m_format);
}
bool isGraphicsDebuggerPresent()
{
    return s_graphicsDebuggerPresent;
}

void dump(const VertexLayout &_layout)
{
    if (BX_ENABLED(BGFX_CONFIG_DEBUG))
    {
        BX_TRACE("VertexLayout %08x (%08x), stride %d", _layout.m_hash,
                 bx::hash<bx::HashMurmur2A>(_layout.m_attributes), _layout.m_stride);

        for (uint32_t attr = 0; attr < Attrib::Count; ++attr)
        {
            if (UINT16_MAX != _layout.m_attributes[attr])
            {
                uint8_t num;
                AttribType::Enum type;
                bool normalized;
                bool asInt;
                _layout.decode(Attrib::Enum(attr), num, type, normalized, asInt);

                BX_TRACE("\tattr %2d: %-20s num %d, type %d, norm [%c], asint [%c], offset %2d", attr,
                         getAttribName(Attrib::Enum(attr)), num, type, normalized ? 'x' : ' ', asInt ? 'x' : ' ',
                         _layout.m_offset[attr]);
            }
        }
    }
}

void setGraphicsDebuggerPresent(bool _present)
{
    BX_TRACE("Graphics debugger is %spresent.", _present ? "" : "not ");
    s_graphicsDebuggerPresent = _present;
}

const char *s_uniformTypeName[] = {
    "sampler1", NULL, "vec4", "mat3", "mat4",
};
BX_STATIC_ASSERT(UniformType::Count == BX_COUNTOF(s_uniformTypeName));

const char *getUniformTypeName(UniformType::Enum _enum)
{
    BX_ASSERT(_enum < UniformType::Count, "%d < UniformType::Count %d", _enum, UniformType::Count);
    return s_uniformTypeName[_enum];
}

UniformType::Enum nameToUniformTypeEnum(const char *_name)
{
    for (uint32_t ii = 0; ii < UniformType::Count; ++ii)
    {
        if (NULL != s_uniformTypeName[ii] && 0 == bx::strCmp(_name, s_uniformTypeName[ii]))
        {
            return UniformType::Enum(ii);
        }
    }

    return UniformType::Count;
}

static const char *s_predefinedName[PredefinedUniform::Count] = {
    "u_viewRect", "u_viewTexel",   "u_view",  "u_invView",   "u_proj",          "u_invProj",
    "u_viewProj", "u_invViewProj", "u_model", "u_modelView", "u_modelViewProj", "u_alphaRef4",
};

const char *getPredefinedUniformName(PredefinedUniform::Enum _enum)
{
    return s_predefinedName[_enum];
}

PredefinedUniform::Enum nameToPredefinedUniformEnum(const char *_name)
{
    for (uint32_t ii = 0; ii < PredefinedUniform::Count; ++ii)
    {
        if (0 == bx::strCmp(_name, s_predefinedName[ii]))
        {
            return PredefinedUniform::Enum(ii);
        }
    }

    return PredefinedUniform::Count;
}

} // namespace bgfx