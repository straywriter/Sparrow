add_library(spirv-headers INTERFACE)
target_include_directories(
  spirv-headers
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
