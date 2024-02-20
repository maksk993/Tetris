#include "Text.hpp"

void Text::init(std::shared_ptr<Sprite> sprite) {
	m_sprite = std::move(sprite);
}

void Text::render() {
	m_sprite->render();
}