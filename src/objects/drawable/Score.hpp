#pragma once

#include <fstream>
#include <iostream>
#include "IDrawableObject.hpp"

using score_t = int;

class Score : public IDrawableObject {
public:
	Score();

	bool operator>(Score other);

	void init(std::vector<std::shared_ptr<Sprite>>& sprites, glm::vec2& position, float offset);
	virtual void render() override;
	void setScore(score_t value);
	void setPathToFile(const std::string& path);
	void writeScoreToFile();
	score_t getScore();
	std::string getScoreString();
	void setScorePerLine(score_t scorePerLine);
	void increaseScore(int leftShiftBy);

private:
	score_t m_score;
	std::string m_pathToHighScoreFile;
	score_t m_scorePerLine;
	
	std::vector<std::shared_ptr<Sprite>> m_pNumbersSprites;
	glm::vec2 m_position;
	float m_offset;
};

using HighScore = Score;