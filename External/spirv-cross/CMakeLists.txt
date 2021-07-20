# ----------------------------------------------------------
# spirv-cross
# ----------------------------------------------------------
file(GLOB SPIRV_CROSS_SOURCES ./*.cpp ./*.h)

add_library(spirv-cross STATIC ${SPIRV_CROSS_SOURCES})
target_compile_definitions(spirv-cross
                           PRIVATE SPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS)
target_include_directories(spirv-cross PUBLIC / /include)

if(MSVC)
  target_compile_options(spirv-cross PRIVATE "/wd4018" "/wd4245" "/wd4706"
                                             "/wd4715")
endif()

set_target_properties(spirv-cross PROPERTIES FOLDER "External")
