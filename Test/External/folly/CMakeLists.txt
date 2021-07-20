
file(GLOB Files *.cpp)
foreach(TestFile ${Files})
  get_filename_component(TestFileName ${TestFile} NAME_WE)
  add_executable(${TestFileName} ${TestFile})
  target_link_libraries(${TestFileName} gtest gtest_main FBString)
  set_target_properties(${TestFileName} PROPERTIES FOLDER "Test/External/Folly")
#   target_compile_options(
#     ${TestFileName}
#     PUBLIC $<CXX_COMPILER_ID:MSVC>:
#            /std:c++17 >)
  add_test(${TestFileName} ${TestFileName})
endforeach()
