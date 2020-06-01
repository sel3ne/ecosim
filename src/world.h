#ifndef ECOSIM_WORLD_H
#define ECOSIM_WORLD_H

#include <SFML/Graphics.hpp>
#include <list>

#include "chunk_manager.h"
#include "entity.h"
#include "human.h"

class World {
 public:
  World();
  void render(sf::RenderWindow& window);
  void addEntityToEntities(std::unique_ptr<Entity> entity);

  void doForAllEntities(std::function<void(Entity&)> func);

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

  void addHappyUnemployedHumans(Human* new_human);
  std::list<Human*>& returnHappyUnemployedHumans();

  void addUnhappyUnemployedHumans(Human* new_human);
  std::list<Human*>& returnUnhappyUnemployedHumans();

  void addHappyEmployedHumans(Human* new_human);
  std::list<Human*>& returnHappyEmployedHumans();

  void addUnhappyEmployedHumans(Human* new_human);
  std::list<Human*>& returnUnhappyEmployedHumans();

  void update(float time_s);

  ChunkManager& returnChunkManager() { return chunk_manager_; }

  template <typename EntityClass>
  EntityClass* buildConstructible(int x_grid, int y_grid, int w_grid,
                                  int h_grid, Entity::EntityType entity_type);

 private:
  ChunkManager chunk_manager_;
  std::vector<std::unique_ptr<Human>> humans_;
  std::vector<std::unique_ptr<Constructible>> constructibles_;

  int number_happy_house_;
  int number_unhappy_house_;
  int number_happy_human_;
  int number_unhappy_human_;
  int number_lighthouse_;
  int number_farmhouse_;
  int number_farm_;
  std::list<Human*> happy_unemployed_humans_;
  std::list<Human*> unhappy_unemployed_humans_;
  std::list<Human*> happy_employed_humans_;
  std::list<Human*> unhappy_employed_humans_;
};

#endif  // define ECOSIM_WORLD_H
