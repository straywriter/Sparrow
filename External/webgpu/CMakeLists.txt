
#----------------------------------------------------------
# webgpu
#----------------------------------------------------------
file(
	GLOB
    WEBGPU_SOURCES
	include/webgpu/*.h
    # ${BGFX_DIR}/3rdparty/webgpu/webgpu_cpp.cpp  # requires dawn to be installed
)

# Library without sources is interface
#add_library( webgpu STATIC ${WEBGPU_SOURCES} )
add_library(webgpu INTERFACE)
target_include_directories( webgpu 
    # PUBLIC
    INTERFACE
	    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
)

