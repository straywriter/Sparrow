#include "GPU/OpenGL/GLVertexBuffer.h"
using namespace Render;
#include <iostream>

GLVertexBuffer::GLVertexBuffer()
{
}

GLVertexBuffer::GLVertexBuffer(GLenum target, GLVertexLayout layout)
{
}

bool GLVertexBuffer::Create(void *data, uint32_t size, GLenum target)
{
    this->id = 0;
    glGenBuffers(1, &id);
    // this->target = target;

    //    if( this->id)
    //    {
    //        std::cout<<"chenggong"<<this->id;
    //    }
    // //    else
    //    {
    //    std::cout<<"hell"<<id;
    //    }
    // Bind();
    glBindBuffer(GL_ARRAY_BUFFER, id);

    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    UnBind();
    return true;
}

void GLVertexBuffer::Update(uint32_t offset, uint32_t size, void *data)
{
}

void GLVertexBuffer::Delete()
{

    glDeleteBuffers(1, &id);
}

void GLVertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void GLVertexBuffer::UnBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
