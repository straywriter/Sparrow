#pragma once

#include "Render/RenderCommand.h"

class RenderCommandBuffer
{
  public:
    // data
    


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