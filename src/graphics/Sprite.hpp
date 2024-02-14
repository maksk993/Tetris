#pragma once
#ifndef __SPRITE_HPP__
#define __SPRITE_HPP__

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.hpp"
#include "ShaderProgram.hpp"
#include <memory>
#include <array>

class Sprite {
public:
	Sprite(std::shared_ptr<Texture> pTexture, std::shared_ptr<ShaderProgram> pShaderProgram,
		const glm::vec2& position = glm::vec2(0.f),
		const glm::vec2& size = glm::vec2(1.f),
		const float rotation = 0.f,
		const std::array<GLfloat, 8>& userTexCoords = std::array<GLfloat, 8>{0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f}
	);
	~Sprite();

	Sprite(const Sprite&) = delete;
	Sprite& operator=(const Sprite&) = delete;

	void render() const;
	void setPosition(const glm::vec2& position);
	void setSize(const glm::vec2& size);
	void setRotation(const float rotation);

private:
	std::shared_ptr<Texture> m_pTexture;
	std::shared_ptr<ShaderProgram> m_pShaderProgram;
	glm::vec2 m_position;
	glm::vec2 m_size;
	float m_rotation;

	GLuint m_VAO;
	GLuint m_vertexVBO;
	GLuint m_texCoordsVBO;
};

#endif // !__SPRITE_HPP__