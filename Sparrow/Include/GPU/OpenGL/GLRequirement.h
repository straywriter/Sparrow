
#pragma once



#include <stdint.h>

#include "glad/glad.h"
#include <glad/glad.h>

#include "GLFW/glfw3.h"

#include "GPU/OpenGL/GLDebug.h"
#include <iostream>

// enum class GLType
// {
//     FLOAT = GL_FLOAT,
//     INT = GL_INT,
//     UINT = GL_UNSIGNED_INT,
//     BYTE = GL_BYTE,
//     UBYTE = GL_UNSIGNED_BYTE,
//     SHORT = GL_SHORT,
//     USHORT = GL_UNSIGNED_SHORT,
//     TRUE = GL_TRUE,
//     FALSE = GL_FALSE
// };

inline bool GLADInit()
{
    // get window
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        // failed to initialize glad
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

const inline char* GLGetErrorString(GLenum error)
{
    switch (error)
    {
    case GL_NO_ERROR:          return "No Error";
    case GL_INVALID_ENUM:      return "Invalid Enum";
    case GL_INVALID_VALUE:     return "Invalid Value";
    case GL_INVALID_OPERATION: return "Invalid Operation";
    case GL_INVALID_FRAMEBUFFER_OPERATION: return "Invalid Framebuffer Operation";
    case GL_OUT_OF_MEMORY:     return "Out of Memory";
    case GL_STACK_UNDERFLOW:   return "Stack Underflow";
    case GL_STACK_OVERFLOW:    return "Stack Overflow";
    case GL_CONTEXT_LOST:      return "Context Lost";
    default:                   return "Unknown Error";
    }
}