#pragma once

#include <cstdint>

class Renderer {
public:
    Renderer();
   ~Renderer();

private:
    uint32_t m_texture = 0;
};
