add_library(dxsdk INTERFACE)
target_include_directories(dxsdk INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)