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
    enum class ECreateVertex
    {
        Create,
        Update,
        Destory
    };

  public:
    // data
    ECreateVertex type;
    // return
    VertexBuffer *object;

  public:
    VertexBuffer *GetVertexBuffer();
    ECreateVertex GetType();

    static VertexBufferCommand CreateVertexBuffer();
    static VertexBufferCommand UpdateVertexBuffer();
    static VertexBufferCommand DestoryVertexBuffer();
};
} // namespace RenderCommand
} // namespace Render