#include "building.h"

#include <iostream>

#include "delivery.h"
#include "game.h"
#include "human.h"
#include "settings.h"
#include "vector_util.h"
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

int Building::returnReservedResourceAmount(ResourceId res) {
  return resources_reserved_[res];
}

int Building::returnIncomingResourceAmount(ResourceId res) {
  return resources_incoming_[res];
}

int Building::returnRequiredResourceAmount(ResourceId res) {
  return resources_required_[res];
}

float Building::returnAvailableAndReservedResourceAmount(ResourceId res) {
  return resources_available_[res] + resources_reserved_[res];
}

void Building::addToAvailableResourceAmount(ResourceId res,
                                            float delta_amount) {
  if (typeOfEntity() == Entity::HOUSE && res == RESOURCE_FOOD &&
      resources_available_[RESOURCE_FOOD] == 0. && delta_amount > 0) {
    gGame->returnWorld().handleHouseBecomingHappy();
  }

  resources_available_[res] += delta_amount;
  tryToDeliverAvailableResources(res);
}

void Building::addToReservedResourceAmount(ResourceId res, int delta_amount) {
  resources_reserved_[res] += delta_amount;
}

void Building::addToIncomingResourceAmount(ResourceId res, int delta_amount) {
  resources_incoming_[res] += delta_amount;
}

void Building::addToRequiredResourceAmount(ResourceId res, int delta_amount) {
  resources_required_[res] += delta_amount;
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
  while (resources_available_[res] >= kCarrierCapacity) {
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

    makeDeliveryTo(target, res);
  }
}

void Building::tryToRequestMissingResources(ResourceId res) {
  while (resources_required_[res] - resources_available_[res] -
             resources_incoming_[res] >=
         kCarrierCapacity) {
    float min_squared_dist = std::numeric_limits<float>::max();
    Building* nearest_source = nullptr;
    for (Building* candidate_source : receiving_from_[res]) {
      if (candidate_source->returnAvailableResourceAmount(res) >=
          kCarrierCapacity) {
        float candidate_squared_distance =
            squaredDistance(candidate_source->worldPos(), worldPos());
        if (candidate_squared_distance < min_squared_dist) {
          min_squared_dist = candidate_squared_distance;
          nearest_source = candidate_source;
        }
      }
    }

    if (!nearest_source) {
      // No one can send us this resource anymore. Give up.
      return;
    }

    nearest_source->makeDeliveryTo(this, res);
  }
}

void Building::makeDeliveryTo(Building* target, ResourceId res) {
  resources_available_[res] -= kCarrierCapacity;
  resources_reserved_[res] += kCarrierCapacity;

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
      setAvailableResourceAmount(RESOURCE_FOOD, 0.);
      gGame->returnWorld().handleHouseBecomingUnhappy();
    }
  }
}

void Building::renderArrows(sf::RenderWindow& window) {
  drawLine(sf::Vector2f(0, -100), sf::Vector2f(10, 200), window);
}

void Building::drawLine(sf::Vector2f start, sf::Vector2f end,
                        sf::RenderWindow& window) {
  // float triangle_size = 5.f;

  // end = end - sf::Vector2f(1.732*);
  float lenght = norm(end - start);
  sf::RectangleShape line(sf::Vector2f(lenght, 1));
  float angle = atan2((end - start).y, (end - start).x);
  line.rotate(angle * 180 / 3.141);
  line.setPosition(start);
  window.draw(line);

  float triangle_size = 5.f;

  // fucking pfeil mit 3 linien -> geht nicht
  // sf::RectangleShape line2(sf::Vector2f(triangle_size, 1));
  // line2.setPosition(end);
  // line2.rotate(angle * 180 / 3.141 - 45 + 180);
  // window.draw(line2);

  // sf::RectangleShape line3(sf::Vector2f(triangle_size, 1));
  // line3.setPosition(end);
  // line3.rotate(angle * 180 / 3.141 + 45 + 180);
  // window.draw(line3);

  // fucking pfeil mit dreieck -> geht nicht
  sf::CircleShape triang(triangle_size, 3);

  triang.rotate(-30 + angle * 180 / 3.141);
  sf::Vector2f triang_summit(triangle_size, 0);
  sf::Vector2f triang_summit2(0, triangle_size * 2);
  sf::Vector2f triang_offset = (triang_summit + triang_summit2) / 2.f;
  triang.setPosition(end - triang_offset);
  window.draw(triang);
}
