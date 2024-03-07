#pragma once

#include <fstream>
#include <iostream>
#include "NumerableObject.hpp"

class Score : public NumerableObject {
public:
	Score();

	bool operator>(Score other);

	void init(std::vector<std::shared_ptr<Sprite>>& sprites, glm::vec2& position, float offset);
	virtual void render() override;
	void setPathToFile(const std::string& path);
	void writeScoreToFile();
	void setScorePerLine(int scorePerLine);
	virtual void increaseValue(int leftShiftBy) override;

private:
	std::string m_pathToHighScoreFile;
	int m_scorePerLine;
	
	std::vector<std::shared_ptr<Sprite>> m_pNumbersSprites;
	glm::vec2 m_position;
	float m_offset;
};

using HighScore = Score;