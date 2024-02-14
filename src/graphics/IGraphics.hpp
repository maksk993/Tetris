#pragma once
#ifndef __IRESOURCESLOADER_HPP__
#define __IRESOURCESLOADER_HPP__

#include <glad/glad.h>

#include <iostream>
#include <string>

class IGraphics { // Интерфейс, который определяет методы для классов шейдеров, текстур
protected:
	GLuint m_ID = 0;
	virtual std::string load(const std::string& path) = 0;
public:
	virtual GLuint getID() const = 0;
	virtual ~IGraphics() = default;
};

#endif // !__IRESOURCESLOADER_HPP__