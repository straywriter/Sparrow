file(GLOB Files *.cpp)
foreach(TestFile ${Files})
  get_filename_component(TestFileName ${TestFile} NAME_WE)
   set( TestFileC ${TestFile} ./main_test.cc)
  add_executable(${TestFileName} ${TestFileC})
  target_link_libraries(${TestFileName} bx)
  set_target_properties(${TestFileName} PROPERTIES FOLDER "Test/External/bx")
#   target_compile_options(
#     ${TestFileName}
#     PUBLIC $<$<CONFIG:>:/MT>
#            $<$<CONFIG:Debug>:/MTd>
#            $<$<CONFIG:Release>:/MT>
#            $<$<CXX_COMPILER_ID:MSVC>:
#            # /d1reportClassLayoutChanges /permissive-
#            /std:c++17
#            >)
  add_test(${TestFileName} ${TestFileName})
endforeach()
