#pragma once
#include "Render/RenderConfig.h"
#include "RenderDefine.h"
#include "RenderEnum.h"

#include "Callback.hpp"
#include "dxgicommon.h"

#include "bx/allocator.h"

#include "embedded_shader.h"

namespace bgfx
{

extern const uint32_t paletteSrgb[];
extern const uint32_t paletteLinear[];

extern const char *s_colorSpaceStr[];

extern const DXGI_COLOR_SPACE_TYPE kDxgiLastColorSpace;

extern const uint8_t vga8x8[256 * 8];
extern const uint8_t vga8x16[256 * 16];

// extern CallbackStub*  s_callbackStub;
extern const char *s_topologyName[];

extern CallbackI *g_callback;
extern bx::AllocatorI *g_allocator;

extern const EmbeddedShader s_embeddedShaders[];


static const CapsFlags s_capsFlags[] = {
#define CAPS_FLAGS(_x)                                                                                                 \
    {                                                                                                                  \
        _x, #_x                                                                                                        \
    }
    CAPS_FLAGS(BGFX_CAPS_ALPHA_TO_COVERAGE),
    CAPS_FLAGS(BGFX_CAPS_BLEND_INDEPENDENT),
    CAPS_FLAGS(BGFX_CAPS_COMPUTE),
    CAPS_FLAGS(BGFX_CAPS_CONSERVATIVE_RASTER),
    CAPS_FLAGS(BGFX_CAPS_DRAW_INDIRECT),
    CAPS_FLAGS(BGFX_CAPS_FRAGMENT_DEPTH),
    CAPS_FLAGS(BGFX_CAPS_FRAGMENT_ORDERING),
    CAPS_FLAGS(BGFX_CAPS_GRAPHICS_DEBUGGER),
    CAPS_FLAGS(BGFX_CAPS_HDR10),
    CAPS_FLAGS(BGFX_CAPS_HIDPI),
    CAPS_FLAGS(BGFX_CAPS_INDEX32),
    CAPS_FLAGS(BGFX_CAPS_INSTANCING),
    CAPS_FLAGS(BGFX_CAPS_OCCLUSION_QUERY),
    CAPS_FLAGS(BGFX_CAPS_RENDERER_MULTITHREADED),
    CAPS_FLAGS(BGFX_CAPS_SWAP_CHAIN),
    CAPS_FLAGS(BGFX_CAPS_TEXTURE_2D_ARRAY),
    CAPS_FLAGS(BGFX_CAPS_TEXTURE_3D),
    CAPS_FLAGS(BGFX_CAPS_TEXTURE_BLIT),
    CAPS_FLAGS(BGFX_CAPS_TEXTURE_COMPARE_ALL),
    CAPS_FLAGS(BGFX_CAPS_TEXTURE_COMPARE_LEQUAL),
    CAPS_FLAGS(BGFX_CAPS_TEXTURE_CUBE_ARRAY),
    CAPS_FLAGS(BGFX_CAPS_TEXTURE_DIRECT_ACCESS),
    CAPS_FLAGS(BGFX_CAPS_TEXTURE_READ_BACK),
    CAPS_FLAGS(BGFX_CAPS_VERTEX_ATTRIB_HALF),
    CAPS_FLAGS(BGFX_CAPS_VERTEX_ATTRIB_UINT10),
    CAPS_FLAGS(BGFX_CAPS_VERTEX_ID),
#undef CAPS_FLAGS
};

static TextureFormat::Enum s_emulatedFormats[] = {
    TextureFormat::BC1,     TextureFormat::BC2,     TextureFormat::BC3,     TextureFormat::BC4,
    TextureFormat::BC5,     TextureFormat::ETC1,    TextureFormat::ETC2,    TextureFormat::ETC2A,
    TextureFormat::ETC2A1,  TextureFormat::PTC12,   TextureFormat::PTC14,   TextureFormat::PTC12A,
    TextureFormat::PTC14A,  TextureFormat::PTC22,   TextureFormat::PTC24,   TextureFormat::ATC,
    TextureFormat::ATCE,    TextureFormat::ATCI,    TextureFormat::ASTC4x4, TextureFormat::ASTC5x5,
    TextureFormat::ASTC6x6, TextureFormat::ASTC8x5, TextureFormat::ASTC8x6, TextureFormat::ASTC10x5,
    TextureFormat::BGRA8, // GL doesn't support BGRA8 without extensions.
    TextureFormat::RGBA8, // D3D9 doesn't support RGBA8
};

} // namespace bgfx