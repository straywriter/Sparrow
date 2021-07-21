
#include <bx/allocator.h>
#include <bx/bx.h>
#include <bx/cpu.h>
#include <bx/debug.h>
#include <bx/endian.h>
#include <bx/float4x4_t.h>
#include <bx/handlealloc.h>
#include <bx/hash.h>
#include <bx/math.h>
#include <bx/mutex.h>
#include <bx/os.h>
#include <bx/readerwriter.h>
#include <bx/ringbuffer.h>
#include <bx/sort.h>
#include <bx/string.h>
#include <bx/thread.h>
#include <bx/timer.h>
#include <bx/uint32_t.h>

#include "RenderDebug.h"
#include "CallbackI.h"


namespace bgfx
{
extern CallbackI *g_callback;

void fatal(const char *_filePath, uint16_t _line, Fatal::Enum _code, const char *_format, ...)
{
    va_list argList;
    va_start(argList, _format);

    char temp[8192];
    char *out = temp;
    int32_t len = bx::vsnprintf(out, sizeof(temp), _format, argList);
    if ((int32_t)sizeof(temp) < len)
    {
        out = (char *)alloca(len + 1);
        len = bx::vsnprintf(out, len, _format, argList);
    }
    out[len] = '\0';

    if (BX_UNLIKELY(NULL == g_callback))
    {
        bx::debugPrintf("%s(%d): BGFX 0x%08x: %s", _filePath, _line, _code, out);
        abort();
    }
    else
    {
        g_callback->fatal(_filePath, _line, _code, out);
    }

    va_end(argList);
}

void trace(const char *_filePath, uint16_t _line, const char *_format, ...)
{
    va_list argList;
    va_start(argList, _format);

    if (BX_UNLIKELY(NULL == g_callback))
    {
        bx::debugPrintfVargs(_format, argList);
    }
    else
    {
        g_callback->traceVargs(_filePath, _line, _format, argList);
    }

    va_end(argList);
}

} // namespace bgfx