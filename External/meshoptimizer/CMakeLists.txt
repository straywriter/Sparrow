set(SOURCES
    meshoptimizer/src/meshoptimizer.h
    meshoptimizer/src/allocator.cpp
    meshoptimizer/src/clusterizer.cpp
    meshoptimizer/src/indexcodec.cpp
    meshoptimizer/src/indexgenerator.cpp
    meshoptimizer/src/overdrawanalyzer.cpp
    meshoptimizer/src/overdrawoptimizer.cpp
    meshoptimizer/src/simplifier.cpp
    meshoptimizer/src/spatialorder.cpp
    meshoptimizer/src/stripifier.cpp
    meshoptimizer/src/vcacheanalyzer.cpp
    meshoptimizer/src/vcacheoptimizer.cpp
    meshoptimizer/src/vertexcodec.cpp
    meshoptimizer/src/vertexfilter.cpp
    meshoptimizer/src/vfetchanalyzer.cpp
    meshoptimizer/src/vfetchoptimizer.cpp)

add_library(meshoptimizer STATIC ${SOURCES})
target_include_directories(meshoptimizer PUBLIC ./)
set_target_properties(meshoptimizer PROPERTIES FOLDER "External")

# set(SOURCES
#     src/meshoptimizer.h
#     src/allocator.cpp
#     src/clusterizer.cpp
#     src/indexcodec.cpp
#     src/indexgenerator.cpp
#     src/overdrawanalyzer.cpp
#     src/overdrawoptimizer.cpp
#     src/simplifier.cpp
#     src/spatialorder.cpp
#     src/stripifier.cpp
#     src/vcacheanalyzer.cpp
#     src/vcacheoptimizer.cpp
#     src/vertexcodec.cpp
#     src/vertexfilter.cpp
#     src/vfetchanalyzer.cpp
#     src/vfetchoptimizer.cpp)

# if(MSVC)
#   add_compile_options(/W4 /WX)
# else()
#   add_compile_options(-Wall -Wextra -Wshadow -Wno-missing-field-initializers
#                       -Werror)
# endif()

# if(MESHOPT_BUILD_SHARED_LIBS)
#   add_library(meshoptimizer SHARED ${SOURCES})
# else()
#   add_library(meshoptimizer STATIC ${SOURCES})
# endif()

# target_include_directories(
#   meshoptimizer INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")

# if(MESHOPT_BUILD_SHARED_LIBS)
#   set_target_properties(meshoptimizer PROPERTIES CXX_VISIBILITY_PRESET hidden)
#   set_target_properties(meshoptimizer PROPERTIES VISIBILITY_INLINES_HIDDEN ON)

#   if(WIN32)
#     target_compile_definitions(
#       meshoptimizer INTERFACE "MESHOPTIMIZER_API=__declspec(dllimport)")
#     target_compile_definitions(
#       meshoptimizer PRIVATE "MESHOPTIMIZER_API=__declspec(dllexport)")
#   else()
#     target_compile_definitions(
#       meshoptimizer
#       PUBLIC "MESHOPTIMIZER_API=__attribute__((visibility(\"default\")))")
#   endif()
# endif()
