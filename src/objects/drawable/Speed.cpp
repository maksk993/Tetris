#include "Speed.hpp"

Speed::Speed() : m_position(glm::vec2(0.f)), m_offset(0) {
}

void Speed::init(std::vector<std::shared_ptr<Sprite>>& sprites, glm::vec2& position, float offset) {
	m_pNumbersSprites = std::move(sprites);
	m_position = std::move(position);
	m_offset = offset;
}

void Speed::render() {
    float dx = 0.f;

    for (char& c : getValueString()) {
        int num = c - '0';
        m_pNumbersSprites[num]->setPosition(glm::vec2(m_position.x + dx, m_position.y));
        m_pNumbersSprites[num]->render();
        dx += m_offset;
    }
}

void Speed::increaseValue(int incr) {
	m_value += incr;
}