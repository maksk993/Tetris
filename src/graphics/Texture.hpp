#pragma once
#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include "IGraphics.hpp"
#include "stb_image.h"

class Texture : public IGraphics { // Работа с текстурами
public:
	Texture(const std::string& texturePath,
        const unsigned int channels = 4,
        const GLenum filter = GL_LINEAR,
        const GLenum wrapmode = GL_CLAMP_TO_EDGE);
    ~Texture();
    
    Texture() = delete;
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&&) noexcept;
    Texture& operator=(Texture&&) noexcept;

    virtual GLuint getID() const override;
    void bind() const;

private:
    GLenum m_filter;
    GLenum m_wrapmode;
    GLenum m_mode;

    unsigned int m_windowWidth;
    unsigned int m_windowHeight;

    virtual std::string load(const std::string& path) override;
    void createTexture(const unsigned int channels, const unsigned char* pixels);
};

#endif // !__TEXTURE_HPP__