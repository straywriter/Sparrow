# ----------------------------------------------------------
# spirv-tools
# ----------------------------------------------------------
file(
  GLOB
  SPIRV_TOOLS_SOURCES
  source/*.cpp
  source/*.h
  source/opt/*.cpp
  source/opt/*.h
  source/reduce/*.cpp
  source/reduce/*.h
  source/util/*.cpp
  source/util/*.h
  source/val/*.cpp
  source/val/*.h)

add_library(spirv-tools STATIC ${SPIRV_TOOLS_SOURCES})
target_include_directories(
  spirv-tools
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/
         ${CMAKE_CURRENT_SOURCE_DIR}/include
         ${CMAKE_CURRENT_SOURCE_DIR}/include/generated
         ${CMAKE_CURRENT_SOURCE_DIR}/source
       
)
target_link_libraries(spirv-tools PUBLIC spirv-headers)

if(MSVC)
  target_compile_options(spirv-tools PRIVATE "/wd4127" "/wd4389" "/wd4702"
                                             "/wd4706")
else()
  target_compile_options(spirv-tools PRIVATE "-Wno-switch")

  if(MINGW OR ${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    target_compile_options(spirv-tools PRIVATE "-Wno-misleading-indentation")
  endif()
endif()

set_target_properties(spirv-tools PROPERTIES FOLDER "External")
