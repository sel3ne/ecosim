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

  // Depending on whether resources were increased or decreased, we might be
  // able to deliver resources somewhere now or we might need to request new
  // ones.
  tryToDeliverAvailableResources(res);
  tryToRequestMissingResources(res);
}

void Building::addToReservedResourceAmount(ResourceId res, int delta_amount) {
  resources_reserved_[res] += delta_amount;
}

void Building::addToIncomingResourceAmount(ResourceId res, int delta_amount) {
  resources_incoming_[res] += delta_amount;
}

void Building::addToRequiredResourceAmount(ResourceId res, int delta_amount) {
  resources_required_[res] += delta_amount;
  tryToRequestMissingResources(res);
}

void Building::setAvailableResourceAmount(ResourceId res, float set_amount) {
  resources_available_[res] = set_amount;
  tryToDeliverAvailableResources(res);
  tryToRequestMissingResources(res);
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
  std::erase(target->receiving_from_[resource], this);
}

void Building::toggleDeliveryTarget(ResourceId resource, Building* target) {
  const std::vector<Building*>& targets = getDeliveryTargets(resource);
  if (std::find(targets.begin(), targets.end(), target) != targets.end()) {
    removeDeliveryTarget(resource, target);
  } else {
    addDeliveryTarget(resource, target);
  }
}

float Building::getResourceDeficit(ResourceId res) {
  return resources_required_[res] - resources_available_[res] -
         resources_incoming_[res];
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
      if (target_candidate->getResourceDeficit(res) >= kCarrierCapacity) {
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

void drawArrow(sf::Vector2f start, sf::Vector2f end, float thickness,
               sf::Color color, sf::RenderWindow& window) {
  float triangle_radius = 2.f * thickness;

  float length = norm(end - start);
  float angle_rad = atan2((end - start).y, (end - start).x);
  float angle_deg = angle_rad * 180 / 3.14159;

  sf::RectangleShape line(
      sf::Vector2f(length - triangle_radius * 1.5, thickness));
  line.setOrigin(0, 0.5 * thickness);
  line.setRotation(angle_deg);
  line.setPosition(start);
  line.setFillColor(color);
  window.draw(line);

  sf::CircleShape triang(triangle_radius, 3);
  triang.setOrigin(triangle_radius, 0.);
  triang.setPosition(end);
  triang.setRotation(90 + angle_deg);
  triang.setFillColor(color);
  window.draw(triang);
}

void Building::renderArrows(sf::RenderWindow& window) {
  for (auto& [resource, sources] : receiving_from_) {
    sf::Vector2f end = this->worldPos();
    end = end + this->worldSize() / 2.f;
    for (auto& source : sources) {
      sf::Vector2f start = source->worldPos();
      start = start + source->worldSize() / 2.f;
      drawArrow(start, end,
                /*thickness=*/2, sf::Color(230, 20, 20), window);
    }
  }
  for (auto& [resource, targets] : delivering_to_) {
    sf::Vector2f start = this->worldPos();
    start = start + this->worldSize() / 2.f;
    for (auto& target : targets) {
      sf::Vector2f end = target->worldPos();
      end = end + target->worldSize() / 2.f;
      drawArrow(start, end,
                /*thickness=*/2, sf::Color::Green, window);
    }
  }
}
