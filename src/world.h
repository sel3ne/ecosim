#ifndef ECOSIM_WORLD_H
#define ECOSIM_WORLD_H

#include <SFML/Graphics.hpp>

#include "chunk_manager.h"
#include "entity.h"

class World {
 public:
  World();
  void render(sf::RenderWindow& window);
  void addEntityToEntities(Entity&& entity);

 private:
  ChunkManager chunk_manager_;
  std::vector<Entity> entities_;
};

#endif  // define ECOSIM_WORLD_H