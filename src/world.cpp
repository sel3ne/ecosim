#include "world.h"

#include <iostream>

World::World() {
  sf::Texture* texture = new sf::Texture;
  if (!texture->loadFromFile("data/green-grass-textures_74190-5443.png")) {
    std::cout << "could not load the test texture";
  }

  entity_ = new Entity(1.0, 1.0, texture);
}

void World::render(sf::RenderWindow& window) {
  chunk_manager_.renderTiles(window);
  entity_->render(window);
}