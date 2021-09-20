

IF (CMAKE_BUILD_TYPE STREQUAL Debug)      
# target_compile_definitions()
    ADD_DEFINITIONS(-DDEBUG)                           
ENDIF()
# Core
file(GLOB_RECURSE BaseIncludeFile ${CMAKE_CURRENT_SOURCE_DIR}/Include/Base/*.h)
file(GLOB_RECURSE BaseSourceFile ${CMAKE_CURRENT_SOURCE_DIR}/Src/Base/*.cpp)
file(GLOB_RECURSE BaseInlineFile ${CMAKE_CURRENT_SOURCE_DIR}/Inline/Base/*.inl)
add_library(SparrowBase)
target_sources(SparrowBase PUBLIC ${BaseIncludeFile} ${BaseSourceFile} ${BaseInlineFile})
target_include_directories(SparrowBase PUBLIC ${IncludeDir} ${CMAKE_CURRENT_SOURCE_DIR}/Inline)
target_link_libraries(SparrowBase PUBLIC spdlog FBString Boost::algorithm fmt)
set_target_properties(SparrowBase PROPERTIES FOLDER "Sparrow/")
