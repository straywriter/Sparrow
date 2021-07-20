
 file(GLOB Files *.cpp)
  foreach(TestFile ${Files})
    get_filename_component(TestFileName ${TestFile} NAME_WE)
    add_executable(${TestFileName} ${TestFile})
    target_link_libraries(${TestFileName}  PUBLIC render-common)
    set_target_properties(${TestFileName} PROPERTIES FOLDER "Test/Render")
    add_test(${TestFileName} ${TestFileName})
  endforeach()
