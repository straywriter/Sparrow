add_library(renderdoc INTERFACE)
target_include_directories(
  renderdoc INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>)
