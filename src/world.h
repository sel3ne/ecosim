#ifndef ECOSIM_WORLD_H
#define ECOSIM_WORLD_H

#include <SFML/Graphics.hpp>

#include "chunk_manager.h"
#include "entity.h"

class World {
 public:
  World();
  void render(sf::RenderWindow& window);
  void addEntityToEntities(std::unique_ptr<Entity> entity);

  void addNumberLighthouse(int i);
  int returnNumberLighthouse();

  void addNumberHappyHouse(int i);
  int returnNumberHappyHouse();

  void addNumberUnhappyHouse(int i);
  int returnNumberUnhappyHouse();

  void addNumberHappyHuman(int i);
  int returnNumberHappyHuman();

  void addNumberUnhappyHuman(int i);
  int returnNumberUnhappyHuman();

  void update(float time_s);

 private:
  ChunkManager chunk_manager_;
  std::vector<std::unique_ptr<Entity>> entities_;
  int number_happy_house_;
  int number_unhappy_house_;
  int number_happy_human_;
  int number_unhappy_human_;
  int number_lighthouse_;
};

#endif  // define ECOSIM_WORLD_H
