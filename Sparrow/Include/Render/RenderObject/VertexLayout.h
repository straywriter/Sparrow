#pragma once
#include "Render/RenderObject.h"

#include <stdint.h>
#include <string >

namespace Render
{

enum class EVertexAttributeType
{
    INT, FLOAT
};

class VertexAttribute
{
  public:
    std::string name;
    uint8_t location;
    uint8_t size;
    EVertexAttributeType type;

    uint8_t stride;
    uint8_t offset;
};

class VertexLayout : RenderObject
{
  public:
    VertexAttribute *data;
    uint8_t size;
};

} // namespace Render