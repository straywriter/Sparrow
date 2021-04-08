#include "GPU/OpenGL/GLFrameBuffer.h"

using namespace Render;

#include <assert.h>

void GLFrameBuffer::Create()
{
    GL_CHECK(glGenFramebuffers(1, &id);)
}

void GLFrameBuffer::Delete()
{
    // GL_CHECK(glDeleteFramebuffers(1, id);)
}

void GLFrameBuffer::Bind()
{

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, id);)
}

void GLFrameBuffer::UnBind()
{
    // glBindFramebuffer(glBindFramebuffer(static_cast<GLuint>(target), 0);, 0);
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0);)
}

void GLFrameBuffer::SetTexture()
{
    assert(isBind);
    // static_assert(isBind);

    // GL_CHECK(glFramebufferTexture(target, attachment, id, 0);)
}




void GLFrameBuffer::SetTexture2D()
{
    assert(isBind);

    // glFramebufferTexture2D(static_cast<GLuint>(_target), static_cast<GLenum>(attachment),
    //                        static_cast<GLenum>(texture.getTarget()), texture.getId(), 0);
}

// void GLFrameBuffer::SetRenderBuffer()
void GLFrameBuffer::CheckError()
{
    assert(isBind);

    GLenum ret = glCheckFramebufferStatus(static_cast<GLenum>(target));

    if (ret != GL_FRAMEBUFFER_COMPLETE)
    {
        std::string what;

        switch (ret)
        {
        case GL_FRAMEBUFFER_UNDEFINED:
            what = "Framebuffer undefined";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            what = "incomplete attachment";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            what = "incomplete missing attachment";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            what = "incomplete draw buffer";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            what = "incomplete read buffer";
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            what = "unsupported";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            what = "incomplete multisample";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            what = "incomplete layer targets";
            break;

        case GL_INVALID_ENUM:
            what = "Invalid Enum";
            break;

        default:
            what = "Unknown";
            break;
        }

        // throw Exception("Framebuffer Error: " + what);
    }
}