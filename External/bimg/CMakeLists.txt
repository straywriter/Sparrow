

# cmake_minimum_required(VERSION 3.10)

# project(bimg)


set(BIMG_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(
	GLOB
	ASTC_CODEC_SOURCES
	${BIMG_DIR}/3rdparty/astc-codec/src/decoder/*.cc
	${BIMG_DIR}/3rdparty/astc-codec/src/decoder/*.h
)

add_library( astc-codec STATIC ${ASTC_CODEC_SOURCES} )
target_include_directories( astc-codec
	PUBLIC
		$<BUILD_INTERFACE:${BIMG_DIR}/3rdparty>
		$<BUILD_INTERFACE:${BIMG_DIR}/3rdparty/astc-codec>
		$<BUILD_INTERFACE:${BIMG_DIR}/3rdparty/astc-codec/include> )
# set_target_properties( astc-codec PROPERTIES FOLDER "bgfx/3rdparty" )


file( GLOB ASTC_SOURCES ${BIMG_DIR}/3rdparty/astc/*.cpp ${BIMG_DIR}/3rdparty/astc/*.h )

add_library( astc STATIC ${ASTC_SOURCES} )
target_include_directories( astc PUBLIC $<BUILD_INTERFACE:${BIMG_DIR}/3rdparty> )
# set_target_properties( astc PROPERTIES FOLDER "bgfx/3rdparty" )

file( GLOB EDTAA3_SOURCES ${BIMG_DIR}/3rdparty/edtaa3/*.cpp ${BIMG_DIR}/3rdparty/edtaa3/*.h )

add_library( edtaa3 STATIC ${EDTAA3_SOURCES} )
target_include_directories( edtaa3 PUBLIC $<BUILD_INTERFACE:${BIMG_DIR}/3rdparty> )
set_target_properties( edtaa3 PROPERTIES FOLDER "bgfx/3rdparty" )


file( GLOB ETC1_SOURCES ${BIMG_DIR}/3rdparty/etc1/*.cpp ${BIMG_DIR}/3rdparty/etc1/*.h )

add_library( etc1 STATIC ${ETC1_SOURCES} )
target_include_directories( etc1 PUBLIC $<BUILD_INTERFACE:${BIMG_DIR}/3rdparty> )
set_target_properties( etc1 PROPERTIES FOLDER "bgfx/3rdparty" )

file( GLOB ETC2_SOURCES ${BIMG_DIR}/3rdparty/etc2/*.cpp ${BIMG_DIR}/3rdparty/etc2/*.h )
add_library( etc2 STATIC ${ETC2_SOURCES} )
target_include_directories( etc2 PUBLIC $<BUILD_INTERFACE:${BIMG_DIR}/3rdparty> )
set_target_properties( etc2 PROPERTIES FOLDER "bgfx/3rdparty" )
target_link_libraries( etc2 PUBLIC bx )


file( GLOB IQA_SOURCES ${BIMG_DIR}/3rdparty/iqa/source/*.c ${BIMG_DIR}/3rdparty/iqa/include/*.h )

add_library( iqa STATIC ${IQA_SOURCES} )
target_include_directories( iqa PUBLIC $<BUILD_INTERFACE:${BIMG_DIR}/3rdparty/iqa/include> )
set_target_properties( iqa PROPERTIES FOLDER "bgfx/3rdparty" )


file( GLOB SQUISH_SOURCES ${BIMG_DIR}/3rdparty/libsquish/*.cpp ${BIMG_DIR}/3rdparty/libsquish/*.h ${BIMG_DIR}/3rdparty/libsquish/*.inl )

add_library( squish STATIC ${SQUISH_SOURCES} )
target_include_directories( squish PUBLIC $<BUILD_INTERFACE:${BIMG_DIR}/3rdparty> )
set_target_properties( squish PROPERTIES FOLDER "bgfx/3rdparty" )


file(
	GLOB
	NVTT_SOURCES
	${BIMG_DIR}/3rdparty/nvtt/bc6h/*.cpp
	${BIMG_DIR}/3rdparty/nvtt/bc6h/*.h
	${BIMG_DIR}/3rdparty/nvtt/bc7/*.cpp
	${BIMG_DIR}/3rdparty/nvtt/bc7/*.h
	${BIMG_DIR}/3rdparty/nvtt/nvcore/*.h
	${BIMG_DIR}/3rdparty/nvtt/nvcore/*.inl
	${BIMG_DIR}/3rdparty/nvtt/nvmath/*.cpp
	${BIMG_DIR}/3rdparty/nvtt/nvmath/*.h
	${BIMG_DIR}/3rdparty/nvtt/*.cpp
	${BIMG_DIR}/3rdparty/nvtt/*.h
)

add_library( nvtt STATIC ${NVTT_SOURCES} )
target_include_directories( nvtt
	PUBLIC
		$<BUILD_INTERFACE:${BIMG_DIR}/3rdparty>
		$<BUILD_INTERFACE:${BIMG_DIR}/3rdparty/nvtt> )
set_target_properties( nvtt PROPERTIES FOLDER "bgfx/3rdparty" )
target_link_libraries( nvtt PUBLIC bx )



file( GLOB PVRTC_SOURCES ${BIMG_DIR}/3rdparty/pvrtc/*.cpp ${BIMG_DIR}/3rdparty/pvrtc/*.h )

add_library( pvrtc STATIC ${PVRTC_SOURCES} )
target_include_directories( pvrtc PUBLIC $<BUILD_INTERFACE:${BIMG_DIR}/3rdparty> )
set_target_properties( pvrtc PROPERTIES FOLDER "bgfx/3rdparty" )



# 


# Grab the bimg source files
file( GLOB BIMG_SOURCES ${BIMG_DIR}/src/*.cpp )

# Create the bimg target
add_library( bimg STATIC ${BIMG_SOURCES} )

# Add include directory of bimg
target_include_directories( bimg
	PUBLIC
		$<BUILD_INTERFACE:${BIMG_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# bimg dependencies
target_link_libraries( bimg bx astc-codec astc edtaa3 etc1 etc2 iqa squish nvtt pvrtc )

# Put in a "bgfx" folder in Visual Studio
set_target_properties( bimg PROPERTIES FOLDER "bgfx" )