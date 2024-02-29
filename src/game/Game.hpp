#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

#include "../objects/drawable/Score.hpp"
#include "../objects/drawable/Field.hpp"
#include "../objects/drawable/Text.hpp"
#include "../objects/FigureManager.hpp"

#include "../graphics/Texture.hpp"
#include "../graphics/ShaderProgram.hpp"
#include "../graphics/Sprite.hpp"

#include "../utilities/Clock.hpp"
#include "../utilities/FlexibleSizes.hpp"

class Game {
public:
    Game(GLFWwindow* _window, size_t width, size_t height);
    ~Game();
    void run();

private:
    GLFWwindow* window;
    const size_t m_windowWidth;
    const size_t m_windowHeight;

    static const size_t FIELD_WIDTH = 10;
    static const size_t FIELD_HEIGHT = 20;
    static const size_t MINISCREEN_WIDTH = 4;
    static const size_t MINISCREEN_HEIGHT = 2;

    std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> shaderProgramMap;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;

    FigureManager m_figureManager;
    Field m_field;
    MiniScreen m_miniScreen;
    Text m_text;
    Score m_score;
    HighScore m_highScore;

    std::array<std::array<GLfloat, 8>, 7> cellTexturesArray = { {
        {0.f, 0.f,   1.f / 7, 0.f,   1.f / 7, 1.f,   0.f, 1.f},
        {1.f / 7, 0.f,   2.f / 7, 0.f,   2.f / 7, 1.f,   1.f / 7, 1.f},
        {2.f / 7, 0.f,   3.f / 7, 0.f,   3.f / 7, 1.f,   2.f / 7, 1.f},
        {3.f / 7, 0.f,   4.f / 7, 0.f,   4.f / 7, 1.f,   3.f / 7, 1.f},
        {4.f / 7, 0.f,   5.f / 7, 0.f,   5.f / 7, 1.f,   4.f / 7, 1.f},
        {5.f / 7, 0.f,   6.f / 7, 0.f,   6.f / 7, 1.f,   5.f / 7, 1.f},
        {6.f / 7, 0.f,   1.f, 0.f,   1.f, 1.f,   6.f / 7, 1.f}
    } };

    std::array<std::array<GLfloat, 8>, 10> numbersTexturesArray = { {
        {0.f, 0.f,   0.1f, 0.f,   0.1f, 1.f,   0.f, 1.f},
        {0.1f, 0.f,   0.2f, 0.f,   0.2f, 1.f,   0.1f, 1.f},
        {0.2f, 0.f,   0.3f, 0.f,   0.3f, 1.f,   0.2f, 1.f},
        {0.3f, 0.f,   0.4f, 0.f,   0.4f, 1.f,   0.3f, 1.f},
        {0.4f, 0.f,   0.5f, 0.f,   0.5f, 1.f,   0.4f, 1.f},
        {0.5f, 0.f,   0.6f, 0.f,   0.6f, 1.f,   0.5f, 1.f},
        {0.6f, 0.f,   0.7f, 0.f,   0.7f, 1.f,   0.6f, 1.f},
        {0.7f, 0.f,   0.8f, 0.f,   0.8f, 1.f,   0.7f, 1.f},
        {0.8f, 0.f,   0.9f, 0.f,   0.9f, 1.f,   0.8f, 1.f},
        {0.9f, 0.f,   1.f, 0.f,   1.f, 1.f,   0.9f, 1.f}
    } };

    Clock clock;
    float delay;
    size_t fallenFiguresCounter;

    void start();
    static void keysCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void handleKey(int key, int action);
    void showGame();
    void increaseSpeed();
};