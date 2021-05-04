#pragma once

#include "Render/RenderCommand.h"
#include <vector>
class RenderCommandBuffer
{
  public:
    // data
    void *data;
    size_t size;
    size_t lenth;
    std::vector<RenderCommand *> renderCommands;

  public:
    RenderCommandBuffer();
    RenderCommandBuffer(size_t size);

    ~RenderCommandBuffer();

    size_t GetMemerySize();

    void Run(RenderContext *);
    void Clear();

  private:
    void Resize();

  public:
    VertexBuffer *CreateVertexBuffer(void *data, uint32_t size);

    void UpdateVertexBuffer(VertexBuffer *object, void *data, uint32_t offset, uint32_t size);

    void DestoryVertexBuffer(RenderObject *object);

    IndexBuffer *CreateIndexBuffer(void *data, uint32_t size);

    void Draw(IndexBuffer *object);
    void Draw(VecterBuffer *object);
}