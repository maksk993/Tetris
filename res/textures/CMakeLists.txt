#res/textures/CMakeLists.txt

set(TEXTURE_FILES 
	res/textures/cells.png
	res/textures/numbers400x46.png
	res/textures/interface400x800.png
)

foreach(TEXTURE_FILE ${TEXTURE_FILES})
	configure_file(${CMAKE_SOURCE_DIR}/${TEXTURE_FILE} ${CMAKE_BINARY_DIR}/${TEXTURE_FILE} COPYONLY)
endforeach()