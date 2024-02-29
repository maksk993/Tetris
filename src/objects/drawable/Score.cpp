#include "Score.hpp"

Score::Score() : m_score(0), m_offset(0), m_scorePerLine(0), m_pathToHighScoreFile(""), m_position(glm::vec2(0.f)) {
}

bool Score::operator>(Score other) {
    return m_score > other.m_score;
}

void Score::init(std::vector<std::shared_ptr<Sprite>>& sprites, glm::vec2& position, float offset) {
    m_pNumbersSprites = std::move(sprites);
    m_position = std::move(position);
    m_offset = offset;
}

void Score::render() {
    float dx = 0.f;

    for (char& c : getScoreString()) {
        int num = c - '0';
        m_pNumbersSprites[num]->setPosition(glm::vec2(m_position.x + dx, m_position.y));
        m_pNumbersSprites[num]->render();
        dx += m_offset;
    }
}

void Score::setScore(score_t value) {
    m_score = value;
}

void Score::setPathToFile(const std::string& path) {
    m_pathToHighScoreFile = path;
    std::ifstream highScoreFile(m_pathToHighScoreFile);
    if (!highScoreFile.is_open()) {
        std::cerr << "Failed to set score from file: " << m_pathToHighScoreFile << std::endl;
        return;
    }
    highScoreFile >> m_score;
    highScoreFile.close();
}

score_t Score::getScore() {
    return m_score;
}

std::string Score::getScoreString() {
    return std::to_string(m_score);
}

void Score::setScorePerLine(score_t scorePerLine) {
    m_scorePerLine = scorePerLine;
}

void Score::increaseScore(int leftShiftBy) {
    m_score += m_scorePerLine << leftShiftBy;
}

void Score::writeScoreToFile() {
    std::ofstream highScoreFile(m_pathToHighScoreFile, std::ios::trunc);
    if (!highScoreFile.is_open()) {
        std::cerr << "Failed to write score to file: " << m_pathToHighScoreFile << std::endl;
        return;
    }
    highScoreFile << m_score;
    highScoreFile.close();
}