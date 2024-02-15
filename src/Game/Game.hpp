#pragma once
#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <array>

#include "../Utilities/Clock.hpp"
#include "../Utilities/Score.hpp"
#include "../graphics/Texture.hpp"
#include "../graphics/ShaderProgram.hpp"
#include "../graphics/Sprite.hpp"

class Game {
    GLFWwindow* window;
    const unsigned int m_windowWidth;
    const unsigned int m_windowHeight;

    const float m_windowWidth_1_2 = m_windowWidth >> 1;
    const float m_windowWidth_1_4 = m_windowWidth >> 2;
    const float m_windowWidth_1_8 = m_windowWidth >> 3;
    const float m_windowWidth_1_18 = m_windowWidth / 18;
    const float m_windowWidth_1_20 = m_windowWidth / 20;
    const float m_windowWidth_1_40 = m_windowWidth / 40;
    const float m_windowWidth_3_4 = m_windowWidth_1_2 + m_windowWidth_1_4;
    const float m_windowWidth_5_8 = m_windowWidth_1_2 + m_windowWidth_1_8;
    const float m_windowWidth_7_8 = m_windowWidth_5_8 + m_windowWidth_1_4;
    const float m_windowWidth_17_20 = m_windowWidth - m_windowWidth_1_8 - m_windowWidth_1_40;
    const float m_windowWidth_19_20 = m_windowWidth - m_windowWidth_1_20;

    static const int FIELD_WIDTH = 10;
    static const int FIELD_HEIGHT = 20;
    static const int SPAWNZONE_WIDTH = 4;
    static const int SPAWNZONE_HEIGHT = 2;
    const int spawnZone_dx = (FIELD_WIDTH - SPAWNZONE_WIDTH) / 2;
    const int spawnZone_dy = FIELD_HEIGHT - SPAWNZONE_HEIGHT;

    struct Cell {
        bool used;
        bool canMove;
        int color;
    };

    Cell field[FIELD_HEIGHT][FIELD_WIDTH];
    Cell previousField[FIELD_HEIGHT][FIELD_WIDTH];
    Cell miniScreen[SPAWNZONE_HEIGHT][SPAWNZONE_WIDTH];

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

    std::array<std::array<bool, 8>, 7> figures = { {
        {0, 1, 1, 0, 0, 1, 1, 0}, // O
        {0, 1, 1, 1, 0, 0, 0, 1}, // L
        {0, 1, 1, 1, 0, 1, 0, 0}, // J
        {0, 1, 1, 0, 0, 0, 1, 1}, // S
        {0, 0, 1, 1, 0, 1, 1, 0}, // Z
        {1, 1, 1, 1, 0, 0, 0, 0}, // I
        {0, 1, 1, 1, 0, 0, 1, 0} // T
    } };
    size_t fallingFigureType;
    size_t numberOfColors = 6;

    bool shouldNewFigureBeSpawned;
    bool gameOver;
    
    Score score;
    Score highScore;
    std::string highScoreStr;
    score_t scorePerLine = 10;

    int fallingFigure_x1 = 0;
    int fallingFigure_y1 = 0;
    int fallingFigure_x2 = 0;
    int fallingFigure_y2 = 0;

    std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> shaderProgramMap;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;
    std::unordered_map<int, std::shared_ptr<Sprite>> cellSpriteMap;
    std::unordered_map<int, std::shared_ptr<Sprite>> numbersSpriteMap;

    void start();
    static void keysCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void handleKey(int key, int action);
    void showGame();
    void showScore();
    int genNextFigure(int color);
    int genNextColor();
    void spawnNextFigure(int figure, int color);
    bool moveFigure(int x, int y, int dx, int dy);
    bool rotateFigure();
    void saveFieldState();
    void returnPrevField();
    void makeFigureMotionless();
    void moveAllFiguresDownFrom(int y);
    void deleteLines();

public:
    Game(GLFWwindow* _window, int width, int height);
    void run();
};

#endif // !__GAME_HPP__