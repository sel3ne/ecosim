#include "building.h"

#include <iostream>

#include "game.h"
#include "settings.h"

Building::Building(int x_grid, int y_grid, int w_grid, int h_grid,
                   Entity::EntityType entity_type)
    : Constructible(x_grid, y_grid, w_grid, h_grid, entity_type) {
  for (int i = 0; i < _N_RESOURCES; i++) {
    Building::ResourceToAmount[static_cast<Resources>(i)] = 0.0f;
  }
  if (entity_type == HOUSE) {
    Building::adaptResource(Resources::FOOD, 100);
  }
}

float Building::returnResourceAmount(Building::Resources res) {
  return ResourceToAmount.at(res);
}

void Building::adaptResource(Building::Resources res, float delta_amount) {
  float old_amount = Building::returnResourceAmount(res);
  float new_amount = old_amount + delta_amount;
  ResourceToAmount[res] = new_amount;
}

void Building::setResourceToAmount(Building::Resources res, float set_amount) {
  ResourceToAmount[res] = set_amount;
}

void Building::update(float time_s) {
  if (typeOfEntity() == HOUSE && ResourceToAmount[Building::FOOD] != 0) {
    float delta_amount = kFoodDecay * time_s;
    Building::adaptResource(Building::FOOD, -delta_amount);

    if (Building::returnResourceAmount(Building::FOOD) <= 0) {
      // Updates number of happy and unhappy houses depeding on the food inside
      // of them
      ResourceToAmount[Building::FOOD] = 0;
      std::cout << "no more food :(" << std::endl;
      World& world = gGame->returnWorld();
      world.addNumberHappyHouse(-1);
      world.addNumberUnhappyHouse(1);
      // find kHumansPerHouse happy people to make unhappy
      for (int i = 0; i < kHumansPerHouse; i++) {
        std::cout << i << std::endl;
        World& world = gGame->returnWorld();
        std::list<Human*>& happy_unemployed_humans =
            world.returnHappyUnemployedHumans();
        std::list<Human*>& unhappy_unemployed_humans =
            world.returnUnhappyUnemployedHumans();

        if (!happy_unemployed_humans.empty()) {
          Human* happy_human = happy_unemployed_humans.front();
          happy_unemployed_humans.pop_front();
          unhappy_unemployed_humans.push_back(happy_human);
          happy_human->set_happiness(false);
        } else {
          std::list<Human*>& happy_employed_humans =
              world.returnHappyEmployedHumans();
          std::list<Human*>& unhappy_employed_humans =
              world.returnUnhappyEmployedHumans();
          Human* happy_human = happy_employed_humans.front();
          happy_employed_humans.pop_front();
          unhappy_employed_humans.push_back(happy_human);
          happy_human->set_happiness(false);
        }
      }
    }
  }
}
