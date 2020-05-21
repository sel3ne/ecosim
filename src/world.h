#ifndef ECOSIM_WORLD_H
#define ECOSIM_WORLD_H

#include <SFML/Graphics.hpp>

#include "chunk_manager.h"

class World {
 public:
  void render(sf::RenderWindow& window);

 private:
  ChunkManager chunk_manager_;
};

#endif  // define ECOSIM_WORLD_H