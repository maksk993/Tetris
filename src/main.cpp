#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

#include "Game/Game.hpp"

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glm::vec2 windowSize(800, 800);

    GLFWwindow* window = glfwCreateWindow(windowSize.x, windowSize.y, "Tetris", nullptr, nullptr);

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

    Game game(window, windowSize.x, windowSize.y);

    game.run();

    glfwTerminate();
    return 0;
}