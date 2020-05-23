#include "resource_manager.h"

#include <iostream>
#include <string_view>

ResourceManager* gResourceManager = nullptr;

const std::map<TextureID, std::string> kTexturePaths = {
    {TEXTURE_GRASS, "data/green-grass-textures_74190-5443.png"},
    {TEXTURE_HOUSE, "data/Greekheroichouse.png"},
    {TEXTURE_LIGHTHOUSE, "data/MTS_blennus-1383251-LighthouseMain.png"},
    {TEXTURE_HUMAN, "data/happy-smile-png-2.png"},
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
