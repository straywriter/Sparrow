add_library(magic_enum INTERFACE)
add_library(magic_enum::magic_enum ALIAS magic_enum)
target_include_directories(
  magic_enum INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
