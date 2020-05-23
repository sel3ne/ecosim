#include "world.h"

#include <iostream>

World::World()
    : number_happy_house_(0),
      number_unhappy_house_(0),
      number_happy_human_(0),
      number_unhappy_human_(0),
      number_lighthouse_(0) {}

void World::render(sf::RenderWindow& window) {
  chunk_manager_.renderTiles(window);
  for (std::unique_ptr<Entity>& n : entities_) {
    n->render(window);
  }
}

void World::update(float time_s) {
  for (std::unique_ptr<Entity>& n : entities_) {
    n->update(time_s);
  }
}

void World::addEntityToEntities(std::unique_ptr<Entity> entity) {
  entities_.push_back(std::move(entity));
}

void World::addNumberLighthouse() { number_lighthouse_++; }

void World::removeNumberLighthouse() { number_lighthouse_--; }

int World::returnNumberLighthouse() { return number_lighthouse_; }
