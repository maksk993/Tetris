#pragma once

#include <fstream>
#include <iostream>
#include <string>

using score_t = int;

class Score {
public:
	Score();

	void operator+=(score_t value);
	bool operator>(Score other);

	void setScore(score_t value);
	void setScore(const std::string& path);
	void setHighScore(score_t value);
	score_t getScore();
	std::string getScoreString();

private:
	score_t score;
	std::string path;
	void writeScoreToFile();
};