#include "resource_manager.h"

#include <iostream>
#include <string_view>

ResourceManager* gResourceManager = nullptr;

const std::map<TextureId, std::string> kTexturePaths = {
    {TEXTURE_GRASS, "data/grass64.png"},
    {TEXTURE_WATER, "data/water64.png"},
    {TEXTURE_STONE, "data/stone64.png"},
    {TEXTURE_HOUSE, "data/Greekheroichouse.png"},
    {TEXTURE_LIGHTHOUSE, "data/MTS_blennus-1383251-LighthouseMain.png"},
    {TEXTURE_HUMAN_HAPPY, "data/happy-smile-png-2.png"},
    {TEXTURE_HUMAN_UNHAPPY, "data/unhappy_human.png"},
    {TEXTURE_FARMHOUSE, "data/farmhouse.png"},
    {TEXTURE_FARM, "data/farm_field.png"},
    {TEXTURE_ONE, "data/one.png"},
    {TEXTURE_STONE_BAR, "data/stone_bar.png"},
    {TEXTURE_GRAIN, "data/grain.png"},
};

const std::map<FontId, std::string> kFontPaths = {
    {FONT_COURIER, "data/cour.ttf"},
    {FONT_COURIER_BOLD, "data/courbd.ttf"},
};

ResourceManager::ResourceManager() {
  for (const auto& [id, path] : kTexturePaths) {
    sf::Texture& texture = textures_[id];
    if (!texture.loadFromFile(path)) {
      std::cout << "Failed to load texture from: " << path << "!" << std::endl;
      exit(1);
    }
  }
  for (const auto& [id, path] : kFontPaths) {
    sf::Font& font = fonts_[id];
    if (!font.loadFromFile(path)) {
      std::cout << "Failed to load font from: " << path << "!" << std::endl;
      exit(1);
    }
  }
}

sf::Texture* ResourceManager::getTexture(TextureId id) {
  return &textures_.at(id);
}

sf::Font* ResourceManager::getFont(FontId id) { return &fonts_.at(id); }
