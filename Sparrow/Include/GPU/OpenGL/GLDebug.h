#pragma once
#include "GPU/OpenGL/GLRequirement.h"
#include "Core/Log.h"

#include <stdio.h>

#define GL_DEBUG

void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                     const void *userParam);

void GLDebugInit();

char const *glGetErrorString(GLenum const err) noexcept;

void GLCheckErrors(const char *filename, int line, char *funName);

#ifdef GL_DEBUG
#define _GL_CHECK(call, callBack) call callBack(__FILE__, __LINE__, #call);

#define GL_CHECK(call) _GL_CHECK(call, GLCheckErrors)

#else
#define _GL_CHECK(call, callBack) call
#define GL_CHECK(call) _GL_CHECK(call, GLCheckErrors)
#endif
