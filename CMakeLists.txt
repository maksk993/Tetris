cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME Tetris)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/res/highScore.txt)
	file(WRITE ${CMAKE_SOURCE_DIR}/res/highScore.txt "0")
endif()
file(COPY ${CMAKE_SOURCE_DIR}/res/highScore.txt DESTINATION ${CMAKE_BINARY_DIR}/res)

add_executable(${PROJECT_NAME} 
	"src/main.cpp"
	"src/game/Game.cpp"

	"src/graphics/ShaderProgram.cpp" 
	"src/graphics/Texture.cpp" 
	"src/graphics/VBO.cpp"
	"src/graphics/VAO.cpp"
	"src/graphics/Sprite.cpp"
	"src/graphics/Renderer.cpp"

	"src/objects/FigureManager.cpp"
	"src/objects/drawable/Score.cpp"
	"src/objects/drawable/Field.cpp" 
	"src/objects/drawable/Text.cpp"
 
	"src/utilities/Clock.cpp"
	"src/utilities/FlexibleSizes.cpp"
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glad)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)

add_subdirectory(res/shaders)
add_subdirectory(res/textures)

target_link_libraries(${PROJECT_NAME} glad glfw glm)