#include "Score.hpp"

Score::Score() {
    score = 0;
    path = "";
}

void Score::operator+=(score_t value) {
    score += value;
}

bool Score::operator>(Score other) {
    return score > other.score;
}

void Score::setScore(score_t value) {
    score = value;
}

void Score::setScore(const std::string& path) {
    this->path = path;
    std::ifstream highScoreFile(path);
    if (!highScoreFile.is_open()) {
        std::cerr << "Failed to set score from file: " << path << std::endl;
        return;
    }
    highScoreFile >> score;
    highScoreFile.close();
}

void Score::setHighScore(score_t value) {
    setScore(value);
    writeScoreToFile();
}

score_t Score::getScore() {
    return score;
}

std::string Score::getScoreString() {
    return std::to_string(score);
}

void Score::writeScoreToFile() {
    std::ofstream highScoreFile(path, std::ios::trunc);
    if (!highScoreFile.is_open()) {
        std::cerr << "Failed to write score to file: " << path << std::endl;
        return;
    }
    highScoreFile << score;
    highScoreFile.close();
}