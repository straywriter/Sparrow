
# cmake_minimum_required(VERSION 3.10)

# project(bx)

set(BX_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Ensure the directory exists
if( NOT IS_DIRECTORY ${BX_DIR} )
	message( SEND_ERROR "Could not load bx, directory does not exist. ${BX_DIR}" )
	return()
endif()

# Grab the bx source files
file( GLOB BX_SOURCES ${BX_DIR}/src/*.cpp )

if(BX_AMALGAMATED)
	set(BX_NOBUILD ${BX_SOURCES})
	list(REMOVE_ITEM BX_NOBUILD ${BX_DIR}/src/amalgamated.cpp)
	foreach(BX_SRC ${BX_NOBUILD})
		set_source_files_properties( ${BX_SRC} PROPERTIES HEADER_FILE_ONLY ON )
	endforeach()
else()
	set_source_files_properties( ${BX_DIR}/src/amalgamated.cpp PROPERTIES HEADER_FILE_ONLY ON )
endif()

# Create the bx target
add_library( bx STATIC ${BX_SOURCES} )

# Link against psapi on Windows
if( WIN32 )
	target_link_libraries( bx PUBLIC psapi )
endif()

include(GNUInstallDirs)

# Add include directory of bx
target_include_directories( bx
	PUBLIC
		$<BUILD_INTERFACE:${BX_DIR}/include>
		$<BUILD_INTERFACE:${BX_DIR}/3rdparty>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )

# Build system specific configurations
if( MSVC )
	target_include_directories( bx
		PUBLIC
			$<BUILD_INTERFACE:${BX_DIR}/include/compat/msvc>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/compat/msvc> )
elseif( MINGW )
	target_include_directories( bx
		PUBLIC
		    $<BUILD_INTERFACE:${BX_DIR}/include/compat/mingw>
		    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/compat/mingw> )
elseif( APPLE )
	target_include_directories( bx
		PUBLIC
		    $<BUILD_INTERFACE:${BX_DIR}/include/compat/osx>
		    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/compat/osx> )
endif()

# All configurations
target_compile_definitions( bx PUBLIC "__STDC_LIMIT_MACROS" )
target_compile_definitions( bx PUBLIC "__STDC_FORMAT_MACROS" )
target_compile_definitions( bx PUBLIC "__STDC_CONSTANT_MACROS" )

target_compile_definitions( bx PRIVATE "$<$<CONFIG:Debug>:BX_CONFIG_DEBUG=1>" )
if(BGFX_CONFIG_DEBUG)
	target_compile_definitions( bx PRIVATE BX_CONFIG_DEBUG=1)
endif()

# Additional dependencies on Unix
if( UNIX AND NOT APPLE AND NOT ANDROID )
	# Threads
	find_package( Threads )
	target_link_libraries( bx ${CMAKE_THREAD_LIBS_INIT} dl )

	# Real time (for clock_gettime)
	target_link_libraries( bx rt )
elseif(APPLE)
	find_library( FOUNDATION_LIBRARY Foundation)
	mark_as_advanced( FOUNDATION_LIBRARY )
	target_link_libraries( bx PUBLIC ${FOUNDATION_LIBRARY} )
endif()

# Put in a "bgfx" folder in Visual Studio
set_target_properties( bx PROPERTIES FOLDER "bgfx" )


# add_subdirectory(tests)