#include "world.h"

#include <cassert>
#include <iostream>

#include "building.h"
#include "delivery.h"
#include "farmhouse.h"
#include "grid.h"
#include "human.h"
#include "settings.h"
#include "vector_util.h"

World* gWorld = nullptr;

World::World()
    : number_happy_house_(0),
      number_unhappy_house_(0),
      number_happy_human_(0),
      number_unhappy_human_(0),
      number_lighthouse_(0) {
  if (gWorld) {
    std::cerr << "Error creating world: There is already a World!" << std::endl;
    exit(1);
  }
  gWorld = this;
}

World::~World() { gWorld = nullptr; }

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

Human* World::closestHuman(sf::Vector2f world_pos,
                           std::function<bool(const Human&)> predicate) {
  float min_dist_sq = std::numeric_limits<float>::max();
  Human* closest = nullptr;
  for (const std::unique_ptr<Human>& human : humans_) {
    if (predicate(*human)) {
      float dist_sq = squaredDistance(
          sf::Vector2f(human->worldX(), human->worldY()), world_pos);
      if (dist_sq < min_dist_sq) {
        closest = human.get();
      }
    }
  }
  return closest;
}

Building* World::closestBuilding(
    sf::Vector2f world_pos, std::function<bool(const Building&)> predicate) {
  float min_dist_sq = std::numeric_limits<float>::max();
  Building* closest = nullptr;
  for (const std::unique_ptr<Constructible>& constructible : constructibles_) {
    if (!constructible->isBuilding()) {
      continue;
    }
    Building* building = dynamic_cast<Building*>(constructible.get());
    if (predicate(*building)) {
      float dist_sq = squaredDistance(
          sf::Vector2f(building->worldX(), building->worldY()), world_pos);
      if (dist_sq < min_dist_sq) {
        closest = building;
      }
    }
  }
  return closest;
}

void World::update(float time_s) {
  for (std::unique_ptr<Human>& n : humans_) {
    n->update(time_s);
  }
  for (std::unique_ptr<Constructible>& n : constructibles_) {
    n->update(time_s);
  }
  total_time_played_ += time_s;
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

void World::scheduleDelivery(std::unique_ptr<Delivery> delivery) {
  sf::Vector2f source_pos(delivery->getSource()->worldX(),
                          delivery->getSource()->worldY());
  Human* carrier = closestHuman(source_pos, [](const Human& human) -> bool {
    return human.returnJob() == Human::UNEMPLOYED;
  });
  if (carrier) {
    carrier->assignDelivery(delivery.get());
  }
  deliveries_.push_back(std::move(delivery));
}

Delivery* World::getOldestUnassignedDelivery() {
  std::list<std::unique_ptr<Delivery>>::iterator first_unassigned_iter =
      std::find_if(deliveries_.begin(), deliveries_.end(),
                   [](const std::unique_ptr<Delivery>& delivery) {
                     return delivery->getCarrier() == nullptr;
                   });

  if (first_unassigned_iter == deliveries_.end()) {
    return nullptr;
  }
  return first_unassigned_iter->get();
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
  return number_happy_house_ * kHumansPerHouse;
}

int World::returnNumberUnhappyHuman() {
  return number_unhappy_house_ * kHumansPerHouse;
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

void World::handleHouseBecomingUnhappy() {
  addNumberHappyHouse(-1);
  addNumberUnhappyHouse(1);

  std::vector<Human*> happy_unemployed_humans;
  std::vector<Human*> happy_employed_humans;

  doForAllHumans([&](Human& human) {
    if (human.isHappy()) {
      if (human.returnJob() == Human::UNEMPLOYED) {
        happy_unemployed_humans.push_back(&human);
      } else {
        happy_employed_humans.push_back(&human);
      }
    }
  });

  if (happy_unemployed_humans.size() >= kHumansPerHouse) {
    for (uint i = 0; i < kHumansPerHouse; i++) {
      happy_unemployed_humans[i]->setHappiness(false);
    }
  } else {
    for (Human* human : happy_unemployed_humans) {
      human->setHappiness(false);
    }
    uint num_to_make_unhappy_left =
        kHumansPerHouse - happy_unemployed_humans.size();
    assert(num_to_make_unhappy_left <= happy_employed_humans.size());
    for (uint i = 0; i < num_to_make_unhappy_left; ++i) {
      happy_employed_humans[i]->setHappiness(false);
    }
  }
}

void World::handleHouseBecomingHappy() {
  addNumberHappyHouse(1);
  addNumberUnhappyHouse(-1);

  std::vector<Human*> unhappy_unemployed_humans;
  std::vector<Human*> unhappy_employed_humans;

  doForAllHumans([&](Human& human) {
    if (!human.isHappy()) {
      if (human.returnJob() == Human::UNEMPLOYED) {
        unhappy_unemployed_humans.push_back(&human);
      } else {
        unhappy_employed_humans.push_back(&human);
      }
    }
  });

  if (unhappy_employed_humans.size() >= kHumansPerHouse) {
    for (uint i = 0; i < kHumansPerHouse; i++) {
      unhappy_employed_humans[i]->setHappiness(true);
    }
  } else {
    for (Human* human : unhappy_employed_humans) {
      human->setHappiness(true);
    }
    uint num_to_make_happy_left =
        kHumansPerHouse - unhappy_employed_humans.size();
    assert(num_to_make_happy_left <= unhappy_unemployed_humans.size());
    for (uint i = 0; i < num_to_make_happy_left; ++i) {
      unhappy_unemployed_humans[i]->setHappiness(true);
    }
  }
}
