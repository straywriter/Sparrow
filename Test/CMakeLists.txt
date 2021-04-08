project(Test)

#External

add_executable(TestOpenGLEnviroment )
target_sources(TestOpenGLEnviroment PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/External/TestOpenGLEnvironment.cpp)
target_link_libraries(TestOpenGLEnviroment glfw glad)


add_executable(TestOpenGLContext )
target_sources(TestOpenGLContext PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/GPU/TestOpenGLContext.cpp)
target_link_libraries(TestOpenGLContext PUBLIC OpenGL )
target_include_directories(TestOpenGLContext PUBLIC ../Sparrow/Include)


#math test


# add_executable(TestMathVector)
# target_sources(TestMathVector PRIVATE ./Math/TestVector.cpp )
# target_link_libraries(TestMathVector MathLib gtest gtest_main)

add_subdirectory(Math)

add_subdirectory(External)