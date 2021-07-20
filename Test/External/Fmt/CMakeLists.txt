file(GLOB Files *.cc)
foreach(TestFile ${Files})
  get_filename_component(TestFileName ${TestFile} NAME_WE)
  add_executable(${TestFileName} ${TestFile})
  target_link_libraries(${TestFileName} gmock_main fmt)
  set_target_properties(${TestFileName} PROPERTIES FOLDER "Test/External/fmt")
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

file(GLOB Files1 *.cpp)
foreach(TestFile ${Files1})
  get_filename_component(TestFileName ${TestFile} NAME_WE)
  add_executable(${TestFileName} ${TestFile})
  target_link_libraries(${TestFileName} gtest_main fmt)
  set_target_properties(${TestFileName} PROPERTIES FOLDER "Test/Math")
  target_compile_options(
    ${TestFileName}
    PUBLIC $<$<CONFIG:>:/MT>
           $<$<CONFIG:Debug>:/MTd>
           $<$<CONFIG:Release>:/MT>
           $<$<CXX_COMPILER_ID:MSVC>:
           # /d1reportClassLayoutChanges /permissive-
           /std:c++17
           >)
  add_test(${TestFileName} ${TestFileName})
endforeach()
