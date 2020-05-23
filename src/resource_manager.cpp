#include "resource_manager.h"

#include <iostream>
#include <string_view>

ResourceManager* gResourceManager = nullptr;

const std::map<TextureID, std::string> kTexturePaths = {
    {TEXTURE_GRASS, "data/grass64.png"},
    {TEXTURE_WATER, "data/water64.png"},
    {TEXTURE_STONE, "data/stone64.png"},
    {TEXTURE_HOUSE, "data/Greekheroichouse.png"},
    {TEXTURE_LIGHTHOUSE, "data/MTS_blennus-1383251-LighthouseMain.png"},
    {TEXTURE_HUMAN_HAPPY, "data/happy-smile-png-2.png"},
    {TEXTURE_HUMAN_UNHAPPY, "data/unhappy_human.png"},
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
