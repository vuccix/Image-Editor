#include <Assets/AssetManager.h>
#include <cassert>
#include <memory>

AssetManager::AssetManager() {
    m_textures.reserve(32);
}

void AssetManager::load(const uint32_t w, const uint32_t h, const std::span<const uint8_t> data) {
    assert(w > 0 && h > 0 && data.size() > 0  && "Invalid texture size or data");

    m_textures.emplace_back(w, h, data);
}

uint32_t AssetManager::get(const Asset name) const {
    return m_textures[static_cast<int32_t>(name)].id();
}
