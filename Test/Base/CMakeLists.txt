
# project(CoreTest)


file(GLOB CoreFile ./*.cpp)

foreach(TestFile ${CoreFile})
  get_filename_component(TestFileName ${TestFile} NAME_WE)
  add_executable(${TestFileName} ${TestFile} )
  target_include_directories(${TestFileName}  PUBLIC )
  target_link_libraries(${TestFileName} PUBLIC SparrowBase gtest_main gmock )
  # target_link_libraries(${TestFileName} PRIVATE  )
  add_test(${TestFileName} ${TestFileName})

   target_compile_definitions (${TestFileName} PRIVATE _CRT_SECURE_NO_WARNINGS)
  # $set_tests_properties(${TestFileName} PROPERTIES TIMEOUT 5)
endforeach()