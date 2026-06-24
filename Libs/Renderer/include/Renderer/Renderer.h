#pragma once

#include <Canvas/Image.h>
#include <cstdint>

class Renderer {
public:
    Renderer();
   ~Renderer();

    void upload(const Image& img) const;
    uint32_t textureID() const;

private:
    uint32_t m_texture = 0;
};
