#include "GPU/OpenGL/GLContext.h"

#include "GPU/OpenGL/GLVertexBuffer.h"

#include <algorithm>
#include <iostream>
using namespace Render;

GLContext::GLContext()
{
}

bool Render::GLContext::Init()
{

    if (GLADInit())
    {
        vertexArray.Create();

        // GLDebugInit();

        return true;
    }
    else
        return false;
}

bool GLContext::Shutdown()
{
    return true;
}

VertexBuffer *GLContext::CreateVertexBuffer(void *data, uint32_t size)
{
    auto result = GLVertexBuffer();

    result.Create(data, size);

    vertexBuffers.push_back(result);
    auto p = vertexBuffers.data();
    auto temp = vertexBuffers.end();

    return (VertexBuffer *)(p + vertexBuffers.size() - 1);
}

void GLContext::UpdateVertexBuffer(VertexBuffer *inputObject, void *data, uint32_t offset, uint32_t size)
{
}
void GLContext::DestoryVertexBuffer(VertexBuffer *inputObject)
{
    GLVertexBuffer *vertexBufferObject = static_cast<GLVertexBuffer *>(inputObject);

    auto re = std::find(vertexBuffers.begin(), vertexBuffers.end(), *vertexBufferObject);

    re->Delete();

    vertexBuffers.erase(re);
}

IndexBuffer *GLContext::CreateIndexBuffer(void *data, uint32_t size)
{

    auto result = GLIndexBuffer();
    result.Create(data, size);
    indexBuffers.push_back(result);
    auto &temp = indexBuffers.back();
    return (IndexBuffer *)(&temp);
}

void GLContext::UpdateIndexBuffer(IndexBuffer *inputObject, void *date, uint32_t offset, uint32_t size)
{
}
void GLContext::DestoryIndexBuffer(IndexBuffer *inputObject)
{
    GLIndexBuffer *indexBufferObject = (GLIndexBuffer *)(inputObject);

    auto re = std::find(indexBuffers.begin(), indexBuffers.end(), *indexBufferObject);

    re->Delete();

    indexBuffers.erase(re);
}

Shader *GLContext::CreateShader(EShaderType shaderType, std::string data)
{

    auto result = GLShader();
    result.Create(shaderType, data);
    shaders.push_back(result);
    auto temp = shaders.end();

    auto p = shaders.data();


    return (Shader *)(p + shaders.size() - 1);
}
void GLContext::DestoryShader(Shader *inputObject)
{
    GLShader *glshader = static_cast<GLShader *>(inputObject);
    auto re = std::find(shaders.begin(), shaders.end(), *glshader);
    re->Delete();

    shaders.erase(re);
}

Program *GLContext::CreateProgram(Shader *vert, Shader *frag)
{
    auto prog = GLProgram();
    prog.Create(*(GLShader *)vert, *(GLShader *)frag);
    this->programs.push_back(prog);

    auto p = programs.data();

    return (Program *)(p + programs.size() - 1);
}

Program *GLContext::DestoryProgram(Program *inputObject)
{

    return nullptr;
}

void GLContext::aa()
{
    //    vertexArray.Create();
    vertexArray.Bind();

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers.front().id);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers.front().id);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    vertexArray.UnBind();
}

// vertex  index  shader program
void GLContext::Render()
{

    vertexArray.Create();
    vertexArray.Bind();

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers.front().id);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers.front().id);

    // vertexBuffers.front().Bind();
    // std::cout<<'\n'<<vertexBuffers.front().id;

    // indexBuffers.front().Bind();
    // std::cout<<'\n'<<indexBuffers.front().id;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    vertexArray.UnBind();
    // VAO
    programs.front().Use();

    vertexArray.Bind();

    // Program

    // gl Draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}