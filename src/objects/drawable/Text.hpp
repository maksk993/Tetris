#pragma once

#include "IDrawableObject.hpp"

class Text : public IDrawableObject {
public:
	Text() = default;

	void init(std::shared_ptr<Sprite> sprite);
	virtual void render() override;

private:
	std::shared_ptr<Sprite> m_sprite;
};