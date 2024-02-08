#include "Game.hpp"

//#include <glm/mat4x4.hpp>
//#include <glm/gtc/matrix_transform.hpp>

Game::Game(GLFWwindow* _window, int width, int height) : window(_window), m_windowWidth(width), m_windowHeight(height) {
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, keysCallback);
    srand(time(NULL));

    spawnZone_dx = (FIELD_WIDTH - SPAWNZONE_WIDTH) / 2;
    spawnZone_dy = FIELD_HEIGHT - SPAWNZONE_HEIGHT;

    figuresArray[0] = Figure(Figures::O);
    figuresArray[1] = Figure(Figures::L);
    figuresArray[2] = Figure(Figures::J);
    figuresArray[3] = Figure(Figures::S);
    figuresArray[4] = Figure(Figures::Z);
    figuresArray[5] = Figure(Figures::I);
    figuresArray[6] = Figure(Figures::T);

    textureMap["tex1"] = std::make_shared<Texture>("res/textures/kubiki.png");

    start();
}

void Game::start() {
    std::memset(field, 0, sizeof(field));
    std::memset(previousField, 0, sizeof(previousField));
    std::memset(miniScreen, 0, sizeof(miniScreen));

    gameOver = false;
    shouldNewFigureBeSpawned = true;
}

void Game::run() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    textureMap["tex1"]->bind();
    
    int nextColor = genNextColor();
    int nextFigure = genNextFigure(nextColor);

    Clock clock;
    float timer = 0.f;
    int delay = 400;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0.8f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (gameOver) {
            start();
        }

        float time = clock.getElapsedTime();
        clock.restart();
        timer += time;

        if (shouldNewFigureBeSpawned) {
            deleteLines();
            spawnNextFigure(nextFigure, nextColor);
            nextColor = genNextColor();
            nextFigure = genNextFigure(nextColor);
            shouldNewFigureBeSpawned = false;
        }

        if (timer > delay) {
            handleKey(GLFW_KEY_DOWN, GLFW_PRESS);
            timer = 0;
        }

        glfwPollEvents();
        showGame();
        glfwSwapBuffers(window);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_2D_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

void Game::keysCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    game->handleKey(key, action);
}

void Game::handleKey(int key, int action) {
    saveFieldState();

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        for (int i = 0; i < FIELD_HEIGHT; i++) {
            for (int j = 0; j < FIELD_WIDTH; j++) {
                if (field[i][j].canMove)
                    if (!moveFigure(j, i, 0, -1)) {
                        returnPrevField();
                        shouldNewFigureBeSpawned = true;
                        makeFigureMotionless();
                        return;
                    }
            }
        }
        fallingFigure_y1--;
        fallingFigure_y2--;
    }
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        while (!shouldNewFigureBeSpawned) {
            handleKey(GLFW_KEY_DOWN, GLFW_PRESS);
        } 
    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        for (int i = 0; i < FIELD_HEIGHT; i++) {
            for (int j = 0; j < FIELD_WIDTH; j++) {
                if (field[i][j].canMove)
                    if (!moveFigure(j, i, -1, 0)) {
                        returnPrevField();
                        return;
                    }
            }
        }
        fallingFigure_x1--;
        fallingFigure_x2--;
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        for (int i = 0; i < FIELD_HEIGHT; i++) {
            for (int j = FIELD_WIDTH - 1; j >= 0; j--) {
                if (field[i][j].canMove)
                    if (!moveFigure(j, i, 1, 0)) {
                        returnPrevField();
                        return;
                    }
            }
        }
        fallingFigure_x1++;
        fallingFigure_x2++;
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        if (!rotateFigure()) {
            returnPrevField();
            return;
        }
    }
}

void Game::showGame() {
    glLoadIdentity();
    glScalef(2.f / FIELD_HEIGHT, 2.f / FIELD_HEIGHT, 1.f);
    glTranslatef(-FIELD_HEIGHT * 0.5f, -FIELD_HEIGHT * 0.5f, 0.f);

    for (size_t i = 0; i < FIELD_HEIGHT; i++) {
        for (size_t j = 0; j < FIELD_WIDTH; j++) {
            glPushMatrix();
            glTranslatef(j, i, 0.0f);

            if (field[i][j].used) {
                showCell(true, field[i][j].color);
            }
            else {
                showCell(false);
            }

            glPopMatrix();
        }
    }

    glTranslatef(FIELD_WIDTH + 3, FIELD_WIDTH + 2, 0);

    for (size_t i = 0; i < SPAWNZONE_HEIGHT; i++) {
        for (size_t j = 0; j < SPAWNZONE_WIDTH; j++) {
            glPushMatrix();
            glTranslatef(j, i, 0.0f);

            if (miniScreen[i][j].used)
                showCell(true, miniScreen[i][j].color);
            else {
                showCell(false);
            }

            glPopMatrix();
        }
    }
}

void Game::showCell(bool isUsed, int color) {
    if (isUsed) glTexCoordPointer(2, GL_FLOAT, 0, texCoord + 8 * color);
    else glTexCoordPointer(2, GL_FLOAT, 0, texCoord);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

int Game::genNextFigure(int color) {
    std::memset(miniScreen, 0, sizeof(miniScreen));
    int figure = rand() % figuresArraySize;

    for (int i = 0; i < SPAWNZONE_HEIGHT; i++)
        for (int j = 0; j < SPAWNZONE_WIDTH; j++)
            if (figuresArray[figure].figureCells[i][j]) {
                miniScreen[i][j].used = true;
                miniScreen[i][j].color = color;
            }

    return figure;
}

int Game::genNextColor() {
    int color = rand() % colorNum + 1;
    return color;
}

void Game::spawnNextFigure(int figure, int color) {
    for (int i = 0; i < SPAWNZONE_HEIGHT; i++) {
        int y = i + spawnZone_dy;
        for (int j = 0; j < SPAWNZONE_WIDTH; j++) {
            int x = j + spawnZone_dx;
            if (figuresArray[figure].figureCells[i][j]) {
                if (field[y][x].used) {
                    gameOver = true;
                    return;
                }
                field[y][x].used = true;
                field[y][x].canMove = true;
                field[y][x].color = color;
            }
        }
    }
    fallingFigureType = figuresArray[figure].type;
    fallingFigure_x1 = spawnZone_dx + 1;
    fallingFigure_y1 = spawnZone_dy - 1;
    if (fallingFigureType == Figures::I) fallingFigure_x1 = spawnZone_dx, fallingFigure_y1 = spawnZone_dy - 2;

    fallingFigure_x2 = SPAWNZONE_WIDTH + spawnZone_dx;
    fallingFigure_y2 = SPAWNZONE_HEIGHT + spawnZone_dy;
}

bool Game::moveFigure(int x, int y, int dx, int dy) {
    if (x + dx < FIELD_WIDTH && x + dx >= 0 && y + dy >= 0 && !field[y + dy][x + dx].used) {
        std::swap(field[y][x], field[y + dy][x + dx]);
        return true;
    }
    return false;
}

bool Game::rotateFigure() {
    if (fallingFigureType == Figures::O) return true;

    if (fallingFigure_x1 < 0) handleKey(GLFW_KEY_RIGHT, GLFW_PRESS);
    if (fallingFigure_x2 > FIELD_WIDTH) handleKey(GLFW_KEY_LEFT, GLFW_PRESS);

    size_t matrixWidth = 3;
    size_t matrixHeigth = 3; 
    if (fallingFigureType == Figures::I) matrixWidth = 4, matrixHeigth = 4;

    std::vector<std::vector<Cell>> CurrentMatrix(matrixHeigth, std::vector<Cell>(matrixWidth));

    for (int i = fallingFigure_y1, c_i = 0; c_i < matrixHeigth; i++, c_i++) {
        for (int j = fallingFigure_x1, c_j = 0; c_j < matrixWidth; j++, c_j++) {
            if (j < 0 || j >= FIELD_WIDTH || i < 0) return false;
            CurrentMatrix[c_i][c_j] = field[i][j];
        }
    }

    std::vector<std::vector<Cell>> TransposedMatrix(matrixWidth, std::vector<Cell>(matrixHeigth));

    for (int i = 0; i < matrixWidth; i++)
        for (int j = 0; j < matrixHeigth; j++)
            TransposedMatrix[i][j] = CurrentMatrix[j][matrixWidth - i - 1];

    for (int i = fallingFigure_y1, t_i = 0; t_i < matrixHeigth; i++, t_i++) {
        for (int j = fallingFigure_x1, t_j = 0; t_j < matrixWidth; j++, t_j++) {
            if ((field[i][j].used && !field[i][j].canMove) || (j < 0 || j >= FIELD_WIDTH || i < 0)) return false;
            field[i][j] = TransposedMatrix[t_i][t_j];
        }
    }

    return true;
}

void Game::saveFieldState() {
    for (int i = 0; i < FIELD_HEIGHT; i++)
        for (int j = 0; j < FIELD_WIDTH; j++)
            previousField[i][j] = field[i][j];
}

void Game::returnPrevField() {
    std::swap(field, previousField);
}

void Game::makeFigureMotionless() {
    for (int i = 0; i < FIELD_HEIGHT; i++)
        for (int j = 0; j < FIELD_WIDTH; j++)
            field[i][j].canMove = false;
}

void Game::moveAllFiguresDownFrom(int y) {
    for (int i = y; i < FIELD_HEIGHT; i++)
        for (int j = 0; j < FIELD_WIDTH; j++)
            std::swap(field[i][j], field[i - 1][j]);
}

void Game::deleteLines() {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        bool shouldLineBeDeleted = true;
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (!field[i][j].used) {
                shouldLineBeDeleted = false;
                break;
            }
        }
        if (shouldLineBeDeleted) {
            for (int j = 0; j < FIELD_WIDTH; j++) {
                field[i][j].used = false;
                field[i][j].color = 0;
            }
            moveAllFiguresDownFrom(i-- + 1);
        }
    }   
}