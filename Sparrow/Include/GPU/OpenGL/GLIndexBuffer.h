#pragma once
#include "Render/IndexBuffer.h"

namespace Render
{
class GLIndexBuffer : IndexBuffer
{
    public:

    GLuint id;
    uint32_t size;

    // TODO: size
    void Create(void *data, uint32_t size);
    void Update();
    void Delete();

    void Bind();
    void UnBind();

    friend bool operator==( const GLIndexBuffer & right,const GLIndexBuffer& left)
    {
        if(left.id==right.id)
        return true;
        else
        return false;
    }
};

} // namespace Render