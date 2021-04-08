#include "GPU/OpenGL/GLRenderBuffer.h"

using namespace Render;


void GLRenderBuffer::Create()
{
    GL_CHECK(glGenRenderbuffers(1, &id);)
}

void GLRenderBuffer::Delete()
{
GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, id);)
}


void GLRenderBuffer::Bind()
{
GL_CHECK( glBindRenderbuffer(GL_RENDERBUFFER, id);)

}

void GLRenderBuffer::UnBind()
{

GL_CHECK( glBindRenderbuffer(GL_RENDERBUFFER, 0);)

}