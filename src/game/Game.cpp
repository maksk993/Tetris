﻿#include "Game.hpp"

Game::Game(GLFWwindow* _window, size_t width, size_t height) : window(_window), m_windowWidth(width), m_windowHeight(height) {
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, keysCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    srand(time(NULL));

    textureMap["cell"] = std::make_shared<Texture>("res/textures/cells.png");
    textureMap["interface"] = std::make_shared<Texture>("res/textures/interface400x800.png");
    textureMap["numbers"] = std::make_shared<Texture>("res/textures/numbers400x46.png");

    shaderProgramMap["sprite"] = std::make_shared<ShaderProgram>("res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
    
    std::vector<std::shared_ptr<Sprite>> cellSprites(cellTexturesArray.size());
    std::vector<std::shared_ptr<Sprite>> scoreSprites(numbersTexturesArray.size());
    std::vector<std::shared_ptr<Sprite>> highScoreSprites(numbersTexturesArray.size());

    std::shared_ptr<Sprite> textSprite = std::make_shared<Sprite>(
        textureMap["interface"], 
        shaderProgramMap["sprite"],
        glm::vec2(FlexibleSizes::getSize(m_windowWidth, 2), 0.f), 
        glm::vec2(FlexibleSizes::getSize(m_windowWidth, 2), m_windowHeight), 
        0.f
    );

    for (int i = 0; i < cellTexturesArray.size(); i++) {
        cellSprites[i] = std::make_shared<Sprite>(
            textureMap["cell"], 
            shaderProgramMap["sprite"],
            glm::vec2(0.f), 
            glm::vec2(FlexibleSizes::getSize(m_windowWidth, 20)), 
            0.f, 
            cellTexturesArray[i]
        );
    }

    for (int i = 0; i < numbersTexturesArray.size(); i++) {
        scoreSprites[i] = std::make_shared<Sprite>(
            textureMap["numbers"], 
            shaderProgramMap["sprite"],
            glm::vec2(0.f), 
            glm::vec2(FlexibleSizes::getSize(m_windowWidth, 20), FlexibleSizes::getSize(m_windowHeight, 18)),
            0.f, 
            numbersTexturesArray[i]
        );
        highScoreSprites[i] = std::make_shared<Sprite>(
            textureMap["numbers"], 
            shaderProgramMap["sprite"],
            glm::vec2(0.f), 
            glm::vec2(FlexibleSizes::getSize(m_windowWidth, 40), FlexibleSizes::getSize(m_windowHeight, 36)), 
            0.f, 
            numbersTexturesArray[i]
        );
    }

    m_text.init(textSprite);

    m_field.init(
        FIELD_WIDTH, 
        FIELD_HEIGHT, 
        cellSprites, 
        glm::vec2(0.f), 
        FlexibleSizes::getSize(m_windowWidth, 20)
    );

    m_miniScreen.init(
        MINISCREEN_WIDTH, 
        MINISCREEN_HEIGHT, 
        cellSprites, 
        glm::vec2((FIELD_WIDTH + 3) * FlexibleSizes::getSize(m_windowWidth, 20), (FIELD_WIDTH + 1) * FlexibleSizes::getSize(m_windowWidth, 20)),
        FlexibleSizes::getSize(m_windowWidth, 20)
    );

    m_score.init(
        scoreSprites, 
        glm::vec2(FlexibleSizes::getSize(5 * m_windowWidth, 8), FlexibleSizes::getSize(3 * m_windowHeight, 4)),
        FlexibleSizes::getSize(m_windowWidth, 20)
    );

    m_highScore.init(
        highScoreSprites, 
        glm::vec2(FlexibleSizes::getSize(17 * m_windowWidth, 20), FlexibleSizes::getSize(19 * m_windowHeight, 20)), 
        FlexibleSizes::getSize(m_windowWidth, 40)
    );

    m_score.setScorePerLine(10);
    m_highScore.setPathToFile("res/highScore.txt");
    m_field.setScore(&m_score);
    m_figureManager.init(&m_field, &m_miniScreen);

    start();
}

void Game::start() {
    m_field.clear();
    m_miniScreen.clear();
    m_figureManager.setGameOver(false);
    m_figureManager.setShouldNewFigureBeSpawned(true);
    m_score.setScore(0);
    fallenFiguresCounter = 0;
    delay = 1000;
}

void Game::run() {
    shaderProgramMap["sprite"]->use();
    shaderProgramMap["sprite"]->setInt("tex", 0);

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowWidth), 0.f, static_cast<float>(m_windowHeight),
                                           -1.f, 1.f);
    shaderProgramMap["sprite"]->setMatrix4("projectionMat", projectionMatrix);

    Renderer::clearColor(0.2f, 0.2f, 0.2f, 1.0f);
    int nextColor = m_figureManager.genNextColor();
    int nextFigure = m_figureManager.genNextFigure(nextColor);

    float timer = 0.f;
    
    while (!glfwWindowShouldClose(window)) {
        if (m_figureManager.isGameOver()) {
            start();
        }
        Renderer::clear();

        glfwPollEvents();

        float time = clock.getElapsedTime();
        clock.restart();
        timer += time;

        if (timer > delay) {
            handleKey(GLFW_KEY_DOWN, GLFW_PRESS);
            timer = 0;
        }

        if (m_figureManager.shouldNewFigureBeSpawned()) {
            m_field.deleteLines();
            increaseSpeed();
            if (m_score > m_highScore) m_highScore.setScore(m_score.getScore());
            m_figureManager.spawnNextFigure(nextFigure, nextColor);
            nextColor = m_figureManager.genNextColor();
            nextFigure = m_figureManager.genNextFigure(nextColor);
            m_figureManager.setShouldNewFigureBeSpawned(false);
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
    Renderer::viewport(0, 0, width, height);
}

void Game::handleKey(int key, int action) {
    switch (key) {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, true);
        break;
    case GLFW_KEY_DOWN:
        m_figureManager.handleKeyDown();
        break;
    case GLFW_KEY_UP:
        if (action == GLFW_PRESS) m_figureManager.handleKeyUp();
        break;
    case GLFW_KEY_LEFT:
        if (action == GLFW_PRESS) m_figureManager.handleKeyLeft();
        break;
    case GLFW_KEY_RIGHT:
        if (action == GLFW_PRESS) m_figureManager.handleKeyRight();
        break;
    case GLFW_KEY_SPACE:
        if (action == GLFW_PRESS) m_figureManager.handleKeySpace();
        break;
    }
}

void Game::showGame() {
    m_field.render();
    m_text.render();
    m_score.render();
    m_highScore.render();
    m_miniScreen.render();
}

void Game::increaseSpeed() {
    if (++fallenFiguresCounter % 50 == 0) delay *= 0.8;
}

Game::~Game() {
    m_highScore.writeScoreToFile();
}