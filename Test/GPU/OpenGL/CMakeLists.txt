file(GLOB Files *.cpp)
foreach(TestFile ${Files})
  get_filename_component(TestFileName ${TestFile} NAME_WE)
  add_executable(${TestFileName} ${TestFile})
  message(${SpdLogIncludeDir})
  target_link_libraries(${TestFileName} PUBLIC  OpenGL)
  set_target_properties(${TestFileName} PROPERTIES FOLDER "GPU/OpenGL")
  add_test(${TestFileName} ${TestFileName})
endforeach()
