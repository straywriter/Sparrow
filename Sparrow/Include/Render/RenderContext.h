#pragma once

#include <iostream>

#include <stdint.h>
#include "Render/IndexBuffer.h"
#include "Render/RenderObject.h"
#include "Render/Shader.h"
#include "Render/VertexBuffer.h"
#include "Render/Program.h"
namespace Render
{

/**
 *
 *
 */
class RenderContext
{
  public:
    enum class HRI
    {
        OpenGL,
        OpenGLES,
        Vulkan,
        DirectX11,
        DirectX12,
        Metal
    };

    // member filed



    virtual bool Init(){std::cout<<"render context cout\n"; return true;};
    virtual bool Shutdown(){return true;};

    // debug
    // render

    virtual VertexBuffer* CreateVertexBuffer(void *data, uint32_t size)
    {
return nullptr;
    }
    virtual void UpdateVertexBuffer(VertexBuffer *inputObject, void *data, uint32_t offset, uint32_t size){}
    virtual void DestoryVertexBuffer(VertexBuffer *inputObject){}

    virtual IndexBuffer *CreateIndexBuffer(void *data, uint32_t size){return nullptr;}
    virtual void UpdateIndexBuffer(IndexBuffer *inputObject, void *date, uint32_t offset, uint32_t size){}
    virtual void DestoryIndexBuffer(IndexBuffer *inputObject){}

    virtual Shader *CreateShader(EShaderType shaderType, std::string data){return nullptr;}
    virtual void DestoryShader(Shader *inputObject){}


virtual Program* CreateProgram (Shader * vert,Shader * frag){return nullptr;}

virtual Program* DestoryProgram(Program* inputObject){return nullptr;}




    // virtual RenderObject CreateFrameBuffer();

    // virtual RenderObject CreateUniform();
    // // get uniform

    // virtual RenderObject CreateTextur2D();

    // virtual RenderObject CreatePipline();

    // virtual RenderObject CreatePass();

    virtual void Render(){}
};

} // namespace Render
