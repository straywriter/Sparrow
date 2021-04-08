#include "GPU/OpenGL/GLShader.h"

using namespace Render;
#include <iostream>
#include <string>

GLShader::GLShader()
{

}

void GLShader::Create(EShaderType shaderType, std::string data)
{

if(shaderType==EShaderType::VertexShader)
{
this->shaderType=GL_VERTEX_SHADER;
}
else if(shaderType==EShaderType::FragmentShader)
{
this->shaderType=GL_FRAGMENT_SHADER;
}



    this->id = glCreateShader(this->shaderType);
const char *c_str = data.c_str();
    glShaderSource(id, 1, &c_str, nullptr);

    glCompileShader(id);

    int isSuccess;
    char logInfo[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &isSuccess);
    if (!isSuccess)
    {
        glGetShaderInfoLog(id, 512, NULL, logInfo);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << logInfo << std::endl;
    }
}

void GLShader::Delete()
{
glDeleteShader(id);
}