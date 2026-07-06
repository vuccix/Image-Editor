#pragma once

#include <Renderer/Texture.h>
#include <unordered_map>
#include <string>
#include <span>

class AssetManager {
public:
    AssetManager();

    void load(std::string name, uint32_t width, uint32_t height, std::span<const uint8_t> data);
    uint32_t get(const std::string& name) const;

private:
    std::unordered_map<std::string, Texture> m_textures;
};
