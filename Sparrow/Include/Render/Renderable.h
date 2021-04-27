#pragma once

#include "Render/RenderCommandBuffer.h"

class Renderable
{
    public:
    bool isInit;


    public:
    virtual RenderCommandBuffer GetRenderCommandBuffer();


};
