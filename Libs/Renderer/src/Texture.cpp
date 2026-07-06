#include <Renderer/Texture.h>
#include <glad/glad.h>
#include <utility>

Texture::Texture(const uint32_t width, const uint32_t height, const std::span<const uint8_t> rgbaPixels)
        : m_width(width), m_height(height) {

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

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
    if (m_texture != 0)
        glDeleteTextures(1, &m_texture);
}

Texture::Texture(Texture&& other) noexcept
        : m_texture(std::exchange(other.m_texture, 0)),
          m_width(std::exchange(other.m_width, 0)),
          m_height(std::exchange(other.m_height, 0)) {}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (m_texture != 0)
            glDeleteTextures(1, &m_texture);

        m_texture = std::exchange(other.m_texture, 0);
        m_width   = std::exchange(other.m_width,   0);
        m_height  = std::exchange(other.m_height,  0);
    }

    return *this;
}

void Texture::resize(const uint32_t width, const uint32_t height) {
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // allocate memory on the GPU without uploading data
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height),
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr
    );

    m_width  = width;
    m_height = height;
}

void Texture::update(const std::span<const uint8_t> rgbaPixels) {
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // replace existing GPU pixels
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0, 0,
        static_cast<GLsizei>(m_width),
        static_cast<GLsizei>(m_height),
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        rgbaPixels.data()
    );
}

uint32_t Texture::id()     const noexcept { return m_texture; }
uint32_t Texture::width()  const noexcept { return m_width;   }
uint32_t Texture::height() const noexcept { return m_height;  }
