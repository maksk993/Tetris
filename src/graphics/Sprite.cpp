#include "Sprite.hpp"

Sprite::Sprite(std::shared_ptr<Texture> pTexture, std::shared_ptr<ShaderProgram> pShaderProgram, 
	const glm::vec2& position, 
	const glm::vec2& size, 
	const float rotation,
	const std::array<GLfloat, 8>& userTexCoords)
	: m_pTexture(std::move(pTexture)), m_pShaderProgram(std::move(pShaderProgram)), 
	m_position(position), m_size(size), m_rotation(rotation)
{
	const GLfloat vertices[] = {
		0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f
	};

	const GLfloat texCoords[] = {
		userTexCoords[0],
		userTexCoords[1],
		userTexCoords[2],
		userTexCoords[3],
		userTexCoords[4],
		userTexCoords[5],
		userTexCoords[6],
		userTexCoords[7]
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &m_texCoordsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), &texCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Sprite::~Sprite() {
	glDeleteBuffers(1, &m_vertexVBO);
	glDeleteBuffers(1, &m_texCoordsVBO);
	glDeleteVertexArrays(1, &m_VAO);
}

void Sprite::render() const {
	m_pShaderProgram->use();

	glm::mat4x4 model(1.f);

	model = glm::translate(model, glm::vec3(m_position, 0.f));
	model = glm::translate(model, glm::vec3(0.5f * m_size.x, 0.5f * m_size.y, 0.f));
	model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.f, 0.f, 1.f));
	model = glm::translate(model, glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.f));
	model = glm::scale(model, glm::vec3(m_size, 1.f));

	glBindVertexArray(m_VAO);
	m_pShaderProgram->setMatrix4("modelMat", model);

	glActiveTexture(GL_TEXTURE0);
	m_pTexture->bind();

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}

void Sprite::setPosition(const glm::vec2& position) {
	m_position = position;
}

void Sprite::setSize(const glm::vec2& size) {
	m_size = size;
}

void Sprite::setRotation(const float rotation) {
	m_rotation = rotation;
}