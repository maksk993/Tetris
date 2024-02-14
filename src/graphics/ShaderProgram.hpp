#pragma once
#ifndef __SHADERPROGRAM_HPP__
#define __SHADERPROGRAM_HPP__

#include "IGraphics.hpp"

#include <fstream>
#include <sstream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram : public IGraphics { // Работа с шейдерами
public:
    ShaderProgram(const std::string& vertexShaderSourcePath, const std::string& fragmentShaderSourcePath);
    ~ShaderProgram();

    ShaderProgram() = delete;
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ShaderProgram(ShaderProgram&&) noexcept;
    ShaderProgram& operator=(ShaderProgram&&) noexcept;

    virtual GLuint getID() const override;
    void use() const;
    void setInt(const std::string& name, const GLint value);
    void setMatrix4(const std::string& name, const glm::mat4& matrix);

private:
    virtual std::string load(const std::string& path) override;
    bool createShader(const char* shaderSource, GLenum shaderType, GLuint& shaderID);
    void createShaderProgram(GLuint& vertexShaderID, GLuint& fragmentShaderID);
};

#endif // !__SHADERPROGRAM_HPP__