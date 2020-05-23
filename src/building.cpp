#include "building.h"

#include <iostream>

const float kFoodDecay = 10. / 60.;

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

void Building::update(float time_s) {
  if (typeOfEntity() == HOUSE) {
    float delta_amount = kFoodDecay * time_s;
    Building::adaptResource(Building::FOOD, -delta_amount);
    if (Building::returnResourceAmount(Building::FOOD) < 0) {
      ResourceToAmount[Building::FOOD] = 0;
      std::cout << "no more food :(" << std::endl;
      // ToDO update unhappy and happy houses number
    }
  }
}
