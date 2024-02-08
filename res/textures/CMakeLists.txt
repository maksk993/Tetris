#res/textures/CMakeLists.txt

set(TEXTURE_FILES res/textures/kubiki.png res/textures/kubiki2.png)

foreach(TEXTURE_FILE ${TEXTURE_FILES})
	configure_file(${CMAKE_SOURCE_DIR}/${TEXTURE_FILE} ${CMAKE_BINARY_DIR}/${TEXTURE_FILE} COPYONLY)
endforeach()