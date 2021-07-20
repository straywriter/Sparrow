#pragma once

#include "Render/Render.h"
#include <bx/allocator.h>
#include <bx/bx.h>
#include <bx/debug.h>
#include <bx/file.h>
#include <bx/readerwriter.h>

#define DBG_STRINGIZE(_x) DBG_STRINGIZE_(_x)
#define DBG_STRINGIZE_(_x) #_x
#define DBG_FILE_LINE_LITERAL "" __FILE__ "(" DBG_STRINGIZE(__LINE__) "): "
#define DBG(_format, ...) bx::debugPrintf(DBG_FILE_LINE_LITERAL "" _format "\n", ##__VA_ARGS__)

static bx::FileReaderI *s_fileReader = NULL;
static bx::FileWriterI *s_fileWriter = NULL;

bx::AllocatorI *getDefaultAllocator()
{
    BX_PRAGMA_DIAGNOSTIC_PUSH();
    BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459); // warning C4459: declaration of 's_allocator' hides global declaration
    BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
    static bx::DefaultAllocator s_allocator;
    return &s_allocator;
    BX_PRAGMA_DIAGNOSTIC_POP();
}

extern bx::AllocatorI *getDefaultAllocator();
bx::AllocatorI *g_allocator = getDefaultAllocator();

typedef bx::StringT<&g_allocator> String;
static String s_currentDir;
class FileReader : public bx::FileReader
{
    typedef bx::FileReader super;

  public:
    virtual bool open(const bx::FilePath &_filePath, bx::Error *_err) override
    {
        String filePath(s_currentDir);
        filePath.append(_filePath);
        return super::open(filePath.getPtr(), _err);
    }
};

struct PosColorVertex
{
    float m_x;
    float m_y;
    float m_z;
    uint32_t m_abgr;

    static void init()
    {
        ms_layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    };

    static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex s_cubeVertices[] = {
    {-1.0f, 1.0f, 1.0f, 0xff000000},   {1.0f, 1.0f, 1.0f, 0xff0000ff},   {-1.0f, -1.0f, 1.0f, 0xff00ff00},
    {1.0f, -1.0f, 1.0f, 0xff00ffff},   {-1.0f, 1.0f, -1.0f, 0xffff0000}, {1.0f, 1.0f, -1.0f, 0xffff00ff},
    {-1.0f, -1.0f, -1.0f, 0xffffff00}, {1.0f, -1.0f, -1.0f, 0xffffffff},
};

static const uint16_t s_cubeTriList[] = {
    0, 1, 2,          // 0
    1, 3, 2, 4, 6, 5, // 2
    5, 6, 7, 0, 2, 4, // 4
    4, 2, 6, 1, 5, 3, // 6
    5, 7, 3, 0, 4, 1, // 8
    4, 5, 1, 2, 3, 6, // 10
    6, 3, 7,
};

static const uint16_t s_cubeTriStrip[] = {
    0, 1, 2, 3, 7, 1, 5, 0, 4, 2, 6, 7, 4, 5,
};

static const uint16_t s_cubeLineList[] = {
    0, 1, 0, 2, 0, 4, 1, 3, 1, 5, 2, 3, 2, 6, 3, 7, 4, 5, 4, 6, 5, 7, 6, 7,
};

static const uint16_t s_cubeLineStrip[] = {
    0, 2, 3, 1, 5, 7, 6, 4, 0, 2, 6, 4, 5, 7, 3, 1, 0,
};

static const uint16_t s_cubePoints[] = {0, 1, 2, 3, 4, 5, 6, 7};

static const char *s_ptNames[]{
    "Triangle List", "Triangle Strip", "Lines", "Line Strip", "Points",
};

static const uint64_t s_ptState[]{
    UINT64_C(0), BGFX_STATE_PT_TRISTRIP, BGFX_STATE_PT_LINES, BGFX_STATE_PT_LINESTRIP, BGFX_STATE_PT_POINTS,
};
BX_STATIC_ASSERT(BX_COUNTOF(s_ptState) == BX_COUNTOF(s_ptNames));

bx::FileReaderI *getFileReader()
{
    return s_fileReader;
}

static const bgfx::Memory *loadMem(bx::FileReaderI *_reader, const char *_filePath)
{
    if (bx::open(_reader, _filePath))
    {
        uint32_t size = (uint32_t)bx::getSize(_reader);
        const bgfx::Memory *mem = bgfx::alloc(size + 1);
        bx::read(_reader, mem->data, size);
        bx::close(_reader);
        mem->data[mem->size - 1] = '\0';
        return mem;
    }

    DBG("Failed to load %s.", _filePath);
    return NULL;
}

static void *loadMem(bx::FileReaderI *_reader, bx::AllocatorI *_allocator, const char *_filePath, uint32_t *_size)
{
    if (bx::open(_reader, _filePath))
    {
        uint32_t size = (uint32_t)bx::getSize(_reader);
        void *data = BX_ALLOC(_allocator, size);
        bx::read(_reader, data, size);
        bx::close(_reader);

        if (NULL != _size)
        {
            *_size = size;
        }
        return data;
    }

    DBG("Failed to load %s.", _filePath);
    return NULL;
}

static bgfx::ShaderHandle loadShader(bx::FileReaderI *_reader, const char *_name)
{
    char filePath[512];

    const char *shaderPath = "???";

    switch (bgfx::getRendererType())
    {
    case bgfx::RendererType::Noop:
    case bgfx::RendererType::Direct3D9:
        shaderPath = "shaders/dx9/";
        break;
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12:
        shaderPath = "shaders/dx11/";
        break;
    case bgfx::RendererType::Gnm:
        shaderPath = "shaders/pssl/";
        break;
    case bgfx::RendererType::Metal:
        shaderPath = "shaders/metal/";
        break;
    case bgfx::RendererType::Nvn:
        shaderPath = "shaders/nvn/";
        break;
    case bgfx::RendererType::OpenGL:
        shaderPath = "shaders/glsl/";
        break;
    case bgfx::RendererType::OpenGLES:
        shaderPath = "shaders/essl/";
        break;
    case bgfx::RendererType::Vulkan:
        shaderPath = "shaders/spirv/";
        break;
    case bgfx::RendererType::WebGPU:
        shaderPath = "shaders/spirv/";
        break;

    case bgfx::RendererType::Count:
        BX_ASSERT(false, "You should not be here!");
        break;
    }

    bx::strCopy(filePath, BX_COUNTOF(filePath), shaderPath);
    bx::strCat(filePath, BX_COUNTOF(filePath), _name);
    bx::strCat(filePath, BX_COUNTOF(filePath), ".bin");

    bgfx::ShaderHandle handle = bgfx::createShader(loadMem(_reader, filePath));
    bgfx::setName(handle, _name);

    return handle;
}

bgfx::ShaderHandle loadShader(const char *_name)
{
    return loadShader(getFileReader(), _name);
}

bgfx::ProgramHandle loadProgram(bx::FileReaderI *_reader, const char *_vsName, const char *_fsName)
{
    bgfx::ShaderHandle vsh = loadShader(_reader, _vsName);
    bgfx::ShaderHandle fsh = BGFX_INVALID_HANDLE;
    if (NULL != _fsName)
    {
        fsh = loadShader(_reader, _fsName);
    }

    return bgfx::createProgram(vsh, fsh, true /* destroy shaders when program is destroyed */);
}

bgfx::ProgramHandle loadProgram(const char *_vsName, const char *_fsName)
{
    return loadProgram(getFileReader(), _vsName, _fsName);
}
