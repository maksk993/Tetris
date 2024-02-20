#pragma once

#include "../../graphics/Sprite.hpp"

class IDrawableObject {
public:
	virtual void render() = 0;
	virtual ~IDrawableObject() = default;
};