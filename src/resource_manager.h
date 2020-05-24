#ifndef ECOSIM_RESOURCE_MANAGER_H
#define ECOSIM_RESOURCE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <string_view>
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
};

class ResourceManager {
 public:
  ResourceManager();

  sf::Texture* getTexture(TextureId id);

 private:
  std::unordered_map<TextureId, sf::Texture> textures_;
};

extern ResourceManager* gResourceManager;

#endif
