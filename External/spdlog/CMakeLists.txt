#
# Copyright(c) 2015 Ruslan Baratov. Distributed under the MIT License
# (http://opensource.org/licenses/MIT)
#
add_library(spdlog INTERFACE)
add_library(spdlog::spdlog ALIAS spdlog)

target_include_directories(
  spdlog INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>")
target_link_libraries(spdlog INTERFACE Threads::Threads)
