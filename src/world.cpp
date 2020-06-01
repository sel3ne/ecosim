#include "world.h"

#include <iostream>

#include "building.h"
#include "farmhouse.h"
#include "human.h"

World::World()
    : number_happy_house_(0),
      number_unhappy_house_(0),
      number_happy_human_(0),
      number_unhappy_human_(0),
      number_lighthouse_(0) {}

void World::render(sf::RenderWindow& window) {
  chunk_manager_.renderTiles(window);
  for (std::unique_ptr<Constructible>& n : constructibles_) {
    n->render(window);
  }
  for (std::unique_ptr<Human>& n : humans_) {
    n->render(window);
  }
}

void World::doForAllEntities(std::function<void(Entity&)> func) {
  for (std::unique_ptr<Human>& n : humans_) {
    func(*n);
  }
  for (std::unique_ptr<Constructible>& n : constructibles_) {
    func(*n);
  }
}

void World::doForAllHumans(std::function<void(Human&)> func) {
  for (std::unique_ptr<Human>& n : humans_) {
    func(*n);
  }
}

void World::doForAllConstructibles(std::function<void(Constructible&)> func) {
  for (std::unique_ptr<Constructible>& n : constructibles_) {
    func(*n);
  }
}

void World::update(float time_s) {
  for (std::unique_ptr<Human>& n : humans_) {
    n->update(time_s);
  }
  for (std::unique_ptr<Constructible>& n : constructibles_) {
    n->update(time_s);
  }
}

void World::addEntityToEntities(std::unique_ptr<Entity> entity) {
  if (entity->typeOfEntity() == Entity::HUMAN) {
    Human* human = dynamic_cast<Human*>(entity.release());
    humans_.emplace_back(human);
  } else {
    Constructible* constructible =
        dynamic_cast<Constructible*>(entity.release());
    constructibles_.emplace_back(constructible);
  }
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

int World::returnNumberHappyHuman() {
  return happy_employed_humans_.size() + happy_unemployed_humans_.size();
}

int World::returnNumberUnhappyHuman() {
  return unhappy_employed_humans_.size() + unhappy_unemployed_humans_.size();
}

void World::addNumberFarmhouse(int i) {
  number_farmhouse_ = number_farmhouse_ + i;
}
int World::returnNumberFarmhouse() { return number_farmhouse_; }

void World::addNumberFarm(int i) { number_farm_ = number_farm_ + i; }
int World::returnNumberFarm() { return number_farm_; }

template <typename EntityClass>
EntityClass* World::buildConstructible(int x_grid, int y_grid, int w_grid,
                                       int h_grid,
                                       Entity::EntityType entity_type) {
  // Check if any tile under the new Constructible is already occupied.
  for (int x = 0; x < w_grid; x++) {
    for (int y = 0; y < h_grid; y++) {
      Tile& tile = chunk_manager_.getTile(x_grid + x, y_grid + y);
      if (tile.returnOccupied()) {
        // We can't build here.
        return nullptr;
      }
    }
  }

  // Build the Constructible.
  std::unique_ptr<Entity> constructible = std::make_unique<EntityClass>(
      x_grid, y_grid, w_grid, h_grid, entity_type);
  EntityClass* constructible_ptr =
      dynamic_cast<EntityClass*>(constructible.get());

  addEntityToEntities(std::move(constructible));

  // Occupy tiles under Constructible.
  for (int x = 0; x < w_grid; x++) {
    for (int y = 0; y < h_grid; y++) {
      Tile& tile = chunk_manager_.getTile(x_grid + x, y_grid + y);
      tile.setOccupied(true);
    }
  }

  return constructible_ptr;
}

// Generate instantiations of the templated function for our building types.
// This is necessary as the call sides of World::buildConstructible don't have
// access to the code in this cpp file. E.g. Game::buildConstructibleAtMouse
// calls this function, but cannot generate versions for the different types, as
// the code is in a different cpp file.
template Building* World::buildConstructible<Building>(
    int x_grid, int y_grid, int w_grid, int h_grid,
    Entity::EntityType entity_type);
template Farmhouse* World::buildConstructible<Farmhouse>(
    int x_grid, int y_grid, int w_grid, int h_grid,
    Entity::EntityType entity_type);
template Farm* World::buildConstructible<Farm>(int x_grid, int y_grid,
                                               int w_grid, int h_grid,
                                               Entity::EntityType entity_type);

void World::addHappyUnemployedHumans(Human* new_human) {
  happy_unemployed_humans_.push_back(new_human);
}
std::list<Human*>& World::returnHappyUnemployedHumans() {
  return happy_unemployed_humans_;
}

void World::addUnhappyUnemployedHumans(Human* new_human) {
  unhappy_unemployed_humans_.push_back(new_human);
}
std::list<Human*>& World::returnUnhappyUnemployedHumans() {
  return unhappy_unemployed_humans_;
}

void World::addHappyEmployedHumans(Human* new_human) {
  happy_employed_humans_.push_back(new_human);
}
std::list<Human*>& World::returnHappyEmployedHumans() {
  return happy_employed_humans_;
}

void World::addUnhappyEmployedHumans(Human* new_human) {
  unhappy_employed_humans_.push_back(new_human);
}
std::list<Human*>& World::returnUnhappyEmployedHumans() {
  return unhappy_employed_humans_;
}
