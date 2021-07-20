add_library(cgltf INTERFACE)
target_include_directories(cgltf INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)