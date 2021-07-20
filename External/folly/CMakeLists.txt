# cmake_minimum_required(VERSION 3.10)


# # add_executable(fbstringtest Fbsrtring.cpp ./folly/memory/detail/MallocImpl.cpp)
# add_executable(fbstringtest Fbsrtring.cpp )
# target_include_directories(fbstringtest PUBLIC ./)
# target_compile_options(fbstringtest PUBLIC $<$<CXX_COMPILER_ID:MSVC>:
#                                            /std:c++17 >)


add_library(FBString INTERFACE)
target_include_directories(FBString INTERFACE ./)
target_link_libraries(FBString INTERFACE Boost::preprocessor)