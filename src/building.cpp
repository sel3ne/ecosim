#include "building.h"

#include <iostream>

#include "game.h"
#include "human.h"
#include "settings.h"
#include "world.h"

Building::Building(int x_grid, int y_grid, int w_grid, int h_grid,
                   Entity::EntityType entity_type)
    : Constructible(x_grid, y_grid, w_grid, h_grid, entity_type) {
  for (int i = 0; i < _N_RESOURCES; i++) {
    // resources_available_[static_cast<ResourceId>(i)] = 0.0f;
  }
  if (entity_type == HOUSE) {
    setAvailableResourceAmount(RESOURCE_FOOD, 100);
  }
}

std::string Building::printNameAndResource() {
  std::string result = "";
  result.append(kEntityNames.at(this->typeOfEntity()));
  result.append("\n");
  for (int resource_index = 0; resource_index < _N_RESOURCES;
       resource_index++) {
    ResourceId resource = static_cast<ResourceId>(resource_index);
    std::string resource_name = kResourceNames.at(resource);
    result.append(resource_name);
    result.append(": ");
    result.append(std::to_string(returnAvailableResourceAmount(resource)));
    result.append("/");
    result.append(
        std::to_string(returnAvailableAndReservedResourceAmount(resource)));
    result.append("\n");
  }
  return result;
}

float Building::returnAvailableResourceAmount(ResourceId res) {
  return resources_available_[res];
}

float Building::returnAvailableAndReservedResourceAmount(ResourceId res) {
  return resources_available_[res] + resources_reserved_[res];
}

void Building::addToAvailableResourceAmount(ResourceId res,
                                            float delta_amount) {
  resources_available_[res] += delta_amount;
}

void Building::setAvailableResourceAmount(ResourceId res, float set_amount) {
  resources_available_[res] = set_amount;
}

const std::set<Building*>& Building::getDeliveryTargets(ResourceId resource) {
  return delivering_to_[resource];
}

void Building::addDeliveryTarget(ResourceId resource, Building* target) {
  delivering_to_[resource].insert(target);
}

void Building::removeDeliveryTarget(ResourceId resource, Building* target) {
  delivering_to_[resource].erase(target);
}

void Building::update(float time_s) {
  if (typeOfEntity() == HOUSE && resources_available_[RESOURCE_FOOD] != 0) {
    float delta_amount = kFoodDecay * time_s;
    addToAvailableResourceAmount(RESOURCE_FOOD, -delta_amount);

    if (returnAvailableResourceAmount(RESOURCE_FOOD) <= 0) {
      // Updates number of happy and unhappy houses depeding on the food inside
      // of them
      setAvailableResourceAmount(RESOURCE_FOOD, 0.);
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
          happy_human->setHappiness(false);
        } else {
          std::list<Human*>& happy_employed_humans =
              world.returnHappyEmployedHumans();
          std::list<Human*>& unhappy_employed_humans =
              world.returnUnhappyEmployedHumans();
          Human* happy_human = happy_employed_humans.front();
          happy_employed_humans.pop_front();
          unhappy_employed_humans.push_back(happy_human);
          happy_human->setHappiness(false);
        }
      }
    }
  }
}
