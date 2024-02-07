#pragma once
#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>

#include "figures.hpp"
#include "../graphics/Texture.hpp"
//#include "../graphics/ShaderProgram.hpp"
//#include "../graphics/Sprite.hpp"

class Game {
    GLFWwindow* window;
    int m_windowWidth;
    int m_windowHeight;

    static const int FIELD_WIDTH = 10;
    static const int FIELD_HEIGHT = 20;
    static const int SPAWNZONE_WIDTH = 4;
    static const int SPAWNZONE_HEIGHT = 2;
    int spawnZone_dx;
    int spawnZone_dy;

    struct Cell {
        bool used;
        bool canMove;
        int color;
    };

    Cell field[FIELD_HEIGHT][FIELD_WIDTH];
    Cell previousField[FIELD_HEIGHT][FIELD_WIDTH];
    Cell miniScreen[2][4];

    GLfloat vertices[8] = {
        0.f, 0.f,   1.f, 0.f,   1.f, 1.f,   0.f, 1.f
    };

    GLfloat texCoord[56] = {
        0.f, 0.f,   1.f/7, 0.f,   1.f/7, 1.f,   0.f, 1.f,
        1.f/7, 0.f,   2.f/7, 0.f,   2.f/7, 1.f,   1.f/7, 1.f,
        2.f/7, 0.f,   3.f/7, 0.f,   3.f/7, 1.f,   2.f/7, 1.f,
        3.f/7, 0.f,   4.f/7, 0.f,   4.f/7, 1.f,   3.f/7, 1.f,
        4.f/7, 0.f,   5.f/7, 0.f,   5.f/7, 1.f,   4.f/7, 1.f,
        5.f/7, 0.f,   6.f/7, 0.f,   6.f/7, 1.f,   5.f/7, 1.f,
        6.f/7, 0.f,   1.f, 0.f,   1.f, 1.f,   6.f/7, 1.f,
    };

    int colorNum = 6;
    int figuresArraySize = 7;
    Figure figuresArray[7];
    Figures fallingFigureType;

    bool shouldNewFigureBeSpawned;
    bool gameOver;

    int fallingFigure_x1 = 0;
    int fallingFigure_y1 = 0;
    int fallingFigure_x2 = 0;
    int fallingFigure_y2 = 0;

   // std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> shaderProgramMap;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;
   // std::unordered_map<std::string, std::shared_ptr<Sprite>> spriteMap;

    void start();
    static void keysCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void handleKey(int key, int action);
    void showGame();
    void showCell(bool isUsed, int color = 0);
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