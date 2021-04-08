#include "GPU/OpenGL/GLIndexBuffer.h"

using namespace Render;

void GLIndexBuffer::Create(void *data, uint32_t size)
{

glGenBuffers(1,&id);

//asset
Bind();

   glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

UnBind();

}
void GLIndexBuffer::Update()
{}


void GLIndexBuffer::Delete()
{
 glDeleteBuffers(1, &id);
}

void GLIndexBuffer::Bind()
{
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);

}
void GLIndexBuffer::UnBind()
{
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}