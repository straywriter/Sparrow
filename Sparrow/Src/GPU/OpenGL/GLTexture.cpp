#include "GPU/OpenGL/GLTexture.h"

using namespace Render;

void GLTexture::Create()
{
    GL_CHECK(glGenTextures(1, &id);)
}

void GLTexture::Delete()
{
    GL_CHECK(glDeleteTextures(1, &id);)
}

void GLTexture::SetData(Target _target, char *pixels)
{

    // assert bind

    // GL_CHECK(glTexImage2D();)
}

void GLTexture::Bind()
{
    // assert

    GL_CHECK(glBindTexture(target, id);)
}
void GLTexture::UnBind()
{

    GL_CHECK(glBindTexture(target, 0);)
}


void GLTexture::SetParameter(Parameters param,ParamValue value)
{

// GL_CHECK( glTexParameteri(static_cast<GLenum>(target), static_cast<GLenum>(param), static_cast<GLint>(value)));



    
}