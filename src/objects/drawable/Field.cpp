#include "Field.hpp"

Field::Field() : m_width(0), m_height(0), m_offset(0), m_pScore(nullptr), 
				 m_pHighScore(nullptr), m_position(glm::vec2(0.f)) {
}

std::vector<Field::Cell>& Field::operator[](size_t index) {
	return m_field[index];
}

void Field::init(const unsigned int width, const unsigned int height, 
				 std::vector<std::shared_ptr<Sprite>>& sprites, glm::vec2& position, float offset)
{
	m_width = width;
	m_height = height;

	m_field.resize(m_height);
	for (auto& i : m_field) {
		i.resize(m_width);
	}
	m_previousField = m_field;

	m_pSprites = sprites;
	m_position = std::move(position);
	m_offset = offset;
}

void Field::render() {
	for (size_t i = 0; i < m_height; i++) {
		for (size_t j = 0; j < m_width; j++) {
			m_pSprites[m_field[i][j].color]->setPosition(glm::vec2(m_position.x + j * m_offset, m_position.y + i * m_offset));
			m_pSprites[m_field[i][j].color]->render();
		}
	}
}

void Field::clear() {
	for (size_t i = 0; i < m_height; i++) {
		for (size_t j = 0; j < m_width; j++) {
			m_previousField[i][j].canMove = m_field[i][j].canMove = false;
			m_previousField[i][j].used = m_field[i][j].used = false;
			m_previousField[i][j].color = m_field[i][j].color = 0;
		}
	}
}

void Field::setScoreAndHighScore(Score* pScore, HighScore* pHighScore) {
	m_pScore = pScore;
	m_pHighScore = pHighScore;
}

void Field::saveField() {
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
			m_previousField[i][j] = m_field[i][j];
}

void Field::returnField() {
	std::swap(m_field, m_previousField);
}

void Field::makeFiguresMotionless() {
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
			m_field[i][j].canMove = false;
}

void Field::moveAllFiguresDownFrom(int y) {
	for (int i = y; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
			std::swap(m_field[i][j], m_field[i - 1][j]);
}

void Field::deleteLines() {
	int linesToDelete = 0;
	for (int i = 0; i < m_height; i++) {
		bool shouldLineBeDeleted = true;
		for (int j = 0; j < m_width; j++) {
			if (!m_field[i][j].used) {
				shouldLineBeDeleted = false;
				break;
			}
		}
		if (shouldLineBeDeleted) {
			for (int j = 0; j < m_width; j++) {
				m_field[i][j].used = false;
				m_field[i][j].color = 0;
			}
			moveAllFiguresDownFrom(i-- + 1);
			(*m_pScore) += (scorePerLine << linesToDelete++);	
		}
	}
	if ((*m_pScore) > (*m_pHighScore)) m_pHighScore->setHighScore((*m_pScore).getScore());
}