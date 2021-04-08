#pragma once

#include "Render/RenderCommand.h"
#include "Render/IndexBuffer.h"

namespace Render
{

namespace RenderCommand
{

class IndexBufferCommand : RenderCommand
{
  public:
    enum class ECreateIndex
    {
        Create,
        Update,
        Destory
    };

  public:
    // data
    ECreateIndex type;
    IndexBuffer *object;



};
} // namespace RenderCommand
} // namespace Render