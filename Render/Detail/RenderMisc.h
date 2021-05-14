

#pragma once

#include "Render/Detail/RenderRequirement.h


constexpr bool isShaderType(uint32_t _magic, char _type)
{
    return uint32_t(_type) == (_magic & BX_MAKEFOURCC(0xff, 0, 0, 0));
}

inline bool isShaderBin(uint32_t _magic)
{
    return BX_MAKEFOURCC(0, 'S', 'H', 0) == (_magic & BX_MAKEFOURCC(0, 0xff, 0xff, 0)) &&
           (isShaderType(_magic, 'C') || isShaderType(_magic, 'F') || isShaderType(_magic, 'V'));
}

inline bool isShaderVerLess(uint32_t _magic, uint8_t _version)
{
    return (_magic & BX_MAKEFOURCC(0, 0, 0, 0xff)) < BX_MAKEFOURCC(0, 0, 0, _version);
}

const char *getShaderTypeName(uint32_t _magic);
