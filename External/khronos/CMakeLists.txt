add_library(khronos INTERFACE)
target_include_directories(khronos INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)