#include <Renderer/Renderer.h>
#include <Canvas/Image.h>
#include <glad/glad.h>

Renderer::Renderer() {
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
}

Renderer::~Renderer() {
    if (m_texture != 0)
        glDeleteTextures(1, &m_texture);
}

uint32_t Renderer::textureID() const {
    return m_texture;
}

void Renderer::resize(const int32_t width, const int32_t height) {
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // allocate memory on the GPU without uploading data
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr
    );
}

void Renderer::upload(const Image& img) const {
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // replace existing GPU pixels
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0, 0,
        img.width,
        img.height,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        img.pixels.data()
    );
}
