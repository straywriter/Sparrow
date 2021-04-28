





project(Sparrow)



set(IncludeDir ${CMAKE_CURRENT_SOURCE_DIR}/Include)


# Core

# Render
set(RenderIncludeDir ${CMAKE_CURRENT_SOURCE_DIR}/Include)
file(GLOB_RECURSE RenderIncludeFile ${CMAKE_CURRENT_SOURCE_DIR}/Include/Render/*.h)
file(GLOB_RECURSE RenderSourceFile ${CMAKE_CURRENT_SOURCE_DIR}/Src/Render/*.cpp)
message(STATUS "render file ${RenderIncludeFile}")
message(STATUS "render file :${RenderSourceFile}")
add_library(Render)
target_include_directories(Render PUBLIC ${RenderIncludeDir})
target_sources(Render PUBLIC ${RenderIncludeFile} ${RenderSourceFile})
target_link_libraries(Render PUBLIC glfw glad)

set_target_properties(Render PROPERTIES FOLDER "Sparrow")

# GPU OpenGL
file(GLOB_RECURSE OpenGLIncludeFile ${CMAKE_CURRENT_SOURCE_DIR}/Include/GPU/OpenGL/*.h)
file(GLOB_RECURSE OpenGLSourceFile ${CMAKE_CURRENT_SOURCE_DIR}/Src/GPU/OpenGL/*.cpp)
message(STATUS "opengl file :${OpenGLIncludeFile}")
message(STATUS "opengl file :${OpenGLSourceFile}")
add_library(OpenGL)
target_sources(OpenGL PUBLIC ${OpenGLIncludeFile} ${OpenGLSourceFile})
target_include_directories(OpenGL PUBLIC ${IncludeDir})
target_link_libraries(OpenGL PUBLIC glfw glad)
set_target_properties(OpenGL PROPERTIES FOLDER "Sparrow/GPU")






# MathLib
set(MathLibIncludeDir ${CMAKE_CURRENT_SOURCE_DIR}/Include)
# file(GLOB_RECURSE MathLibIncludeFile ${CMAKE_CURRENT_SOURCE_DIR}/Include/Math/*.h)
file(GLOB_RECURSE MathLibSourceFile ${CMAKE_CURRENT_SOURCE_DIR}/Src/Math/*.cpp)
message(STATUS "math lib include file: ${MathLibIncludeFile}")

add_library(MathLib)
target_include_directories(MathLib PUBLIC ${MathLibIncludeDir})
target_sources(MathLib PUBLIC ${MathLibIncludeFile} ${MathLibSourceFile})
 target_link_libraries(MathLib  Eigen)

set_target_properties(MathLib PROPERTIES FOLDER "Sparrow")


# target_include_directories(Core )