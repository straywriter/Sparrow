file(GLOB Files *.cpp)
foreach(TestFile ${Files})
  get_filename_component(TestFileName ${TestFile} NAME_WE)
  set( TestFileC ${TestFile} ./catchmain.cpp)
  add_executable(${TestFileName} ${TestFileC})
  target_link_libraries(${TestFileName} spdlog)
  target_include_directories(${TestFileName} PUBLIC ./ )
  set_target_properties(${TestFileName} PROPERTIES FOLDER "Test/External/spdlog")
  target_compile_options(
    ${TestFileName}
    PUBLIC $<$<CONFIG:>:/MT>
           $<$<CONFIG:Debug>:/MTd>
           $<$<CONFIG:Release>:/MT>
           $<$<CXX_COMPILER_ID:MSVC>:
           /std:c++17
           >)
  add_test(${TestFileName} ${TestFileName})
endforeach()