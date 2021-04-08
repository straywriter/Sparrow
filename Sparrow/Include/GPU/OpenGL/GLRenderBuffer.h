#pragma once

#include "GPU/OpenGL/GLRequirement.h"

namespace Render
{

class GLRenderBuffer
{

  public:
    GLuint id;

    GLRenderBuffer(); ~GLRenderBuffer();

        void Create();
    void Delete();

    void Bind();
    void UnBind();

    void Storage(int wight, int hight, GLenum e)
    {

        GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLuint>(e), wight, hight);)
    }
};

} // namespace Render