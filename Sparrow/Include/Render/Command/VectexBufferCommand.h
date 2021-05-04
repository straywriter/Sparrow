#pragma once

#include "Render/RenderCommand.h"
#include "Render/VertexBuffer.h"

namespace Render
{

namespace RenderCommand
{

class VertexBufferCommand : RenderCommand
{

  public:
    VertexBuffer *object;

  public:
    inline VertexBuffer *GetVertexBuffer() const
    {
        if (isSuccessed)
            return object;
        else
            return nullptr;
    }
};
} // namespace RenderCommand
} // namespace Render