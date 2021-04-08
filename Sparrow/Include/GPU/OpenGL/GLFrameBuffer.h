
#pragma once
#include "GPU/OpenGL/GLRequirement.h"
#include "Render/FrameBuffer.h"

namespace Render

{

class GLFrameBuffer : FrameBuffer
{
  public:
    enum class Target : GLenum
    {
        FRAMEBUFFER = GL_FRAMEBUFFER,
        READ = GL_READ_FRAMEBUFFER,
        DRAW = GL_DRAW_FRAMEBUFFER
    };

    enum class Attachment:GLenum
    {
        NONE = GL_NONE,

        COLOR0 = GL_COLOR_ATTACHMENT0,
        COLOR1 = GL_COLOR_ATTACHMENT1,
        COLOR2 = GL_COLOR_ATTACHMENT2,
        COLOR3 = GL_COLOR_ATTACHMENT3,
        COLOR4 = GL_COLOR_ATTACHMENT4,
        COLOR5 = GL_COLOR_ATTACHMENT5,
        COLOR6 = GL_COLOR_ATTACHMENT6,
        COLOR7 = GL_COLOR_ATTACHMENT7,
        COLOR8 = GL_COLOR_ATTACHMENT8,
        COLOR9 = GL_COLOR_ATTACHMENT9,
        COLOR10 = GL_COLOR_ATTACHMENT10,
        COLOR11 = GL_COLOR_ATTACHMENT11,
        COLOR12 = GL_COLOR_ATTACHMENT12,
        COLOR13 = GL_COLOR_ATTACHMENT13,
        COLOR14 = GL_COLOR_ATTACHMENT14,
        COLOR15 = GL_COLOR_ATTACHMENT15,

        DEPTH = GL_DEPTH_ATTACHMENT,
        STENCIL = GL_STENCIL_ATTACHMENT
    };

  public:
    GLuint id;
    Target target;
    Attachment attachment;
    bool isBind;

  public:
    GLFrameBuffer();
    ~GLFrameBuffer();

    void Create();

    void Delete();

    void Bind();
    void UnBind();

    void SetTexture();
    void SetTexture2D();

    void SetReadBuffer(Attachment a);

void CheckError();

    //----------------

    // enum class Target
    // {
    //     DEFAULT = GL_FRAMEBUFFER,
    //     READ = GL_READ_FRAMEBUFFER,
    //     DRAW = GL_DRAW_FRAMEBUFFER
    // };

    // enum class Attachment
    // {
    //     NONE = GL_NONE,

    //     COLOR0 = GL_COLOR_ATTACHMENT0,
    //     COLOR1 = GL_COLOR_ATTACHMENT1,
    //     COLOR2 = GL_COLOR_ATTACHMENT2,
    //     COLOR3 = GL_COLOR_ATTACHMENT3,
    //     COLOR4 = GL_COLOR_ATTACHMENT4,
    //     COLOR5 = GL_COLOR_ATTACHMENT5,
    //     COLOR6 = GL_COLOR_ATTACHMENT6,
    //     COLOR7 = GL_COLOR_ATTACHMENT7,
    //     COLOR8 = GL_COLOR_ATTACHMENT8,
    //     COLOR9 = GL_COLOR_ATTACHMENT9,
    //     COLOR10 = GL_COLOR_ATTACHMENT10,
    //     COLOR11 = GL_COLOR_ATTACHMENT11,
    //     COLOR12 = GL_COLOR_ATTACHMENT12,
    //     COLOR13 = GL_COLOR_ATTACHMENT13,
    //     COLOR14 = GL_COLOR_ATTACHMENT14,
    //     COLOR15 = GL_COLOR_ATTACHMENT15,

    //     DEPTH = GL_DEPTH_ATTACHMENT,
    //     STENCIL = GL_STENCIL_ATTACHMENT
    // };
};

} // namespace  Render
