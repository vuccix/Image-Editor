#pragma once

#include <Renderer/Texture.h>
#include <vector>
#include <span>

enum class Asset {
    Checkerboard,
    Count
};

class AssetManager {
public:
    AssetManager();

    void load(uint32_t width, uint32_t height, std::span<const uint8_t> data);
    uint32_t get(Asset name) const;

private:
    std::vector<Texture> m_textures;
};
