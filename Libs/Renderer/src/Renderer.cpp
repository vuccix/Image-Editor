#include <Renderer/Renderer.h>
#include <glad/glad.h>

Renderer::Renderer() {
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

Renderer::~Renderer() {
    if (m_texture != 0)
        glDeleteTextures(1, &m_texture);
}

uint32_t Renderer::textureID() const {
    return m_texture;
}

void Renderer::upload(const Image& img) const {
    glBindTexture(GL_TEXTURE_2D, m_texture);

    if (img.channels == 1) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);
    }
    else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
    }

    GLenum format; GLint internal;
    switch (img.channels) {
        case 3:  format = GL_RGB;  internal = GL_RGB8;  break;
        case 4:  format = GL_RGBA; internal = GL_RGBA8; break;
        default: format = GL_RED;  internal = GL_R8;    break;
    }

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internal,
        img.width,
        img.height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        img.pixels.data()
    );
}
