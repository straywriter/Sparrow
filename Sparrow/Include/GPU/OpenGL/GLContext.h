#pragma once

#include "Render/RenderContext.h"

#include "Render/IndexBuffer.h"
#include "Render/VertexBuffer.h"

#include "Render/Shader.h"

#include "GPU/OpenGL/GLIndexBuffer.h"
#include "GPU/OpenGL/GLProgram.h "
#include "GPU/OpenGL/GLRequirement.h"
#include "GPU/OpenGL/GLShader.h"
#include "GPU/OpenGL/GLVertexArray.h"
#include "GPU/OpenGL/GLVertexBuffer.h"

#include <vector>
namespace Render
{

class GLContext : public RenderContext
{
  public:
    std::vector<GLVertexBuffer> vertexBuffers;
    std::vector<GLIndexBuffer> indexBuffers;
    std::vector<GLShader> shaders;
    std::vector<GLProgram> programs;
    GLVertexArray vertexArray;

    int majorVersion;
    int minorVersion;

    GLContext();
    bool Init() override;

    bool Shutdown() override;
    // std::Vector

    VertexBuffer *CreateVertexBuffer(void *data, uint32_t size) override;
    //   VertexBuffer* CreateVertexBuffer(void *data, uint32_t size) override;
    void UpdateVertexBuffer(VertexBuffer *inputObject, void *data, uint32_t offset, uint32_t size) override;
    void DestoryVertexBuffer(VertexBuffer *inputObject) override;

    IndexBuffer *CreateIndexBuffer(void *data, uint32_t size);
    void UpdateIndexBuffer(IndexBuffer *inputObject, void *date, uint32_t offset, uint32_t size);
    void DestoryIndexBuffer(IndexBuffer *inputObject);

    Program *CreateProgram(Shader *vert, Shader *frag);

    Program *DestoryProgram(Program *inputObject);

    Shader *CreateShader(EShaderType shaderType, std::string data) override;
    void DestoryShader(Shader *inputObject) override;

    void Render() override;
    void aa();
};

} // namespace Render