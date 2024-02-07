cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME Tetris)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} 
	"src/main.cpp"
	"src/Game/Game.hpp"
	"src/Game/Game.cpp"
	#"src/graphics/IGraphics.hpp"
	#"src/graphics/ShaderProgram.hpp"
	#"src/graphics/ShaderProgram.cpp" 
	"src/graphics/stb_image.h" 
	"src/graphics/Texture.hpp" 
	"src/graphics/Texture.cpp" 
	#"src/graphics/Sprite.hpp"
	#"src/graphics/Sprite.cpp"
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glad)
add_subdirectory(external/glfw)
#add_subdirectory(external/glm)

#add_subdirectory(res/shaders)
add_subdirectory(res/textures)

target_link_libraries(${PROJECT_NAME} glad glfw) # +glm