#res/textures/CMakeLists.txt

set(TEXTURE_FILES 
	res/textures/cells.png
	res/textures/highScore400x60.png
	res/textures/numbers400x60.png
	res/textures/score400x100.png
)

foreach(TEXTURE_FILE ${TEXTURE_FILES})
	configure_file(${CMAKE_SOURCE_DIR}/${TEXTURE_FILE} ${CMAKE_BINARY_DIR}/${TEXTURE_FILE} COPYONLY)
endforeach()