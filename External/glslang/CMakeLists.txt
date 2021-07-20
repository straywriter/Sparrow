# ----------------------------------------------------------
# glslang
# ----------------------------------------------------------
file(
  GLOB
  GLSLANG_SOURCES
  glslang/GenericCodeGen/*.cpp
  glslang/MachineIndependent/*.cpp
  glslang/MachineIndependent/preprocessor/*.cpp
  glslang/HLSL/*.cpp
  hlsl/*.cpp
  SPIRV/*.cpp
  OGLCompilersDLL/*.cpp)

if(WIN32)
  list(APPEND GLSLANG_SOURCES ./glslang/OSDependent/Windows/ossource.cpp)
else()
  list(APPEND GLSLANG_SOURCES ./glslang/OSDependent/Unix/ossource.cpp)
endif()

add_library(glslang STATIC EXCLUDE_FROM_ALL ${GLSLANG_SOURCES})

target_include_directories(glslang PUBLIC ./ ./glslang/Include ./glslang/Public)

target_link_libraries(glslang PUBLIC spirv-tools)

set_target_properties(glslang PROPERTIES FOLDER "External")

if(MSVC)
  target_compile_options(
    glslang
    PRIVATE "/wd4005"
            "/wd4065"
            "/wd4100"
            "/wd4127"
            "/wd4189"
            "/wd4244"
            "/wd4310"
            "/wd4389"
            "/wd4456"
            "/wd4457"
            "/wd4458"
            "/wd4702"
            "/wd4715"
            "/wd4838")
else()
  target_compile_options(
    glslang
    PRIVATE "-Wno-ignored-qualifiers"
            "-Wno-implicit-fallthrough"
            "-Wno-missing-field-initializers"
            "-Wno-reorder"
            "-Wno-return-type"
            "-Wno-shadow"
            "-Wno-sign-compare"
            "-Wno-switch"
            "-Wno-undef"
            "-Wno-unknown-pragmas"
            "-Wno-unused-function"
            "-Wno-unused-parameter"
            "-Wno-unused-variable"
            "-fno-strict-aliasing")
endif()

if(APPLE)
  target_compile_options(
    glslang PRIVATE "-Wno-c++11-extensions" "-Wno-unused-const-variable"
                    "-Wno-deprecated-register")
endif()

if(UNIX AND NOT APPLE)
  target_compile_options(glslang PRIVATE "-Wno-unused-but-set-variable")
endif()

target_compile_definitions(glslang PRIVATE ENABLE_OPT=1 ENABLE_HLSL=1)
