#pragma once

#include "Render/RenderConfig.h"
#include "RenderHandle.h"


namespace bgfx
{

#if BX_COMPILER_CLANG_ANALYZER
void __attribute__((analyzer_noreturn)) fatal(Fatal::Enum _code, const char *_format, ...);
#else
void fatal(const char *_filePath, uint16_t _line, Fatal::Enum _code, const char *_format, ...);
#endif // BX_COMPILER_CLANG_ANALYZER

void trace(const char *_filePath, uint16_t _line, const char *_format, ...);


}