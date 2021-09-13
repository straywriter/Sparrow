project(Sparrow)

set(IncludeDir ${CMAKE_CURRENT_SOURCE_DIR}/Include)

# Base

# file(GLOB_RECURSE BaseIncludeFile
# ${CMAKE_CURRENT_SOURCE_DIR}/Include/Base/*.h) file(GLOB_RECURSE BaseSourceFile
# ${CMAKE_CURRENT_SOURCE_DIR}/Src/Base/*.cpp) add_library(Base)
# target_sources(Base PUBLIC ${BaseIncludeFile} ${BaseSourceFile})
# target_include_directories(Base PUBLIC ${IncludeDir})
# target_link_libraries(Base PUBLIC spdlog ) set_target_properties(Base
# PROPERTIES FOLDER "Sparrow/")

# Core
file(GLOB_RECURSE CoreIncludeFile ${CMAKE_CURRENT_SOURCE_DIR}/Include/Core/*.h)
file(GLOB_RECURSE CoreSourceFile ${CMAKE_CURRENT_SOURCE_DIR}/Src/Core/*.cpp)
add_library(Core)
target_sources(Core PUBLIC ${CoreIncludeFile} ${CoreSourceFile})
target_include_directories(Core PUBLIC ${IncludeDir})
target_link_libraries(Core PUBLIC spdlog)
set_target_properties(Core PROPERTIES FOLDER "Sparrow/")


# Core
file(GLOB_RECURSE BaseIncludeFile ${CMAKE_CURRENT_SOURCE_DIR}/Include/Base/*.h)
file(GLOB_RECURSE BaseSourceFile ${CMAKE_CURRENT_SOURCE_DIR}/Src/Base/*.cpp)
add_library(SparrowBase)
target_sources(SparrowBase PUBLIC ${BaseIncludeFile} ${BaseSourceFile})
target_include_directories(SparrowBase PUBLIC ${IncludeDir} ${CMAKE_CURRENT_SOURCE_DIR}/Inline)
target_link_libraries(SparrowBase PUBLIC spdlog folly)
set_target_properties(SparrowBase PROPERTIES FOLDER "Sparrow/")

# Render file(GLOB_RECURSE RenderIncludeFile
# ${CMAKE_CURRENT_SOURCE_DIR}/Include/Render/*.h) file(GLOB_RECURSE
# RenderSourceFile ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/*.cpp) message(STATUS
# "render file ${RenderIncludeFile}") message(STATUS "render file
# :${RenderSourceFile}") add_library(Render) target_include_directories(Render
# PUBLIC ${RenderIncludeDir}) target_sources(Render PUBLIC ${RenderIncludeFile}
# ${RenderSourceFile}) target_link_libraries(Render PUBLIC glfw glad)

# set_target_properties(Render PROPERTIES FOLDER "Sparrow")

# GPU OpenGL file(GLOB_RECURSE OpenGLIncludeFile
# ${CMAKE_CURRENT_SOURCE_DIR}/Include/GPU/OpenGL/*.h) file(GLOB_RECURSE
# OpenGLSourceFile ${CMAKE_CURRENT_SOURCE_DIR}/Src/GPU/OpenGL/*.cpp)
# message(STATUS "opengl file :${OpenGLIncludeFile}") message(STATUS "opengl
# file :${OpenGLSourceFile}") add_library(OpenGL) target_sources(OpenGL PUBLIC
# ${OpenGLIncludeFile} ${OpenGLSourceFile}) target_include_directories(OpenGL
# PUBLIC ${IncludeDir}) target_link_libraries(OpenGL PUBLIC glfw glad Core)
# set_target_properties(OpenGL PROPERTIES FOLDER "Sparrow/GPU")

# Render

# ----------------------------------------------------------------------------
# Render
# ----------------------------------------------------------------------------
set(RenderIncludeDir ${CMAKE_CURRENT_SOURCE_DIR}/Include)
set(RenderSourceDir ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render)

if(NOT APPLE)
  set(RenderAmalgamateSourceFile ${RenderSourceDir}/Amalgamated.cpp)
else()
  set(RenderAmalgamateSourceFile ${RenderSourceDir}/Amalgamated.mm)
endif()

# set(RenderSourceFile
# # "${RenderSourceDir}/Amalgamated.cpp"
# # "${RenderSourceDir}/Amalgamated.mm"
# "${RenderSourceDir}/bgfx.cpp"
# "${RenderSourceDir}/DebugRenderdoc.cpp"
# "${RenderSourceDir}/HRI/DirectX/DXGI.cpp"
# "${RenderSourceDir}/HRI/OpenGL/OpenGLEGL.cpp"
# "${RenderSourceDir}/HRI/OpenGL/OpenGLGLX.cpp"
# "${RenderSourceDir}/HRI/OpenGL/OpenGLWGL.cpp"
# "${RenderSourceDir}/HRI/OpenGL/OpenGLHTML5.cpp"
# "${RenderSourceDir}/HRI/DirectX/NVIDIAAPI.cpp"
# "${RenderSourceDir}/HRI/DirectX/DirectX11/RenderDirectX11.cpp"
# "${RenderSourceDir}/HRI/DirectX/DirectX12/RenderDirectX12.cpp"
# "${RenderSourceDir}/HRI/DirectX/DirectX9/RenderDirectX9.cpp"
# "${RenderSourceDir}/HRI/OpenGL/RenderOpenGL.cpp"
# "${RenderSourceDir}/HRI/Other/renderer_gnm.cpp"
# "${RenderSourceDir}/HRI/Other/renderer_noop.cpp"
# "${RenderSourceDir}/HRI/Other/renderer_nvn.cpp"
# "${RenderSourceDir}/HRI/Web/RenderWebGPU.cpp"
# "${RenderSourceDir}/HRI/Vulkan/RenderVulkan.cpp"
# "${RenderSourceDir}/shader_dx9bc.cpp"
# "${RenderSourceDir}/shader_dxbc.cpp"
# "${RenderSourceDir}/shader_spirv.cpp"
# "${RenderSourceDir}/topology.cpp"
# "${RenderSourceDir}/vertexlayout.cpp"
# "${RenderSourceDir}/Encoder.cpp"
# "${RenderSourceDir}/IRenderContext.cpp"
# "${RenderSourceDir}/RenderContext.cpp"
# "${RenderSourceDir}/RenderFunction.cpp"
# "${RenderSourceDir}/StaticVariable.cpp"
# "${RenderSourceDir}/EmbeddedShader.cpp"
# "${RenderSourceDir}/RenderDebug.cpp"
# "${RenderSourceDir}/RenderHandle.cpp"
# "${RenderSourceDir}/RenderObject.cpp"
# "${RenderSourceDir}/RenderPlatform.cpp"
# "${RenderSourceDir}/RenderStructInternal.cpp"
# "${RenderSourceDir}/RTemp.cpp"
# )

# if(BGFX_AMALGAMATED)
	# set(BGFX_NOBUILD ${RenderSourceFile})
	# list(REMOVE_ITEM BGFX_NOBUILD ${RenderAmalgamateSourceFile})
	foreach(BGFX_SRC ${RenderSourceFile})
    message(${BGFX_SRC})
		set_source_files_properties( ${BGFX_SRC} PROPERTIES HEADER_FILE_ONLY ON )
	endforeach()
# else()
# 	# Do not build using amalgamated sources
# 	set_source_files_properties( ${BGFX_DIR}/src/amalgamated.cpp PROPERTIES HEADER_FILE_ONLY ON )
# 	set_source_files_properties( ${BGFX_DIR}/src/amalgamated.mm PROPERTIES HEADER_FILE_ONLY ON )
# endif()

file(GLOB_RECURSE RenderIncludeFile
     ${CMAKE_CURRENT_SOURCE_DIR}/Include/Render/*.h)
# set(RenderSourceFile ${BGFX_AMALGAMATED_SOURCE})

set(RenderSourceFile 
"${RenderSourceDir}/bgfx.cpp"
"${RenderSourceDir}/DebugRenderdoc.cpp"
"${RenderSourceDir}/HRI/DirectX/DXGI.cpp"
"${RenderSourceDir}/HRI/OpenGL/OpenGLEGL.cpp"
"${RenderSourceDir}/HRI/OpenGL/OpenGLGLX.cpp"
"${RenderSourceDir}/HRI/OpenGL/OpenGLWGL.cpp"
"${RenderSourceDir}/HRI/OpenGL/OpenGLHTML5.cpp"
"${RenderSourceDir}/HRI/DirectX/NVIDIAAPI.cpp"
"${RenderSourceDir}/HRI/DirectX/DirectX11/RenderDirectX11.cpp"
"${RenderSourceDir}/HRI/DirectX/DirectX12/RenderDirectX12.cpp"
"${RenderSourceDir}/HRI/DirectX/DirectX9/RenderDirectX9.cpp"
"${RenderSourceDir}/HRI/OpenGL/RenderOpenGL.cpp"
"${RenderSourceDir}/HRI/Other/renderer_gnm.cpp"
"${RenderSourceDir}/HRI/Other/renderer_noop.cpp"
"${RenderSourceDir}/HRI/Other/renderer_nvn.cpp"
"${RenderSourceDir}/HRI/Web/RenderWebGPU.cpp"
"${RenderSourceDir}/HRI/Vulkan/RenderVulkan.cpp"
"${RenderSourceDir}/shader_dx9bc.cpp"
"${RenderSourceDir}/shader_dxbc.cpp"
"${RenderSourceDir}/shader_spirv.cpp"
"${RenderSourceDir}/topology.cpp"
"${RenderSourceDir}/vertexlayout.cpp"
"${RenderSourceDir}/Encoder.cpp"
"${RenderSourceDir}/IRenderContext.cpp"
"${RenderSourceDir}/RenderContext.cpp"
"${RenderSourceDir}/RenderFunction.cpp"
"${RenderSourceDir}/StaticVariable.cpp"
"${RenderSourceDir}/EmbeddedShader.cpp"
"${RenderSourceDir}/RenderDebug.cpp"
"${RenderSourceDir}/RenderHandle.cpp"
"${RenderSourceDir}/RenderObject.cpp"
"${RenderSourceDir}/RenderPlatform.cpp"
"${RenderSourceDir}/RenderStructInternal.cpp"
"${RenderSourceDir}/RTemp.cpp"
)
# message("asdfasdf")
# message(${RenderSourceFile})
# set(BGFX_NOBUILD ${RenderSourceFile})
# list(REMOVE_ITEM BGFX_NOBUILD ${BGFX_AMALGAMATED_SOURCE})
# foreach(BGFX_SRC ${RenderSourceFile})
#   set_source_files_properties(${BGFX_SRC} PROPERTIES HEADER_FILE_ONLY ON)
# endforeach()

add_library(Render )
# target_sources(Render PUBLIC ${RenderIncludeFile} ${RenderSourceFile} ${RenderAmalgamateSourceFile})
target_sources(Render PUBLIC ${RenderIncludeFile} ${RenderSourceFile} )
target_link_libraries(Render PUBLIC bx bimg dxsdk khronos renderdoc)
# Includes
target_include_directories(
  Render
  PUBLIC
         ${CMAKE_CURRENT_SOURCE_DIR}/Include/Render
         ${CMAKE_CURRENT_SOURCE_DIR}/Include/
         ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/)
# Special Visual Studio Flags
if(MSVC)
  target_compile_definitions(Render PRIVATE "_CRT_SECURE_NO_WARNINGS")
endif()
set_target_properties(Render PROPERTIES FOLDER "Render/aa")

# ----------------------------------------------------------------------------
# Render VertexLayout
# ----------------------------------------------------------------------------

add_library(RenderVertexLayout INTERFACE)
target_sources(
  RenderVertexLayout
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/Render/Tools/VertexLayout/VertexLayout.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/Tools/VertexLayout/VertexLayout.cpp)
target_include_directories(RenderVertexLayout
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include/)

# target_link_libraries(RenderVertexLayout PUBLIC bx)

# add_library(RenderVertexLayout)

# target_sources( RenderVertexLayout PUBLIC
# ${CMAKE_CURRENT_SOURCE_DIR}/Include/Render/Tools/VertexLayout/VertexLayout.h
# ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/Tools/VertexLayout/VertexLayout.cpp)
# target_include_directories(RenderVertexLayout PUBLIC
# ${CMAKE_CURRENT_SOURCE_DIR}/Include/) target_link_libraries(RenderVertexLayout
# PUBLIC bx)

# ----------------------------------------------------------------------------
# Render ShaderSpirv
# ----------------------------------------------------------------------------


add_library(RenderShaderSpirv INTERFACE)

target_sources(
  RenderShaderSpirv
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/Tools/ShaderSpirv/ShaderSpirv.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/Include/Render/Tools/ShaderSpirv/ShaderSpirv.h
)
target_include_directories(RenderShaderSpirv
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include)
# target_link_libraries(RenderShaderSpirv PUBLIC bx bimg)

# target_sources( RenderShaderSpirv PUBLIC
# ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/Tools/ShaderSpirv/ShaderSpirv.cpp
# ${CMAKE_CURRENT_SOURCE_DIR}/Include/Render/Tools/ShaderSpirv/ShaderSpirv.h)
# target_include_directories(RenderShaderSpirv PUBLIC
# ${CMAKE_CURRENT_SOURCE_DIR}/Include) target_link_libraries(RenderShaderSpirv
# PUBLIC bx bimg)

# ----------------------------------------------------------------------------
# Render Bounds
# ----------------------------------------------------------------------------

add_library(RenderBounds INTERFACE)
target_sources(
  RenderBounds
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/Tools/Bounds/Bounds.cpp)
target_include_directories(RenderBounds
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include/)
# target_link_libraries(Render)

add_executable(
  RenderShaderc
  ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/Tools/Shaderc/Shaderc.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Include/Render/Tools/Shaderc/Shaderc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/Tools/Shaderc/ShadercGLSL.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/Tools/Shaderc/ShadercHLSL.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/Tools/Shaderc/ShadercMetal.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/Tools/Shaderc/ShadercPssl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/Tools/Shaderc/ShadercSpirv.cpp)

target_compile_definitions(RenderShaderc PRIVATE "-D_CRT_SECURE_NO_WARNINGS")
set_target_properties(RenderShaderc PROPERTIES FOLDER "Render/Tools")
target_link_libraries(
  RenderShaderc
  PRIVATE bx
          bimg
          RenderVertexLayout
          RenderShaderSpirv
          fcpp
          glsl-optimizer
          glslang
          spirv-cross
          spirv-tools
          khronos
          webgpu)

if(ANDROID)
  target_link_libraries(RenderShaderc PRIVATE log)
elseif(IOS)
  set_target_properties(
    RenderShaderc PROPERTIES MACOSX_BUNDLE ON MACOSX_BUNDLE_GUI_IDENTIFIER
                                              RenderShaderc)
endif()

# ----------------------------------------------------------------------------
# Render Geometryc
# ----------------------------------------------------------------------------

add_executable(RenderGeometryc)
target_sources(
  RenderGeometryc
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/Tools/Geometryc/Geometryc.cpp)
target_compile_definitions(RenderGeometryc PRIVATE "-D_CRT_SECURE_NO_WARNINGS")
set_target_properties(RenderGeometryc PROPERTIES FOLDER "Render/Tools")
target_include_directories(RenderGeometryc
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
target_link_libraries(RenderGeometryc bx RenderBounds RenderVertexLayout
                      meshoptimizer cgltf)
if(IOS)
  set_target_properties(
    RenderGeometryc PROPERTIES MACOSX_BUNDLE ON MACOSX_BUNDLE_GUI_IDENTIFIER
                                                example-${ARG_NAME})
endif()

# ----------------------------------------------------------------------------
# Render Geometryv
# ----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# Render Testurec
# ----------------------------------------------------------------------------

add_executable(RenderTexturec)
target_sources(
  RenderTexturec
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/Tools/Texturec/Texturec.cpp)
target_link_libraries(RenderTexturec bimg)
if(ANDROID)
  target_link_libraries(RenderTexturec log)
elseif(IOS)
  set_target_properties(
    RenderTexturec PROPERTIES MACOSX_BUNDLE ON MACOSX_BUNDLE_GUI_IDENTIFIER
                                               RenderTexturec)
endif()
set_target_properties(RenderTexturec PROPERTIES FOLDER "Render/Tools")

# add_executable( shaderc ${BGFX_DIR}/tools/shaderc/shaderc.cpp
# ${BGFX_DIR}/tools/shaderc/shaderc.h ${BGFX_DIR}/tools/shaderc/shaderc_glsl.cpp
# ${BGFX_DIR}/tools/shaderc/shaderc_hlsl.cpp
# ${BGFX_DIR}/tools/shaderc/shaderc_pssl.cpp
# ${BGFX_DIR}/tools/shaderc/shaderc_spirv.cpp
# ${BGFX_DIR}/tools/shaderc/shaderc_metal.cpp)
# target_compile_definitions(shaderc PRIVATE "-D_CRT_SECURE_NO_WARNINGS")
# set_target_properties(shaderc PROPERTIES FOLDER "bgfx/tools")
# target_link_libraries( shaderc PRIVATE bx bimg bgfx-vertexlayout
# bgfx-shader-spirv fcpp glsl-optimizer glslang spirv-cross spirv-tools webgpu)

# file(GLOB RenderHelpIncludeFile
# ${CMAKE_CURRENT_SOURCE_DIR}/Include/Render/Help/*.h
# ${CMAKE_CURRENT_SOURCE_DIR}/Include/Render/Help/*.hpp) file(GLOB
# RenderHelpSourceFile ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/Help/*.cpp
# ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/*.mm) add_library(RenderHelp)
# target_sources(RenderHelp PUBLIC ${RenderHelpIncludeFile}
# ${RenderHelpSourceFile}) target_link_libraries(RenderHelp PUBLIC dear-imgui
# meshoptimizer)

# if(BGFX_WITH_GLFW) find_package(glfw3 REQUIRED)
# target_link_libraries(example-${ARG_NAME} PUBLIC glfw)
# target_compile_definitions(example-${ARG_NAME} PUBLIC ENTRY_CONFIG_USE_GLFW)
# elseif(BGFX_WITH_SDL) find_package(SDL2 REQUIRED)
# target_link_libraries(example-${ARG_NAME} PUBLIC ${SDL2_LIBRARIES})
# target_compile_definitions(example-${ARG_NAME} PUBLIC ENTRY_CONFIG_USE_SDL)
# elseif(UNIX AND NOT APPLE) target_link_libraries(example-${ARG_NAME} PUBLIC
# X11) endif()

# target_include_directories( RenderHelp PUBLIC
# $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
# ${CMAKE_CURRENT_SOURCE_DIR}/Include/Render PRIVATE ${BGFX_DIR}/3rdparty
# ${BGFX_DIR}/3rdparty/dxsdk/include ${BGFX_DIR}/3rdparty/khronos)

# set(BGFXLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

# # Ensure the directory exists if( NOT IS_DIRECTORY ${BGFXLIB_DIR} ) message(
# SEND_ERROR "Could not load bgfx, directory does not exist. ${BGFXLIB_DIR}" )
# return() endif()

# if(NOT APPLE) set(BGFX_AMALGAMATED_SOURCE ${BGFXLIB_DIR}/src/Amalgamated.cpp)
# else() set(BGFX_AMALGAMATED_SOURCE ${BGFXLIB_DIR}/src/Amalgamated.mm) endif()

# # Grab the bgfx source files file(GLOB_RECURSE  BGFX_SOURCES
# ${BGFXLIB_DIR}/src/*.cpp ${BGFXLIB_DIR}/src/*.mm ${BGFXLIB_DIR}/src/*.h
# ${BGFXLIB_DIR}/include/bgfx/*.h  ) # if(BGFX_AMALGAMATED) set(BGFX_NOBUILD
# ${BGFX_SOURCES}) list(REMOVE_ITEM BGFX_NOBUILD ${BGFX_AMALGAMATED_SOURCE})
# foreach(BGFX_SRC ${BGFX_NOBUILD}) set_source_files_properties( ${BGFX_SRC}
# PROPERTIES HEADER_FILE_ONLY ON ) endforeach() # else() #       # Do not build
# using amalgamated sources #       set_source_files_properties(
# ${BGFXLIB_DIR}/src/amalgamated.cpp PROPERTIES HEADER_FILE_ONLY ON ) #
# set_source_files_properties( ${BGFXLIB_DIR}/src/amalgamated.mm PROPERTIES
# HEADER_FILE_ONLY ON ) # endif()

# add_library( bgfx ${BGFX_LIBRARY_TYPE} ${BGFX_SOURCES} )

# if(BGFX_CONFIG_RENDERER_WEBGPU) include(cmake/3rdparty/webgpu.cmake)
# target_compile_definitions( bgfx PRIVATE BGFX_CONFIG_RENDERER_WEBGPU=1) if
# (EMSCRIPTEN) target_link_options(bgfx PRIVATE "-s USE_WEBGPU=1") else()
# target_link_libraries(bgfx PRIVATE webgpu) endif() endif()

# # Enable BGFX_CONFIG_DEBUG in Debug configuration target_compile_definitions(
# bgfx PRIVATE "$<$<CONFIG:Debug>:BGFX_CONFIG_DEBUG=1>" ) if(BGFX_CONFIG_DEBUG)
# target_compile_definitions( bgfx PRIVATE BGFX_CONFIG_DEBUG=1) endif()

# if( NOT ${BGFX_OPENGL_VERSION} STREQUAL "" ) target_compile_definitions( bgfx
# PRIVATE BGFX_CONFIG_RENDERER_OPENGL_MIN_VERSION=${BGFX_OPENGL_VERSION} )
# endif()

# if( NOT ${BGFX_OPENGLES_VERSION} STREQUAL "" ) target_compile_definitions(
# bgfx PRIVATE
# BGFX_CONFIG_RENDERER_OPENGLES_MIN_VERSION=${BGFX_OPENGLES_VERSION} ) endif()

# # Special Visual Studio Flags if( MSVC ) target_compile_definitions( bgfx
# PRIVATE "_CRT_SECURE_NO_WARNINGS" ) endif()

# # Includes target_include_directories( bgfx PRIVATE ${BGFX_DIR}/3rdparty
# ${BGFX_DIR}/3rdparty/dxsdk/include ${BGFX_DIR}/3rdparty/khronos PUBLIC
# $<BUILD_INTERFACE:${BGFXLIB_DIR}/include/>
# $<BUILD_INTERFACE:${BGFXLIB_DIR}/include/bgfx/>
# $<BUILD_INTERFACE:${BGFXLIB_DIR}/src/>
# $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# # bgfx depends on bx and bimg target_link_libraries( bgfx PUBLIC bx bimg )

# # ovr support if( BGFX_USE_OVR ) target_link_libraries( bgfx PUBLIC ovr )
# endif()

# # Frameworks required on iOS, tvOS and macOS if( ${CMAKE_SYSTEM_NAME} MATCHES
# iOS|tvOS ) target_link_libraries (bgfx PUBLIC "-framework OpenGLES  -framework
# Metal -framework UIKit -framework CoreGraphics -framework QuartzCore") elseif(
# APPLE ) find_library( COCOA_LIBRARY Cocoa ) find_library( METAL_LIBRARY Metal
# ) find_library( QUARTZCORE_LIBRARY QuartzCore ) mark_as_advanced(
# COCOA_LIBRARY ) mark_as_advanced( METAL_LIBRARY ) mark_as_advanced(
# QUARTZCORE_LIBRARY ) target_link_libraries( bgfx PUBLIC ${COCOA_LIBRARY}
# ${METAL_LIBRARY} ${QUARTZCORE_LIBRARY} ) endif()

# if( UNIX AND NOT APPLE AND NOT EMSCRIPTEN AND NOT ANDROID ) find_package(X11
# REQUIRED) find_package(OpenGL REQUIRED) #The following commented libraries are
# linked by bx #find_package(Threads REQUIRED) #find_library(LIBRT_LIBRARIES rt)
# #find_library(LIBDL_LIBRARIES dl) target_link_libraries( bgfx PUBLIC
# ${X11_LIBRARIES} ${OPENGL_LIBRARIES}) endif()

# # Exclude mm files if not on OS X if( NOT APPLE ) set_source_files_properties(
# ${BGFXLIB_DIR}/src/glcontext_eagl.mm PROPERTIES HEADER_FILE_ONLY ON )
# set_source_files_properties( ${BGFXLIB_DIR}/src/glcontext_nsgl.mm PROPERTIES
# HEADER_FILE_ONLY ON ) set_source_files_properties(
# ${BGFXLIB_DIR}/src/RenderMTL.mm PROPERTIES HEADER_FILE_ONLY ON ) endif()

# # Exclude glx context on non-unix if( NOT UNIX OR APPLE )
# set_source_files_properties( ${BGFXLIB_DIR}/src/glcontext_glx.cpp PROPERTIES
# HEADER_FILE_ONLY ON ) endif()

# # Put in a "bgfx" folder in Visual Studio set_target_properties( bgfx
# PROPERTIES FOLDER "bgfx" )

# # in Xcode we need to specify this file as objective-c++ (instead of renaming
# to .mm) if (XCODE)
# set_source_files_properties(${BGFXLIB_DIR}/src/renderer_vk.cpp PROPERTIES
# LANGUAGE OBJCXX XCODE_EXPLICIT_FILE_TYPE sourcecode.cpp.objcpp) endif()

# --------------------------------------------

# --------------------------------------------

# MathLib
set(MathLibIncludeDir ${CMAKE_CURRENT_SOURCE_DIR}/Include)
file(GLOB_RECURSE MathLibIncludeFile
     ${CMAKE_CURRENT_SOURCE_DIR}/Include/Math/*.h)
file(GLOB_RECURSE MathLibSourceFile ${CMAKE_CURRENT_SOURCE_DIR}/Src/Math/*.cpp)
message(STATUS "math lib include file: ${MathLibIncludeFile}")

add_library(MathLib)
target_include_directories(MathLib PUBLIC ${MathLibIncludeDir})
target_sources(MathLib PUBLIC ${MathLibIncludeFile} ${MathLibSourceFile})
target_link_libraries(MathLib Eigen)

set_target_properties(MathLib PROPERTIES FOLDER "Sparrow")

# target_include_directories(Core )
