# project(MathTest)

add_executable(TestMathVector)
target_sources(TestMathVector PRIVATE ./TestVector.cpp)
target_link_libraries(TestMathVector PRIVATE MathLib gtest gtest_main)


target_compile_options(
  TestMathVector
  PUBLIC $<$<CONFIG:>:/MT> $<$<CONFIG:Debug>:/MTd> $<$<CONFIG:Release>:/MT>
         $<$<CXX_COMPILER_ID:MSVC>:  -wd4201 /W4 /WX /std:c++17 >)
include_directories(${RootDir}/External/eigen/Eigen/)

# add_executable(TestEigen)
# target_include_directories(TestEigen PUBLIC ${RootDir}/External/eigen/)
# message(${RootDir})
# target_sources(TestEigen PRIVATE ./TestEigen.cpp)
# # target_include_directories(TestEigen PRIVATE ${RootDir}/External/eigen/Eigen/)
# target_compile_options(
#   TestEigen PUBLIC $<$<CONFIG:>:/MT> $<$<CONFIG:Debug>:/MTd>
#                    $<$<CONFIG:Release>:/MT>)
# target_link_libraries(TestEigen gtest  Eigen)
# set_target_properties(TestEigen PROPERTIES FOLDER "Test/Math")

# set(path ${RootDir}/External/eigen/Eigen/) message(STATE ${path}) #
# target_include_directories(TestEigen  PRIVATE  ${path} )
# target_include_directories(TestEigen  PRIVATE ${RootDir}/External/eigen/Eigen/
# )

# cmake -G "Visual Studio 16 2019" ../llvm
# -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra"

# set(MathDeps "MathLib gtest gtest_main" )
# set(MathFolder "Test/Math")
# AddTestExe(${CMAKE_CURRENT_SOURCE_DIR}  ${MathDeps} "Test/Math")