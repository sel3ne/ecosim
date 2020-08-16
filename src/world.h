#ifndef ECOSIM_WORLD_H
#define ECOSIM_WORLD_H

#include <list>

#include "chunk_manager.h"
#include "entity.h"

namespace sf {
class RenderWindow;
}

class Building;
class Constructible;
class Delivery;
class Human;

class World {
 public:
  World();
  void render(sf::RenderWindow& window);
  void addEntityToEntities(std::unique_ptr<Entity> entity);
  void scheduleDelivery(std::unique_ptr<Delivery> delivery);
  Delivery* getOldestUnassignedDelivery();

  void doForAllEntities(std::function<void(Entity&)> func);
  void doForAllHumans(std::function<void(Human&)> func);
  void doForAllConstructibles(std::function<void(Constructible&)> func);

  Human* closestHuman(sf::Vector2f world_pos,
                      std::function<bool(const Human&)> predicate);
  Building* closestBuilding(sf::Vector2f world_pos,
                            std::function<bool(const Building&)> predicate);

  void addNumberLighthouse(int i);
  int returnNumberLighthouse();

  void addNumberHappyHouse(int i);
  int returnNumberHappyHouse();

  void addNumberUnhappyHouse(int i);
  int returnNumberUnhappyHouse();

  int returnNumberHappyHuman();

  int returnNumberUnhappyHuman();

  void addNumberFarmhouse(int i);
  int returnNumberFarmhouse();

  void addNumberFarm(int i);
  int returnNumberFarm();

  void handleHouseBecomingUnhappy();
  void handleHouseBecomingHappy();

  void update(float time_s);

  ChunkManager& returnChunkManager() { return chunk_manager_; }

  template <typename EntityClass>
  EntityClass* buildConstructible(int x_grid, int y_grid, int w_grid,
                                  int h_grid, Entity::EntityType entity_type);

  float totalTimePlayed() { return total_time_played_; }

 private:
  ChunkManager chunk_manager_;
  std::vector<std::unique_ptr<Human>> humans_;
  std::vector<std::unique_ptr<Constructible>> constructibles_;
  std::list<std::unique_ptr<Delivery>> deliveries_;

  int number_happy_house_;
  int number_unhappy_house_;
  int number_happy_human_;
  int number_unhappy_human_;
  int number_lighthouse_;
  int number_farmhouse_;
  int number_farm_;
  float total_time_played_ = 0.f;
};

#endif  // define ECOSIM_WORLD_H
