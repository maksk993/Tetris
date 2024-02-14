#include "Game.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

Game::Game(GLFWwindow* _window, int width, int height) : window(_window), m_windowWidth(width), m_windowHeight(height) {
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, keysCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    srand(time(NULL));

    textureMap["cell"] = std::make_shared<Texture>("res/textures/cells.png");
    textureMap["interface"] = std::make_shared<Texture>("res/textures/interface400x800.png");
    textureMap["numbers"] = std::make_shared<Texture>("res/textures/numbers400x46.png");

    shaderProgramMap["sprite"] = std::make_shared<ShaderProgram>("res/shaders/vSprite.txt",
                                                                "res/shaders/fSprite.txt");

    for (int i = 0; i < cellTexturesArray.size(); i++) {
        cellSpriteMap[i] = std::make_shared<Sprite>(textureMap["cell"], shaderProgramMap["sprite"],
        glm::vec2(0.f), glm::vec2(m_windowWidth_1_20), 0.f, cellTexturesArray[i]);
    }
    
    for (int i = 0; i < numbersTexturesArray.size(); i++) {
        numbersSpriteMap[i] = std::make_shared<Sprite>(textureMap["numbers"], shaderProgramMap["sprite"],
            glm::vec2(0.f), glm::vec2(m_windowWidth_1_20, m_windowWidth_1_18), 0.f, numbersTexturesArray[i]);
        numbersSpriteMap[i + numbersTexturesArray.size()] = std::make_shared<Sprite>(textureMap["numbers"], shaderProgramMap["sprite"],
            glm::vec2(0.f), glm::vec2(m_windowWidth_1_40, m_windowWidth_1_18 / 2), 0.f, numbersTexturesArray[i]);
    }

    numbersSpriteMap[-1] = std::make_shared<Sprite>(textureMap["interface"], shaderProgramMap["sprite"],
        glm::vec2(m_windowWidth_1_2, 0.f), glm::vec2(m_windowWidth_1_2, m_windowHeight), 0.f);

    std::ifstream highScoreFile("res/highScore.txt");
    if (!highScoreFile.is_open()) {
        std::cerr << "Failed to open file: res/highScore.txt" << std::endl;
        exit(1);
    }
    highScoreFile >> highScore;
    highScoreStr = std::to_string(highScore);
    highScoreFile.close();

    start();
}

void Game::start() {
    std::memset(field, 0, sizeof(field));
    std::memset(previousField, 0, sizeof(previousField));
    std::memset(miniScreen, 0, sizeof(miniScreen));

    gameOver = false;
    shouldNewFigureBeSpawned = true;
    score = 0;
}

void Game::run() {
    shaderProgramMap["sprite"]->use();
    shaderProgramMap["sprite"]->setInt("tex", 0);

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowWidth), 0.f, static_cast<float>(m_windowHeight),
                                           -1.f, 1.f);
    shaderProgramMap["sprite"]->setMatrix4("projectionMat", projectionMatrix);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    int nextColor = genNextColor();
    int nextFigure = genNextFigure(nextColor);

    Clock clock;
    float timer = 0.f;
    int delay = 400;
    
    while (!glfwWindowShouldClose(window)) {
        if (gameOver) {
            start();
        }
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

        float time = clock.getElapsedTime();
        clock.restart();
        timer += time;

        if (timer > delay) {
            handleKey(GLFW_KEY_DOWN, GLFW_PRESS);
            timer = 0;
        }

        if (shouldNewFigureBeSpawned) {
            deleteLines();
            spawnNextFigure(nextFigure, nextColor);
            nextColor = genNextColor();
            nextFigure = genNextFigure(nextColor);
            shouldNewFigureBeSpawned = false;
        }

        showGame();
        glfwSwapBuffers(window);
    }
}

void Game::keysCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    game->handleKey(key, action);
}

void Game::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Game::handleKey(int key, int action) {
    saveFieldState();

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
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
    for (size_t i = 0; i < FIELD_HEIGHT; i++) { // shows field
        for (size_t j = 0; j < FIELD_WIDTH; j++) {
            cellSpriteMap[field[i][j].color]->setPosition(glm::vec2(j * m_windowWidth_1_20, i * m_windowWidth_1_20));
            cellSpriteMap[field[i][j].color]->render();
        }
    }

    showScore(); // shows score

    for (size_t i = 0; i < SPAWNZONE_HEIGHT; i++) { // shows mini screen
        for (size_t j = 0; j < SPAWNZONE_WIDTH; j++) {
            cellSpriteMap[miniScreen[i][j].color]->setPosition(glm::vec2(j * m_windowWidth_1_20 + (FIELD_WIDTH + 3) * m_windowWidth_1_20,
                                                                         i * m_windowWidth_1_20 + (FIELD_WIDTH + 2) * m_windowWidth_1_20));
            cellSpriteMap[miniScreen[i][j].color]->render();
        }
    }
}

void Game::showScore() {
    numbersSpriteMap[-1]->render();
    if (score > highScore) writeNewHighScore();

    std::string scoreStr = std::to_string(score);
    float dx = 0.f;
    
    for (char& c : scoreStr) {
        int num = c - '0';
        numbersSpriteMap[num]->setPosition(glm::vec2(m_windowWidth_5_8 + dx, m_windowWidth_3_4));
        numbersSpriteMap[num]->render();
        dx += m_windowWidth_1_20;
    }

    dx = 0.f;
    for (char& c : highScoreStr) {
        int num = c - '0';
        numbersSpriteMap[num + numbersTexturesArray.size()]->setPosition(glm::vec2(m_windowWidth_17_20 + dx, m_windowWidth_19_20));
        numbersSpriteMap[num + numbersTexturesArray.size()]->render();
        dx += m_windowWidth_1_40;
    }
}
 
void Game::writeNewHighScore() {
    highScore = score;
    highScoreStr = std::to_string(highScore);

    std::ofstream highScoreFile("res/highScore.txt", std::ios::trunc);
    if (!highScoreFile.is_open()) {
        std::cerr << "Failed to open file: res/highScore.txt" << std::endl;
        exit(1);
    }
    highScoreFile << highScore;
    highScoreFile.close();
}

int Game::genNextFigure(int color) {
    std::memset(miniScreen, 0, sizeof(miniScreen));
    int figure = rand() % figures.size();

    for (int i = 0; i < SPAWNZONE_HEIGHT; i++)
        for (int j = 0; j < SPAWNZONE_WIDTH; j++)
            if (figures[figure][i * 4 + j]) {
                miniScreen[i][j].used = true;
                miniScreen[i][j].color = color;
            }

    return figure;
}

int Game::genNextColor() {
    int color = rand() % numberOfColors + 1;
    return color;
}

void Game::spawnNextFigure(int figure, int color) {
    for (int i = 0; i < SPAWNZONE_HEIGHT; i++) {
        int y = i + spawnZone_dy;
        for (int j = 0; j < SPAWNZONE_WIDTH; j++) {
            int x = j + spawnZone_dx;
            if (figures[figure][i * 4 + j]) {
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
    fallingFigureType = figure;
    fallingFigure_x1 = spawnZone_dx + 1;
    fallingFigure_y1 = spawnZone_dy - 1;
    if (fallingFigureType == 5) fallingFigure_x1 = spawnZone_dx, fallingFigure_y1 = spawnZone_dy - 2; // figures[5] == 'I'

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
    if (fallingFigureType == 0) return true; // figures[0] == 'O'

    if (fallingFigure_x1 < 0) handleKey(GLFW_KEY_RIGHT, GLFW_PRESS);
    if (fallingFigure_x2 > FIELD_WIDTH) handleKey(GLFW_KEY_LEFT, GLFW_PRESS);
    for (int i = fallingFigure_y1; i < fallingFigure_y2; i++) {
        if (field[i][fallingFigure_x2 - 1].used && !field[i][fallingFigure_x2 - 1].canMove)
            handleKey(GLFW_KEY_LEFT, GLFW_PRESS);
    }
    for (int i = fallingFigure_y1; i < fallingFigure_y2; i++) {
        if (field[i][fallingFigure_x1].used && !field[i][fallingFigure_x1].canMove)
            handleKey(GLFW_KEY_RIGHT, GLFW_PRESS);
    }

    size_t matrixWidth = 3;
    size_t matrixHeigth = 3; 
    if (fallingFigureType == 5) matrixWidth = 4, matrixHeigth = 4; // figures[5] == 'I'

    std::vector<std::vector<Cell>> CurrentMatrix(matrixHeigth, std::vector<Cell>(matrixWidth));

    for (int i = fallingFigure_y1, c_i = 0; c_i < matrixHeigth; i++, c_i++) {
        for (int j = fallingFigure_x1, c_j = 0; c_j < matrixWidth; j++, c_j++) {
            if (j < 0 || j >= FIELD_WIDTH || i < 0) return false;
            CurrentMatrix[c_i][c_j] = field[i][j];
        }
    }

    std::vector<std::vector<Cell>> RotatedMatrix(matrixWidth, std::vector<Cell>(matrixHeigth));

    for (int i = 0; i < matrixWidth; i++)
        for (int j = 0; j < matrixHeigth; j++)
            RotatedMatrix[i][j] = CurrentMatrix[j][matrixWidth - i - 1];

    for (int i = fallingFigure_y1, r_i = 0; r_i < matrixHeigth; i++, r_i++) {
        for (int j = fallingFigure_x1, r_j = 0; r_j < matrixWidth; j++, r_j++) {
            if ((field[i][j].used && !field[i][j].canMove) || (j < 0 || j >= FIELD_WIDTH || i < 0)) return false;
            field[i][j] = RotatedMatrix[r_i][r_j];
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
    int scoreMultiplier = 1;
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
            score += scorePerLine * scoreMultiplier++;
        }
    }   
}