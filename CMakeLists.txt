cmake_minimum_required(VERSION 3.10)



set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE STRING "" FORCE)

project(Sparrow)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")


# if(MSVC)
#   set(variables CMAKE_CXX_FLAGS_DEBUG 
#                 CMAKE_CXX_FLAGS_RELEASE
#                 CMAKE_CXX_FLAGS_RELWITHDEBINFO 
#                 CMAKE_CXX_FLAGS_MINSIZEREL)
#   foreach(variable ${variables})
#     if(${variable} MATCHES "/MD")
#       string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
#     endif()
#   endforeach()
# endif()


set(RootDir ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)

add_subdirectory(./External)

add_subdirectory(./Sparrow)

add_subdirectory(./Test)

# add_subdirectory(Render)

# add_subdirectory(Example)
