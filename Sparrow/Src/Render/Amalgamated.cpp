/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "bgfx.cpp"

#include "DebugRenderdoc.cpp"

// #include "dxgi.cpp"
#include "HRI/DirectX/DXGI.cpp"

// #include "glcontext_egl.cpp"
// #include "glcontext_glx.cpp"

#include "HRI/OpenGL/OpenGLEGL.cpp"
#include "HRI/OpenGL/OpenGLGLX.cpp"

#include "HRI/OpenGL/OpenGLWGL.cpp"
// #include "glcontext_html5.cpp"
#include "HRI/OpenGL/OpenGLHTML5.cpp"

#include "HRI/DirectX/NVIDIAAPI.cpp"

#include "HRI/DirectX/DirectX11/RenderDirectX11.cpp"
#include "HRI/DirectX/DirectX12/RenderDirectX12.cpp"
#include "HRI/DirectX/DirectX9/RenderDirectX9.cpp"

// #include "renderer_gl.cpp"
#include "HRI/OpenGL/RenderOpenGL.cpp"

#include "HRI/Other/renderer_gnm.cpp"
#include "HRI/Other/renderer_noop.cpp"
#include "HRI/Other/renderer_nvn.cpp"

#include "HRI/Web/RenderWebGPU.cpp"
#include "HRI/Vulkan/RenderVulkan.cpp"
// #include "renderer_webgpu.cpp"

#include "shader_dx9bc.cpp"
#include "shader_dxbc.cpp"
#include "shader_spirv.cpp"
#include "topology.cpp"
#include "vertexlayout.cpp"

#include "Encoder.cpp"
#include "IRenderContext.cpp"
#include "RenderContext.cpp"
#include "RenderFunction.cpp"
// #include "RenderVertexLayout.cpp"
#include "StaticVariable.cpp"

#include "EmbeddedShader.cpp"
#include "RenderDebug.cpp"
#include "RenderHandle.cpp"
#include "RenderObject.cpp"
#include "RenderPlatform.cpp"
#include "RenderStructInternal.cpp"

#include "RTemp.cpp"

// #include "Render/Help/Camera.cpp"
// #include "Render/Help/RenderHelp.cpp"
// // #include "Render/Help/Bounds.cpp"

// #include "Render/Help/Entry/cmd.cpp"
// // #include "Render/Help/Entry/dialog.cpp"
// // #include "Render/Help/Entry/dialog_darwin.mm"
// #include "Render/Help/Entry/entry.cpp"
// // #include "Render/Help/Entry/entry_android.cpp"
// #include "Render/Help/Entry/entry_glfw.cpp     "
// // #include "Render/Help/Entry/entry_html5.cpp    "
// // #include "Render/Help/Entry/entry_ios.mm       "
// // #include "Render/Help/Entry/entry_noop.cpp     "
// // #include "Render/Help/Entry/entry_osx.mm       "
// // #include "Render/Help/Entry/entry_sdl.cpp"
// #include "Render/Help/Entry/entry_windows.cpp"
// // #include "Render/Help/Entry/entry_x11.cpp      "
// #include "Render/Help/Entry/input.cpp"

// #include "Render/Help/ImGui.cpp"