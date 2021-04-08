
#include "GPU/OpenGL/GLVertexArray.h"
#include <iostream>
using namespace Render;

void GLVertexArray::Create()
{
  glGenVertexArrays(1, &id);
    //std::cout<<"vaid:"<<id<<'\n';
}

void GLVertexArray::Bind()
{
    glBindVertexArray(id);
}

void GLVertexArray::UnBind()
{

    glBindVertexArray(0);
}
