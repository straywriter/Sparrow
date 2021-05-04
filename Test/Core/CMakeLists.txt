
# project(CoreTest)


file(GLOB CoreFile ./*.cpp)

foreach(TestFile ${CoreFile})
  get_filename_component(TestFileName ${TestFile} NAME_WE)
  add_executable(${TestFileName} ${TestFile} )
  target_include_directories(${TestFileName}  PUBLIC )
  target_link_libraries(${TestFileName}  Core )
  # target_link_libraries(${TestFileName} PRIVATE  )
  add_test(${TestFileName} ${TestFileName})
  # $set_tests_properties(${TestFileName} PROPERTIES TIMEOUT 5)
endforeach()