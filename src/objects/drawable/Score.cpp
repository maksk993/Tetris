#include "Score.hpp"

Score::Score() : m_offset(0), m_scorePerLine(0), m_pathToHighScoreFile(""), m_position(glm::vec2(0.f)) {
}

bool Score::operator>(Score other) {
    return m_value > other.m_value;
}

void Score::init(std::vector<std::shared_ptr<Sprite>>& sprites, glm::vec2& position, float offset) {
    m_pNumbersSprites = std::move(sprites);
    m_position = std::move(position);
    m_offset = offset;
}

void Score::render() {
    float dx = 0.f;

    for (char& c : getValueString()) {
        int num = c - '0';
        m_pNumbersSprites[num]->setPosition(glm::vec2(m_position.x + dx, m_position.y));
        m_pNumbersSprites[num]->render();
        dx += m_offset;
    }
}


void Score::setPathToFile(const std::string& path) {
    m_pathToHighScoreFile = path;
    std::ifstream highScoreFile(m_pathToHighScoreFile);
    if (!highScoreFile.is_open()) {
        std::cerr << "Failed to set score from file: " << m_pathToHighScoreFile << std::endl;
        return;
    }
    highScoreFile >> m_value;
    highScoreFile.close();
}

void Score::setScorePerLine(int scorePerLine) {
    m_scorePerLine = scorePerLine;
}

void Score::increaseValue(int leftShiftBy) {
    m_value += m_scorePerLine << leftShiftBy;
}

void Score::writeScoreToFile() {
    std::ofstream highScoreFile(m_pathToHighScoreFile, std::ios::trunc);
    if (!highScoreFile.is_open()) {
        std::cerr << "Failed to write score to file: " << m_pathToHighScoreFile << std::endl;
        return;
    }
    highScoreFile << m_value;
    highScoreFile.close();
}