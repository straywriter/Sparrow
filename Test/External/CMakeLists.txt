# project(TestFme)


# cmake_policy(SET CMP0079 NEW)
macro(AddFmtExe FmtDir)
  file(GLOB FmtFile ${FmtDir}/*.cpp)

  foreach(TestFile ${FmtFile})

    get_filename_component(TestFileName ${TestFile} NAME_WE)
    add_executable(${TestFileName} ${TestFile})
    target_link_libraries(
      ${TestFileName} PUBLIC gtest 
                             ${CMAKE_THREAD_LIBS_INIT} fmt)
    # get_target_property(${TestFileName} PROPERTIES CMAKE_CXX_FLAGS_DEBUG  /MDd
    # /Z7 /Od) target_compile_options(${TestFileName} PUBLIC /MP)
    # set_property(TARGET ${TestFileName} PROPERTY MSVC_RUNTIME_LIBRARY
    # "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    target_compile_options(${TestFileName} PUBLIC  $<$<CONFIG:>:/MT>
    $<$<CONFIG:Debug>:/MTd> $<$<CONFIG:Release>:/MT> )
    add_test(${TestFileName} ${TestFileName})
    set_tests_properties(${TestFileName} PROPERTIES TIMEOUT 5)
  endforeach()
endmacro()

# add_executable(TestFmt ./fmt/TestFmt.cpp) target_link_libraries(TestFmt PUBLIC
# fmt)

addfmtexe(${CMAKE_CURRENT_SOURCE_DIR}/fmt)

# foreach(TestFile ) get_filename_component(TestFileName ${TestFile} NAME_WE)
# add_executable(${TestFileName}  ${TestFile} )
# target_link_libraries(${TestFileName}  gtest gtest_main
# ${CMAKE_THREAD_LIBS_INIT} fmt) # add_test(${TestFileName} ${TestFileName}) #
# set_tests_properties(${TestFileName} PROPERTIES TIMEOUT 5) endforeach()


add_subdirectory(spdlog)

add_subdirectory(ghc)

add_subdirectory(lua)