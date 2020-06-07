#include "building.h"

#include <iostream>

#include "delivery.h"
#include "game.h"
#include "human.h"
#include "settings.h"
#include "world.h"

Building::Building(int x_grid, int y_grid, int w_grid, int h_grid,
                   Entity::EntityType entity_type)
    : Constructible(x_grid, y_grid, w_grid, h_grid, entity_type) {
  if (entity_type == HOUSE) {
    setAvailableResourceAmount(RESOURCE_FOOD, 100);
    resources_required_[RESOURCE_FOOD] = 100;
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
  tryToDeliverAvailableResources(res);
}

void Building::setAvailableResourceAmount(ResourceId res, float set_amount) {
  resources_available_[res] = set_amount;
  tryToDeliverAvailableResources(res);
}

const std::vector<Building*>& Building::getDeliveryTargets(
    ResourceId resource) {
  return delivering_to_[resource];
}

void Building::addDeliveryTarget(ResourceId resource, Building* target) {
  delivering_to_[resource].push_back(target);
  target->receiving_from_[resource].push_back(this);
}

void Building::removeDeliveryTarget(ResourceId resource, Building* target) {
  std::erase(delivering_to_[resource], target);
}

bool Building::needsMoreResource(ResourceId res) {
  return resources_required_[res] >
         resources_available_[res] + resources_incoming_[res];
}

void Building::tryToDeliverAvailableResources(ResourceId res) {
  while (resources_available_[res] >= 1) {
    std::cout << "Trying to deliver 1 " << kResourceNames.at(res) << std::endl;
    int& next_delivery_target_index = next_delivery_target_indices[res];
    std::vector<Building*>& target_candidates = delivering_to_[res];

    Building* target = nullptr;
    for (uint i = 0; i < target_candidates.size(); ++i) {
      Building* target_candidate =
          target_candidates.at(next_delivery_target_index);
      next_delivery_target_index =
          next_delivery_target_index + 1 % target_candidates.size();
      if (target_candidate->needsMoreResource(res)) {
        target = target_candidate;
        break;
      }
    }
    if (!target) {
      // All receivers already have enough of this resource.
      return;
    }

    MakeDeliveryTo(target, res);
  }
}

void Building::MakeDeliveryTo(Building* target, ResourceId res) {
  resources_available_[res] -= 1.;
  resources_reserved_[res] += 1;

  std::unique_ptr<Delivery> delivery =
      std::make_unique<Delivery>(this, target, nullptr, res);

  outgoing_deliveries_.push_back(delivery.get());
  target->incoming_deliveries_.push_back(delivery.get());

  gGame->returnWorld().scheduleDelivery(std::move(delivery));
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
