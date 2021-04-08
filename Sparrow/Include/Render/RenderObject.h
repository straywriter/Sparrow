
#pragma once

#include "Core/Object.h"

class RenderObject : public Object
{
  public:
    enum class ERenderObjectType
    {
        VertexBuffer,
        IndexBuffer,

        Uniform
    };


    
};