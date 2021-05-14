

#ifndef BGFX_CONFIG_DEBUG
#define BGFX_CONFIG_DEBUG 1
#endif // BGFX_CONFIG_DEBUG

#if BGFX_CONFIG_DEBUG || BX_COMPILER_CLANG_ANALYZER
#define BX_TRACE _BX_TRACE
#define BX_WARN _BX_WARN
#define BX_ASSERT _BX_ASSERT
#define BX_CONFIG_ALLOCATOR_DEBUG 1
#endif // BGFX_CONFIG_DEBUG

#define _BX_TRACE(_format, ...)                                                                                        \
    BX_MACRO_BLOCK_BEGIN                                                                                               \
    bgfx::trace(__FILE__, uint16_t(__LINE__), "BGFX " _format "\n", ##__VA_ARGS__);                                    \
    BX_MACRO_BLOCK_END

#define _BX_WARN(_condition, _format, ...)                                                                             \
    BX_MACRO_BLOCK_BEGIN                                                                                               \
    if (!BX_IGNORE_C4127(_condition))                                                                                  \
    {                                                                                                                  \
        BX_TRACE("WARN " _format, ##__VA_ARGS__);                                                                      \
    }                                                                                                                  \
    BX_MACRO_BLOCK_END

#define _BX_ASSERT(_condition, _format, ...)                                                                           \
    BX_MACRO_BLOCK_BEGIN                                                                                               \
    if (!BX_IGNORE_C4127(_condition))                                                                                  \
    {                                                                                                                  \
        BX_TRACE("CHECK " _format, ##__VA_ARGS__);                                                                     \
        bgfx::fatal(__FILE__, uint16_t(__LINE__), bgfx::Fatal::DebugCheck, _format, ##__VA_ARGS__);                    \
    }                                                                                                                  \
    BX_MACRO_BLOCK_END

#define BGFX_FATAL(_condition, _err, _format, ...)                                                                     \
    BX_MACRO_BLOCK_BEGIN                                                                                               \
    if (!BX_IGNORE_C4127(_condition))                                                                                  \
    {                                                                                                                  \
        fatal(__FILE__, uint16_t(__LINE__), _err, _format, ##__VA_ARGS__);                                             \
    }                                                                                                                  \
    BX_MACRO_BLOCK_END


