#pragma once

#include "NumerableObject.hpp"

class Speed : public NumerableObject {
public:
	Speed();

	void init(std::vector<std::shared_ptr<Sprite>>& sprites, glm::vec2& position, float offset);
	virtual void render() override;
	virtual void increaseValue(int incr) override;

private:
	std::vector<std::shared_ptr<Sprite>> m_pNumbersSprites;
	glm::vec2 m_position;
	float m_offset;
};