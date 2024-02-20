#pragma once

#include <fstream>
#include <iostream>
#include "IDrawableObject.hpp"

using score_t = int;

class Score : public IDrawableObject {
public:
	Score();

	void operator+=(score_t value);
	bool operator>(Score other);

	void init(std::vector<std::shared_ptr<Sprite>>& sprites, glm::vec2& position, float offset);
	virtual void render() override;
	void setScore(score_t value);
	void setPathToFile(const std::string& path);
	void setHighScore(score_t value);
	score_t getScore();
	std::string getScoreString();

private:
	score_t m_score;
	std::string m_pathToHighScoreFile;
	
	std::vector<std::shared_ptr<Sprite>> m_pNumbersSprites;
	glm::vec2 m_position;
	float m_offset;

	void writeScoreToFile();
};

using HighScore = Score;