#pragma once
#include "Render/CallbackI.h"
#include "Render/Render.h"
#include "Render/RenderConfig.h"
#include "Render/StaticVariable.h"

namespace bgfx
{

struct CallbackStub : public CallbackI
{
    virtual ~CallbackStub()
    {
    }

    virtual void fatal(const char *_filePath, uint16_t _line, Fatal::Enum _code, const char *_str) override
    {
        if (Fatal::DebugCheck == _code)
        {
            bx::debugBreak();
        }
        else
        {
            bgfx::trace(_filePath, _line, "BGFX 0x%08x: %s\n", _code, _str);
            BX_UNUSED(_code, _str);
            abort();
        }
    }

    virtual void traceVargs(const char *_filePath, uint16_t _line, const char *_format, va_list _argList) override
    {
        char temp[2048];
        char *out = temp;
        va_list argListCopy;
        va_copy(argListCopy, _argList);
        int32_t len = bx::snprintf(out, sizeof(temp), "%s (%d): ", _filePath, _line);
        int32_t total = len + bx::vsnprintf(out + len, sizeof(temp) - len, _format, argListCopy);
        va_end(argListCopy);
        if ((int32_t)sizeof(temp) < total)
        {
            out = (char *)alloca(total + 1);
            bx::memCopy(out, temp, len);
            bx::vsnprintf(out + len, total - len, _format, _argList);
        }
        out[total] = '\0';
        bx::debugOutput(out);
    }

    virtual void profilerBegin(const char * /*_name*/, uint32_t /*_abgr*/, const char * /*_filePath*/,
                               uint16_t /*_line*/) override
    {
    }

    virtual void profilerBeginLiteral(const char * /*_name*/, uint32_t /*_abgr*/, const char * /*_filePath*/,
                                      uint16_t /*_line*/) override
    {
    }

    virtual void profilerEnd() override
    {
    }

    virtual uint32_t cacheReadSize(uint64_t /*_id*/) override
    {
        return 0;
    }

    virtual bool cacheRead(uint64_t /*_id*/, void * /*_data*/, uint32_t /*_size*/) override
    {
        return false;
    }

    virtual void cacheWrite(uint64_t /*_id*/, const void * /*_data*/, uint32_t /*_size*/) override
    {
    }

    virtual void screenShot(const char *_filePath, uint32_t _width, uint32_t _height, uint32_t _pitch,
                            const void *_data, uint32_t _size, bool _yflip) override
    {
        BX_UNUSED(_filePath, _width, _height, _pitch, _data, _size, _yflip);

        const int32_t len = bx::strLen(_filePath) + 5;
        char *filePath = (char *)alloca(len);
        bx::strCopy(filePath, len, _filePath);
        bx::strCat(filePath, len, ".tga");

        bx::FileWriter writer;
        if (bx::open(&writer, filePath))
        {
            bimg::imageWriteTga(&writer, _width, _height, _pitch, _data, false, _yflip);
            bx::close(&writer);
        }
    }

    virtual void captureBegin(uint32_t /*_width*/, uint32_t /*_height*/, uint32_t /*_pitch*/,
                              TextureFormat::Enum /*_format*/, bool /*_yflip*/) override
    {
        BX_TRACE("Warning: using capture without callback (a.k.a. pointless).");
    }

    virtual void captureEnd() override
    {
    }

    virtual void captureFrame(const void * /*_data*/, uint32_t /*_size*/) override
    {
    }
};

#ifndef BGFX_CONFIG_MEMORY_TRACKING
#define BGFX_CONFIG_MEMORY_TRACKING (BGFX_CONFIG_DEBUG && BX_CONFIG_SUPPORTS_THREADING)
#endif // BGFX_CONFIG_MEMORY_TRACKING

const size_t kNaturalAlignment = 8;

class AllocatorStub : public bx::AllocatorI
{
  public:
    AllocatorStub()
#if BGFX_CONFIG_MEMORY_TRACKING
        : m_numBlocks(0), m_maxBlocks(0)
#endif // BGFX_CONFIG_MEMORY_TRACKING
    {
    }

    virtual void *realloc(void *_ptr, size_t _size, size_t _align, const char *_file, uint32_t _line) override
    {
        if (0 == _size)
        {
            if (NULL != _ptr)
            {
                if (kNaturalAlignment >= _align)
                {
#if BGFX_CONFIG_MEMORY_TRACKING
                    {
                        bx::MutexScope scope(m_mutex);
                        BX_ASSERT(m_numBlocks > 0, "Number of blocks is 0. Possible alloc/free mismatch?");
                        --m_numBlocks;
                    }
#endif // BGFX_CONFIG_MEMORY_TRACKING

                    ::free(_ptr);
                }
                else
                {
                    bx::alignedFree(this, _ptr, _align, _file, _line);
                }
            }

            return NULL;
        }
        else if (NULL == _ptr)
        {
            if (kNaturalAlignment >= _align)
            {
#if BGFX_CONFIG_MEMORY_TRACKING
                {
                    bx::MutexScope scope(m_mutex);
                    ++m_numBlocks;
                    m_maxBlocks = bx::max(m_maxBlocks, m_numBlocks);
                }
#endif // BGFX_CONFIG_MEMORY_TRACKING

                return ::malloc(_size);
            }

            return bx::alignedAlloc(this, _size, _align, _file, _line);
        }

        if (kNaturalAlignment >= _align)
        {
#if BGFX_CONFIG_MEMORY_TRACKING
            if (NULL == _ptr)
            {
                bx::MutexScope scope(m_mutex);
                ++m_numBlocks;
                m_maxBlocks = bx::max(m_maxBlocks, m_numBlocks);
            }
#endif // BGFX_CONFIG_MEMORY_TRACKING

            return ::realloc(_ptr, _size);
        }

        return bx::alignedRealloc(this, _ptr, _size, _align, _file, _line);
    }

    void checkLeaks();

  protected:
#if BGFX_CONFIG_MEMORY_TRACKING
    bx::Mutex m_mutex;
    uint32_t m_numBlocks;
    uint32_t m_maxBlocks;
#endif // BGFX_CONFIG_MEMORY_TRACKING
};


extern CallbackStub*  s_callbackStub;

void AllocatorStub::checkLeaks()
	{
#if BGFX_CONFIG_MEMORY_TRACKING
		// BK - CallbackStub will be deleted after printing this info, so there is always one
		// leak if CallbackStub is used.
		BX_WARN(uint32_t(NULL != s_callbackStub ? 1 : 0) == m_numBlocks
			, "\n\n"
			  "\n########################################################"
			  "\n"
			  "\nMEMORY LEAK: Number of leaked blocks %d (Max blocks: %d)"
			  "\n"
			  "\n########################################################"
			  "\n\n"
			, m_numBlocks
			, m_maxBlocks
			);
#endif // BGFX_CONFIG_MEMORY_TRACKING
	}

} // namespace bgfx