
# cmake_minimum_required(VERSION 3.10)

# project(Eigen)

add_library(Eigen INTERFACE)
target_include_directories(Eigen INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
# file(GLOB_RECURSE EigenFile ./Eigen/*)
# target_include_directories(Eigen INTERFACE ./Eigen/)
# target_sources(Eigen INTERFACE ${EigenFile})

