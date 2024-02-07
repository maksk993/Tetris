#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

#include "Game/Game.hpp"
//#include <glm/vec2.hpp>

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    //glm::ivec2 windowSize(800, 800);
    int windowWidth = 800;
    int windowHeigth = 800;

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeigth, "Tetris", nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    Game game(window, windowWidth, windowHeigth);

    game.run();

    glfwTerminate();
    return 0;
}