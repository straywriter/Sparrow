#pragma once
#include "GPU/OpenGL/GLRequirement.h"
#include "Render/Shader.h"

namespace Render
{
class GLShader : Shader
{
  public:
    GLuint id;
    GLenum shaderType;
GLShader();

    void Create(EShaderType shaderType, std::string data);

    void Delete();

    friend bool operator==(const GLShader &left, const GLShader &right)
    {
        if (left.id == right.id)
            return true;
        else
            return false;
    }
};

} // namespace Render