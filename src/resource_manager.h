#ifndef ECOSIM_RESOURCE_MANAGER_H
#define ECOSIM_RESOURCE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <string_view>
#include <unordered_map>

enum TextureID { TEXTURE_GRASS, TEXTURE_HOUSE };

class ResourceManager {
 public:
  ResourceManager();

  sf::Texture* getTexture(TextureID id);

 private:
  std::unordered_map<TextureID, sf::Texture> textures_;
};

extern ResourceManager* gResourceManager;

#endif
