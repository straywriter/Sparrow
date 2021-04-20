project(Test)

# External

# add_executable(TestOpenGLEnviroment)
# target_sources(
#   TestOpenGLEnviroment
#   PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/External/TestOpenGLEnvironment.cpp)
# target_link_libraries(TestOpenGLEnviroment glfw glad)

# add_executable(TestOpenGLContext)
# target_sources(TestOpenGLContext
#                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/GPU/TestOpenGLContext.cpp)
# target_link_libraries(TestOpenGLContext PUBLIC OpenGL)
# target_include_directories(TestOpenGLContext PUBLIC ../Sparrow/Include)

# math test

function(AddTestExe FileDir Deps Folder)

message(${FileDir})
message(${Deps})
message(${Folder})

  file(GLOB Files ${FileDir}/*.cpp)
  foreach(TestFile ${Files})
    get_filename_component(TestFileName ${TestFile} NAME_WE)
    add_executable(${TestFileName} ${TestFile})
    target_link_libraries(${TestFileName} PUBLIC ${Deps})
    set_target_properties(${TestFileName} PROPERTIES FOLDER ${Folder})
    target_compile_options(
      ${TestFileName} PUBLIC $<$<CONFIG:>:/MT> $<$<CONFIG:Debug>:/MTd>
                             $<$<CONFIG:Release>:/MT>)
    add_test(${TestFileName} ${TestFileName})
  endforeach()
endfunction()


# add_executable(TestMathVector) target_sources(TestMathVector PRIVATE
# ./Math/TestVector.cpp ) target_link_libraries(TestMathVector MathLib gtest
# gtest_main)
# add_subdirectory(Core)

add_subdirectory(Math)

add_subdirectory(External)
