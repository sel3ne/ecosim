#include "world.h"

#include <iostream>

#include "human.h"

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

void World::doForAllEntities(std::function<void(Entity&)> func) {
  for (std::unique_ptr<Entity>& n : entities_) {
    func(*n);
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

void World::addNumberLighthouse(int i) {
  number_lighthouse_ = number_lighthouse_ + i;
}
int World::returnNumberLighthouse() { return number_lighthouse_; }

void World::addNumberHappyHouse(int i) {
  number_happy_house_ = number_happy_house_ + i;
}
int World::returnNumberHappyHouse() { return number_happy_house_; }

void World::addNumberUnhappyHouse(int i) {
  number_unhappy_house_ = number_unhappy_house_ + i;
}
int World::returnNumberUnhappyHouse() { return number_unhappy_house_; }

void World::addNumberHappyHuman(int i) {
  number_happy_human_ = number_happy_human_ + i;
}
int World::returnNumberHappyHuman() { return number_happy_human_; }

void World::addNumberUnhappyHuman(int i) {
  number_unhappy_human_ = number_unhappy_human_ + i;
}
int World::returnNumberUnhappyHuman() { return number_unhappy_human_; }

void World::addNumberFarmhouse(int i) {
  number_farmhouse_ = number_farmhouse_ + i;
}
int World::returnNumberFarmhouse() { return number_farmhouse_; }

void World::addNumberFarm(int i) { number_farm_ = number_farm_ + i; }
int World::returnNumberFarm() { return number_farm_; }
