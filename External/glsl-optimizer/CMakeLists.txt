# glsl-optimizer

# glsl-optimizer makes UBSan segfault for whatever reason We already know that
# it's full of UB anyway... so just don't build it with UBSan string(REPLACE
# "-fsanitize=undefined" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}") string(REPLACE
# "-fsanitize=undefined" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# ----------------------------------------------------------
# glcpp
# ----------------------------------------------------------
file(GLOB GLCPP_SOURCES ./src/glsl/glcpp/*.c ./src/util/*.c)
add_library(glcpp ${GLCPP_SOURCES})
set(GLSL-OPTIMIZER_INCLUDES include src/mesa src/mapi src/glsl src)
target_include_directories(glcpp PUBLIC ${GLSL-OPTIMIZER_INCLUDES})
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
  target_compile_options(glcpp PRIVATE "-fno-strict-aliasing")
endif()
if(MSVC)
  set_target_properties(glcpp PROPERTIES COMPILE_FLAGS "/W0")
endif()
set_target_properties(glcpp PROPERTIES FOLDER "External/glsl-optimizer")

# mesa
file(GLOB MESA_SOURCES ./src/mesa/program/*.c ./src/mesa/main/*.c)
add_library(mesa ${MESA_SOURCES})
target_include_directories(mesa PUBLIC ${GLSL-OPTIMIZER_INCLUDES})
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
  target_compile_options(mesa PRIVATE "-fno-strict-aliasing")
endif()
if(MSVC)
  set_target_properties(mesa PROPERTIES COMPILE_FLAGS "/W0")
endif()
set_target_properties(mesa PROPERTIES FOLDER "External/glsl-optimizer")

# ----------------------------------------------------------
# glsl-optimizer
# ----------------------------------------------------------
file(GLOB GLSL-OPTIMIZER_SOURCES ./src/glsl/*.cpp ./src/glsl/*.c)
file(GLOB GLSL-OPTIMIZER_SOURCES_REMOVE ./src/glsl/main.cpp
     ./src/glsl/builtin_stubs.cpp)
list(REMOVE_ITEM GLSL-OPTIMIZER_SOURCES ${GLSL-OPTIMIZER_SOURCES_REMOVE})
add_library(glsl-optimizer ${GLSL-OPTIMIZER_SOURCES})
target_link_libraries(glsl-optimizer glcpp mesa)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES
                                           "Clang")
  target_compile_options(
    glsl-optimizer
    PRIVATE -fno-strict-aliasing -Wno-implicit-fallthrough -Wno-parentheses
            -Wno-sign-compare -Wno-unused-function -Wno-unused-parameter)
endif()
if(MSVC)
  set_target_properties(glsl-optimizer PROPERTIES COMPILE_FLAGS "/W0")
  target_compile_definitions(
    glsl-optimizer PRIVATE __STDC__ __STDC_VERSION__=199901L strdup=_strdup
                           alloca=_alloca isascii=__isascii)
elseif(APPLE)
  target_compile_options(glsl-optimizer PRIVATE -Wno-deprecated-register)
endif()
set_target_properties(glsl-optimizer PROPERTIES FOLDER "External/glsl-optimizer")

# cmake_minimum_required(VERSION 2.8)

# include_directories(include) include_directories(src/mesa)
# include_directories(src/mapi) include_directories(src/glsl)
# include_directories(src)

# option (DEBUG "Enable debugging" FALSE)

# if(${DEBUG} MATCHES "on") set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0") else() set(CMAKE_CXX_FLAGS
# "${CMAKE_CXX_FLAGS} -Os -DNDEBUG") set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os
# -DNDEBUG") set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
# endif()

# file(GLOB glcpp-library_sources src/glsl/glcpp/*.c src/util/*.c) #file(GLOB
# glcpp-library_sources_remove src/glsl/glcpp/glcpp.c) #list(REMOVE_ITEM
# glcpp-library_sources ${glcpp-library_sources_remove})
# add_library(glcpp-library ${glcpp-library_sources})

# file(GLOB mesa_sources src/mesa/program/*.c src/mesa/main/*.c)
# add_library(mesa ${mesa_sources})

# file(GLOB glsl_sources src/glsl/*.cpp src/glsl/*.c) file(GLOB
# glsl_sources_remove src/glsl/main.cpp src/glsl/builtin_stubs.cpp)
# list(REMOVE_ITEM glsl_sources ${glsl_sources_remove})
# add_library(glsl_optimizer ${glsl_sources})
# target_link_libraries(glsl_optimizer glcpp-library mesa)

# add_executable(glsl_compiler src/glsl/main.cpp)
# target_link_libraries(glsl_compiler glsl_optimizer)

# file(GLOB glsl_test_sources tests/*.cpp) add_executable(glsl_test
# ${glsl_test_sources}) target_link_libraries(glsl_test glsl_optimizer)
# if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # OSX-specific build requirements
# find_library(OpenGL_LIBRARY OpenGL ) target_link_libraries(glsl_test
# ${OpenGL_LIBRARY}) endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# file(GLOB glslopt_sources contrib/glslopt/*.cpp) add_executable(glslopt
# ${glslopt_sources}) target_link_libraries(glslopt glsl_optimizer)

# #add_executable(glcpp src/glsl/glcpp/glcpp.c) #target_link_libraries(glcpp
# glsl_optimizer)
