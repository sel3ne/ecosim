#include "resource_manager.h"

#include <iostream>
#include <string_view>

const std::map<TextureID, std::string> kTexturePaths = {
    {TEXTURE_GRASS, "data/green-grass-textures_74190-5443.png"},
    {TEXTURE_HOUSE, "data/Greekheroichouse.png"},
};

ResourceManager::ResourceManager() {
  for (const auto& [id, path] : kTexturePaths) {
    sf::Texture& texture = textures_[id];
    if (!texture.loadFromFile(path)) {
      std::cout << "Failed to load texture from: " << path << "!" << std::endl;
      exit(1);
    }
  }
}

sf::Texture* ResourceManager::getTexture(TextureID id) {
  return &textures_.at(id);
}
