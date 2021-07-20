

/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */
#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/allocator.h>
#include <bx/bx.h>
#include <bx/timer.h>
#include <iostream>


// External\bx\include\bx\readerwriter.h

#include <bx/readerwriter.h>
#include <stdio.h>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include "logo.h"
#include <GLFW/glfw3native.h>

#include "Cubes.hpp"

static bool s_showStats = false;

static void glfw_errorCallback(int error, const char *description)
{
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

static void glfw_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
        s_showStats = !s_showStats;
}

int main(int argc, char **argv)
{
    // Create a GLFW window without an OpenGL context.
    glfwSetErrorCallback(glfw_errorCallback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(1024, 768, "helloworld", nullptr, nullptr);
    if (!window)
        return 1;
    glfwSetKeyCallback(window, glfw_keyCallback);
    // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
    // Most graphics APIs must be used on the same thread that created the window.
    bgfx::renderFrame();
    // Initialize bgfx using the native window handle and window resolution.
    bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    init.platformData.ndt = glfwGetX11Display();
    init.platformData.nwh = (void *)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
    init.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
    init.platformData.nwh = glfwGetWin32Window(window);
#endif
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    init.resolution.width = (uint32_t)width;
    init.resolution.height = (uint32_t)height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    if (!bgfx::init(init))
    {
        std::cout << "asdfasd";
        return 1;
    }
    // Set view 0 to the same dimensions as the window and to clear the color buffer.
    // const bgfx::ViewId kClearView = 0;
    // bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
    // bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

    //--------------------------------------
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_debug;
    uint32_t m_reset;
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh[BX_COUNTOF(s_ptState)];
    bgfx::ProgramHandle m_program;
    int64_t m_timeOffset;
    int32_t m_pt=1;

    bool m_r=true;
    bool m_g=true;
    bool m_b=true;
    bool m_a=true;

    m_width = (uint32_t)width;
    m_height =  (uint32_t)height;
    m_debug = BGFX_DEBUG_NONE;
    m_reset = BGFX_RESET_VSYNC;
    // Enable debug text.
    bgfx::setDebug(m_debug);

    // Set view 0 clear state.
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303037ff, 1.0f, 0);

    // Create vertex stream declaration.
    PosColorVertex::init();

    extern bx::AllocatorI *getDefaultAllocator();
    bx::AllocatorI *g_allocator = getDefaultAllocator();

    s_fileReader = BX_NEW(g_allocator, FileReader);

    // Create static vertex buffer.
    m_vbh = bgfx::createVertexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), PosColorVertex::ms_layout);

    // Create static index buffer for triangle list rendering.
    m_ibh[0] = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

    // Create static index buffer for triangle strip rendering.
    m_ibh[1] = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeTriStrip, sizeof(s_cubeTriStrip)));

    // Create static index buffer for line list rendering.
    m_ibh[2] = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeLineList, sizeof(s_cubeLineList)));

    // Create static index buffer for line strip rendering.
    m_ibh[3] = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeLineStrip, sizeof(s_cubeLineStrip)));

    // Create static index buffer for point list rendering.
    m_ibh[4] = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubePoints, sizeof(s_cubePoints)));

    // Create program from shaders.
    // m_program = loadProgram("vs_cubes", "fs_cubes");

    m_timeOffset = bx::getHPCounter();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        // Handle window resize.
        int oldWidth = width, oldHeight = height;
        glfwGetWindowSize(window, &width, &height);
        // if (width != oldWidth || height != oldHeight)
        // {
        //     bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
        //     // bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
        // }
        // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view
        // 0.
        // bgfx::touch(kClearView);
        // bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);

	


        // float time = (float)((bx::getHPCounter() - m_timeOffset) / double(bx::getHPFrequency()));

        // const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
        // const bx::Vec3 eye = {0.0f, 0.0f, -35.0f};

        // // Set view and projection matrix for view 0.
        // {
        //     float view[16];
        //     bx::mtxLookAt(view, eye, at);

        //     float proj[16];
        //     bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
        //     bgfx::setViewTransform(0, view, proj);

        //     // Set view 0 default viewport.
        //     bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));
        // }

        // // This dummy draw call is here to make sure that view 0 is cleared
        // // if no other draw calls are submitted to view 0.
        // // bgfx::touch(0);

        // bgfx::IndexBufferHandle ibh = m_ibh[m_pt];
        // uint64_t state = 0 | (m_r ? BGFX_STATE_WRITE_R : 0) | (m_g ? BGFX_STATE_WRITE_G : 0) |
        //                  (m_b ? BGFX_STATE_WRITE_B : 0) | (m_a ? BGFX_STATE_WRITE_A : 0) | BGFX_STATE_WRITE_Z |
        //                  BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA | s_ptState[m_pt];

        // // Submit 11x11 cubes.
        // for (uint32_t yy = 0; yy < 11; ++yy)
        // {
        //     for (uint32_t xx = 0; xx < 11; ++xx)
        //     {
        //         float mtx[16];
        //         bx::mtxRotateXY(mtx, time + xx * 0.21f, time + yy * 0.37f);
        //         mtx[12] = -15.0f + float(xx) * 3.0f;
        //         mtx[13] = -15.0f + float(yy) * 3.0f;
        //         mtx[14] = 0.0f;

        //         // Set model matrix for rendering.
        //         bgfx::setTransform(mtx);

        //         // Set vertex and index buffer.
        //         bgfx::setVertexBuffer(0, m_vbh);
        //         bgfx::setIndexBuffer(ibh);

        //         // Set render states.
        //         bgfx::setState(state);

        //         // Submit primitive for rendering to view 0.
        //         bgfx::submit(0, m_program);
        //     }
        // }

        // // Use debug font to print information about this example.
        // bgfx::dbgTextClear();
        // bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(width / 2 / 8), 20) - 20, bx::max<uint16_t>(uint16_t(height / 2
        // / 16), 6) - 6, 40, 12, s_logo, 160); bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
        // bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI
        // \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too."); bgfx::dbgTextPrintf(80, 1,
        // 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m
        // \x1b[0m"); bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m
        // \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m"); const bgfx::Stats* stats = bgfx::getStats();
        // bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.",
        // stats->width, stats->height, stats->textWidth, stats->textHeight); Enable stats or debug text.
        // Advance to next frame. Process submitted rendering primitives.
        bgfx::frame();
    }

    bgfx::shutdown();
    glfwTerminate();
    return 0;
}
