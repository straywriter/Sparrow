#pragma once
#include "GPU/OpenGL/GLProgram.h"
using namespace Render;

#include <iostream>

void GLProgram::Create(GLShader vert,GLShader frag)

{

this->id=glCreateProgram();

Attach(vert,frag);

Link();
int success;

char infoLog[512];
 glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }


}

void GLProgram::Attach(GLShader vert,GLShader frag)
{

glAttachShader(id,vert.id);
glAttachShader(id,frag.id);

}

void GLProgram::Link()
{
    glLinkProgram(id);
}

void GLProgram::Delete()
{
glDeleteProgram(id);

}

void GLProgram::Use()
{
glUseProgram(id);

}
