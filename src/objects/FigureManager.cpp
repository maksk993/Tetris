#include "FigureManager.hpp"

FigureManager::FigureManager() : m_pField(nullptr), m_pMiniScreen(nullptr) {
}

void FigureManager::init(Field* pField, MiniScreen* pMiniScreen) {
	m_pField = pField;
	m_pMiniScreen = pMiniScreen;

    m_fieldWidth = m_pField->getWidth();
    m_fieldHeight = m_pField->getHeight();
    m_miniScreenWidth = m_pMiniScreen->getWidth();
    m_miniScreenHeight = m_pMiniScreen->getHeight();

    spawnZone_dx = (m_fieldWidth - m_miniScreenWidth) / 2;
    spawnZone_dy = m_fieldHeight - m_miniScreenHeight;
}

int FigureManager::genNextFigure(int color) {
    m_pMiniScreen->clear();
    int figure = rand() % figures.size();

    for (int i = 0; i < m_miniScreenHeight; i++)
        for (int j = 0; j < m_miniScreenWidth; j++)
            if (figures[figure][i * 4 + j]) {
                (*m_pMiniScreen)[i][j].used = true;
                (*m_pMiniScreen)[i][j].color = color;
            }

    return figure;
}

int FigureManager::genNextColor() {
    int color = rand() % numberOfColors + 1;
    return color;
}

void FigureManager::spawnNextFigure(int figure, int color) {
    for (int i = 0; i < m_miniScreenHeight; i++) {
        int y = i + spawnZone_dy;
        for (int j = 0; j < m_miniScreenWidth; j++) {
            int x = j + spawnZone_dx;
            if (figures[figure][i * 4 + j]) {
                if ((*m_pField)[y][x].used) {
                    m_gameOver = true;
                    return;
                }
                (*m_pField)[y][x].used = true;
                (*m_pField)[y][x].canMove = true;
                (*m_pField)[y][x].color = color;
            }
        }
    }
    fallingFigureType = figure;
    fallingFigure_x1 = spawnZone_dx + 1;
    fallingFigure_y1 = spawnZone_dy - 1;
    if (fallingFigureType == 5) fallingFigure_x1 = spawnZone_dx, fallingFigure_y1 = spawnZone_dy - 2; // figures[5] == 'I'

    fallingFigure_x2 = m_miniScreenWidth + spawnZone_dx;
    fallingFigure_y2 = m_miniScreenHeight + spawnZone_dy;
}

bool FigureManager::moveFigure(int x, int y, int dx, int dy) {
    if (x + dx < m_fieldWidth && x + dx >= 0 && y + dy >= 0 && !(*m_pField)[y + dy][x + dx].used) {
        std::swap((*m_pField)[y][x], (*m_pField)[y + dy][x + dx]);
        return true;
    }
    return false;
}

bool FigureManager::rotateFigure() {
    if (fallingFigureType == 0) return true; // figures[0] == 'O'

    if (fallingFigure_x1 < 0) handleKeyRight();
    if (fallingFigure_x2 > m_fieldWidth) handleKeyLeft();
    for (int i = fallingFigure_y1; i < fallingFigure_y2; i++) {
        if (fallingFigure_x2 - 1 >= m_fieldWidth || fallingFigure_y1 < 0) return true;
        if ((*m_pField)[i][fallingFigure_x2 - 1].used && !(*m_pField)[i][fallingFigure_x2 - 1].canMove)
            handleKeyLeft();
    }
    for (int i = fallingFigure_y1; i < fallingFigure_y2; i++) {
        if (fallingFigure_x1 < 0 || fallingFigure_y1 < 0) return true;
        if ((*m_pField)[i][fallingFigure_x1].used && !(*m_pField)[i][fallingFigure_x1].canMove)
            handleKeyRight();
    }

    size_t matrixWidth = 3;
    size_t matrixHeigth = 3;
    if (fallingFigureType == 5) matrixWidth = 4, matrixHeigth = 4; // figures[5] == 'I'

    std::vector<std::vector<Field::Cell>> CurrentMatrix(matrixHeigth, std::vector<Field::Cell>(matrixWidth));

    for (int i = fallingFigure_y1, c_i = 0; c_i < matrixHeigth; i++, c_i++) {
        for (int j = fallingFigure_x1, c_j = 0; c_j < matrixWidth; j++, c_j++) {
            if (j < 0 || j >= m_fieldWidth || i < 0) return false;
            CurrentMatrix[c_i][c_j] = (*m_pField)[i][j];
        }
    }

    std::vector<std::vector<Field::Cell>> RotatedMatrix(matrixWidth, std::vector<Field::Cell>(matrixHeigth));

    for (int i = 0; i < matrixWidth; i++)
        for (int j = 0; j < matrixHeigth; j++)
            RotatedMatrix[i][j] = CurrentMatrix[j][matrixWidth - i - 1];

    for (int i = fallingFigure_y1, r_i = 0; r_i < matrixHeigth; i++, r_i++) {
        for (int j = fallingFigure_x1, r_j = 0; r_j < matrixWidth; j++, r_j++) {
            if (((*m_pField)[i][j].used && !(*m_pField)[i][j].canMove) || (j < 0 || j >= m_fieldWidth || i < 0)) return false;
            (*m_pField)[i][j] = RotatedMatrix[r_i][r_j];
        }
    }

    return true;
}

void FigureManager::handleKeyDown() {
    m_pField->saveField();
    for (int i = 0; i < m_fieldHeight; i++) {
        for (int j = 0; j < m_fieldWidth; j++) {
            if ((*m_pField)[i][j].canMove)
                if (!moveFigure(j, i, 0, -1)) {
                    m_pField->returnField();
                    m_shouldNewFigureBeSpawned = true;
                    m_pField->makeFiguresMotionless();
                    return;
                }
        }
    }
    fallingFigure_y1--;
    fallingFigure_y2--;
}

void FigureManager::handleKeyUp() {
    while (!m_shouldNewFigureBeSpawned) {
        handleKeyDown();
    }
}

void FigureManager::handleKeyLeft() {
    m_pField->saveField();
    for (int i = 0; i < m_fieldHeight; i++) {
        for (int j = 0; j < m_fieldWidth; j++) {
            if ((*m_pField)[i][j].canMove)
                if (!moveFigure(j, i, -1, 0)) {
                    m_pField->returnField();
                    return;
                }
        }
    }
    fallingFigure_x1--;
    fallingFigure_x2--;
}

void FigureManager::handleKeyRight() {
    m_pField->saveField();
    for (int i = 0; i < m_fieldHeight; i++) {
        for (int j = m_fieldWidth - 1; j >= 0; j--) {
            if ((*m_pField)[i][j].canMove)
                if (!moveFigure(j, i, 1, 0)) {
                    m_pField->returnField();
                    return;
                }
        }
    }
    fallingFigure_x1++;
    fallingFigure_x2++;
}

void FigureManager::handleKeySpace() {
    m_pField->saveField();
    if (!rotateFigure()) {
        m_pField->returnField();
        return;
    }
}