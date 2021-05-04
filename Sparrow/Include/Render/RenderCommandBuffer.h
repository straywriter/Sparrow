#pragma once

#include "Render/RenderCommand.h"

class RenderCommandBuffer
{
  public:
    // data

  public:
    void Run(RenderContext *);

  public:
    bool AddCommand();

  public:
    RenderObject *CreateVertexBuffer();
    RenderObject *CreateVertexBuffer()
    {
    }
    void UpdateVertexBuffer(RenderObject *object);
    void DestoryVertexBuffer(RenderObject *object);

    void Draw();
}