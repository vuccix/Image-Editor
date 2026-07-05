#pragma once

#include <cstdint>
#include <span>

class Texture {
public:
    Texture(uint32_t width, uint32_t height, std::span<const uint32_t> rgbaPixels);
   ~Texture();

    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    uint32_t id()     const noexcept;
    uint32_t width()  const noexcept;
    uint32_t height() const noexcept;

private:
    uint32_t m_textureId = 0;
    uint32_t m_width     = 0;
    uint32_t m_height    = 0;
};
