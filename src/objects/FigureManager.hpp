#pragma once

#include "drawable/Field.hpp"
#include <cstdlib>
#include <time.h>

class FigureManager {
public:
	FigureManager();

	void init(Field* pField, MiniScreen* pMiniScreen);
    int genNextFigure(int color);
    int genNextColor();
    void spawnNextFigure(int figure, int color);
    bool moveFigure(int x, int y, int dx, int dy);
    void rotateFigure();

    void handleKeyDown();
    void handleKeyUp();
    void handleKeyLeft();
    void handleKeyRight();
    void handleKeySpace();

    bool shouldNewFigureBeSpawned() { return m_shouldNewFigureBeSpawned; }
    bool isGameOver() { return m_gameOver; }
    void setShouldNewFigureBeSpawned(bool state) { m_shouldNewFigureBeSpawned = state; }
    void setGameOver(bool state) { m_gameOver = state; }

private:
    Field* m_pField;
    MiniScreen* m_pMiniScreen;

    bool m_shouldNewFigureBeSpawned = false;
    bool m_gameOver = false;

    std::array<std::array<bool, 8>, 7> figures = { {
        {0, 1, 1, 0, 0, 1, 1, 0}, // O
        {0, 1, 1, 1, 0, 0, 0, 1}, // L
        {0, 1, 1, 1, 0, 1, 0, 0}, // J
        {0, 1, 1, 0, 0, 0, 1, 1}, // S
        {0, 0, 1, 1, 0, 1, 1, 0}, // Z
        {1, 1, 1, 1, 0, 0, 0, 0}, // I
        {0, 1, 1, 1, 0, 0, 1, 0} // T
    } };
    
    size_t numberOfColors = 6;

    size_t spawnZone_dx = 0;
    size_t spawnZone_dy = 0;

    size_t m_fieldWidth = 0;
    size_t m_fieldHeight = 0;
    size_t m_miniScreenWidth = 0;
    size_t m_miniScreenHeight = 0;

    size_t fallingFigureType = 0;
    int fallingFigure_x1 = 0; // left bottom x
    int fallingFigure_y1 = 0; // left bottom y
    int fallingFigure_x2 = 0; // right top x
    int fallingFigure_y2 = 0; // right top y
};