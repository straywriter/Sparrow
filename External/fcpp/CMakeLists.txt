#----------------------------------------------------------
#fcpp
#----------------------------------------------------------

file( GLOB FCPP_SOURCES ./*.c ./*.h )

add_library( fcpp STATIC ${FCPP_SOURCES} )
target_include_directories( fcpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_compile_definitions( fcpp
	PRIVATE
	NINCLUDE=64
	NWORK=65536
	NBUFF=65536
	OLD_PREPROCESSOR=0
)

if( MSVC )
	set_target_properties( fcpp PROPERTIES COMPILE_FLAGS "/W0" )
endif()

set_target_properties( fcpp PROPERTIES FOLDER "External" )
set_source_files_properties( ./usecpp.c PROPERTIES HEADER_FILE_ONLY ON )
