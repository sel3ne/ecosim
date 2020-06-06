#ifndef ECOSIM_RESOURCE_MANAGER_H
#define ECOSIM_RESOURCE_MANAGER_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <unordered_map>

enum TextureId {
  TEXTURE_GRASS,
  TEXTURE_WATER,
  TEXTURE_STONE,
  TEXTURE_HOUSE,
  TEXTURE_LIGHTHOUSE,
  TEXTURE_HUMAN_HAPPY,
  TEXTURE_HUMAN_UNHAPPY,
  TEXTURE_FARMHOUSE,
  TEXTURE_FARM,
  TEXTURE_ONE,
  TEXTURE_STONE_BAR,
  TEXTURE_GRAIN,
};

enum FontId {
  FONT_COURIER,
  FONT_COURIER_BOLD,
};

class ResourceManager {
 public:
  ResourceManager();

  sf::Texture* getTexture(TextureId id);
  sf::Font* getFont(FontId id);

 private:
  std::unordered_map<TextureId, sf::Texture> textures_;
  std::unordered_map<FontId, sf::Font> fonts_;
};

extern ResourceManager* gResourceManager;

#endif
