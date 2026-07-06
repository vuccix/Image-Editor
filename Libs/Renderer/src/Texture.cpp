#include <Renderer/Texture.h>
#include <glad/glad.h>
#include <utility>

Texture::Texture(const uint32_t width, const uint32_t height, const std::span<const uint8_t> rgbaPixels)
        : m_width(width), m_height(height) {

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height),
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        rgbaPixels.data()
    );

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    if (m_textureId != 0)
        glDeleteTextures(1, &m_textureId);
}

Texture::Texture(Texture&& other) noexcept
        : m_textureId(std::exchange(other.m_textureId, 0)),
          m_width(std::exchange(other.m_width, 0)),
          m_height(std::exchange(other.m_height, 0)) {}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (m_textureId != 0)
            glDeleteTextures(1, &m_textureId);

        m_textureId = std::exchange(other.m_textureId, 0);
        m_width     = std::exchange(other.m_width,     0);
        m_height    = std::exchange(other.m_height,    0);
    }

    return *this;
}

uint32_t Texture::id()     const noexcept { return m_textureId; }
uint32_t Texture::width()  const noexcept { return m_width;     }
uint32_t Texture::height() const noexcept { return m_height;    }
