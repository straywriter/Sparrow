
project(CoreTest)


file(GLOB VectorTestSrc ./*.cpp)

foreach(TestFile ${TestSrc})
  get_filename_component(TestFileName ${TestFile} NAME_WE)
  add_executable(${TestFileName} ${TestFile} )
  target_include_directories(${TestFileName}  PUBLIC )
  target_link_libraries(${TestFileName} gtest gtest_main )
  # target_link_libraries(${TestFileName} PRIVATE  )
  add_test(${TestFileName} ${TestFileName})
  $set_tests_properties(${TestFileName} PROPERTIES TIMEOUT 5)
endforeach()