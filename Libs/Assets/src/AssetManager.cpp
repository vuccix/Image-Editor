#include <Assets/AssetManager.h>
#include <cassert>
#include <memory>

AssetManager::AssetManager() {
    m_textures.reserve(32);
}

void AssetManager::load(std::string name, const uint32_t w, const uint32_t h, const std::span<const uint8_t> data) {
    assert(name.empty() == false              && "Invalid texture name");
    assert(w > 0 && h > 0 && data.size() > 0  && "Invalid texture size or data");
    assert(m_textures.contains(name) == false && "Texture already loaded");

    m_textures.emplace(std::move(name), Texture{ w, h, data });
}

uint32_t AssetManager::get(const std::string& name) const {
    assert(name.empty() == false && "Invalid texture name");

    if (const auto it = m_textures.find(name); it != m_textures.end())
        return it->second.id();

    assert(false && "Incorrect texture name");
}
