#pragma once
#include "Render/Program.h"

#include "GPU/OpenGL/GLRequirement.h"
#include "GPU/OpenGL/GLShader.h"

namespace Render
{

class GLProgram:Program
{
    public:
    GLuint id;



void Create(GLShader vert,GLShader frag);

void Attach(GLShader vertexShader,GLShader fragShader);

void Link();

void Delete();

void Use();

}; 



}