
#include "Render/RenderConfig.h"

#include "dxgicommon.h"
#include "Callback.hpp"

namespace bgfx
{

extern const uint32_t paletteSrgb[];
extern const uint32_t paletteLinear[];


extern const char* s_colorSpaceStr[];

extern const DXGI_COLOR_SPACE_TYPE kDxgiLastColorSpace;

extern const uint8_t vga8x8[256*8];
extern const uint8_t vga8x16[256*16];

// extern CallbackStub*  s_callbackStub;
extern const char *s_topologyName[];

} // namespace bgfx