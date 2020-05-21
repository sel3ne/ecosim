#include "world.h"

#include <iostream>

World::World() {
  // sf::Texture* texture1 = new sf::Texture;
  // if (!texture1->loadFromFile("data/green-grass-textures_74190-5443.png")) {
  //   std::cout << "could not load the test texture";
  // }
  // sf::Texture* texture2 = new sf::Texture;
  // if (!texture2->loadFromFile("data/Greekheroichouse.png")) {
  //   std::cout << "could not load the test texture";
  // }

  // Entity entity1 = Entity(1.0, 1.0, texture1);
  // Entity entity2 = Entity(700.0, 1.0, texture2);
  // entities_ = {entity1, entity2};
}

void World::render(sf::RenderWindow& window) {
  chunk_manager_.renderTiles(window);
  for (Entity& n : entities_) {
    n.render(window);
  }
}

void World::addEntityToEntities(Entity&& entity) {
  entities_.push_back(entity);
}