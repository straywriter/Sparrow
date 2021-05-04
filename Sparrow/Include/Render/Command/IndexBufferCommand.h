#pragma once

#include "Render/IndexBuffer.h"
#include "Render/RenderCommand.h"


namespace Render
{

namespace RenderCommand
{

class IndexBufferCommand : RenderCommand
{
  public:
    // data
    IndexBuffer *object;

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