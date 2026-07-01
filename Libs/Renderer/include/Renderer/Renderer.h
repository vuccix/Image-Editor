#pragma once

#include <cstdint>

struct Image;

class Renderer {
public:
    Renderer();
   ~Renderer();

    void resize(int32_t width, int32_t height);
    void upload(const Image& img) const;
    uint32_t textureID() const;

private:
    uint32_t m_texture = 0;
};
