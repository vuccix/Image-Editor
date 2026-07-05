#pragma once

#include <cstdint>

struct Image;

class Renderer {
public:
    Renderer();
   ~Renderer();

    void resize(uint32_t width, uint32_t height);
    void upload(const Image& img) const;
    uint32_t textureID() const noexcept;

private:
    uint32_t m_texture = 0;
};
