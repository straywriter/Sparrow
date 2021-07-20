
#include "RenderHandle.h"

namespace bgfx
{
const char *getTypeName(Handle _handle)
{
    switch (_handle.type)
    {
    case Handle::IndexBuffer:
        return "IB";
    case Handle::Shader:
        return "S";
    case Handle::Texture:
        return "T";
    case Handle::VertexBuffer:
        return "VB";
    default:
        break;
    }

    BX_ASSERT(false, "You should not be here.");
    return "?";
}

} // namespace bgfx