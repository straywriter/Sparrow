# macro(AddEigenTestExe FileDir)
set(FileDir ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB Files ${FileDir}/*.cpp)
foreach(TestFile ${Files})
  get_filename_component(TestFileName ${TestFile} NAME_WE)
  add_executable(${TestFileName} ${TestFile})

  # target_include_directories(TestEigen PUBLIC ${RootDir}/External/eigen/)
  target_link_libraries(${TestFileName} PUBLIC gtest gtest_main Eigen)
  set_target_properties(${TestFileName} PROPERTIES FOLDER "Test/External/Eigen")
  target_compile_options(
    ${TestFileName}
    PUBLIC $<$<CONFIG:>:/MT> $<$<CONFIG:Debug>:/MTd> $<$<CONFIG:Release>:/MT>
           $<$<CXX_COMPILER_ID:MSVC>: /WX >)
  add_test(${TestFileName} ${TestFileName})
endforeach()
# endmacro()

# addmathtestexe(${CMAKE_CURRENT_SOURCE_DIR})
