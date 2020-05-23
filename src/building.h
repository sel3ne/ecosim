#ifndef ECOSIM_BUILDING_H
#define ECOSIM_BUILDING_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "constructible.h"

class Building : public Constructible {
 public:
  enum Resources {
    FOOD = 0,
    GOLD,

    _N_RESOURCES,
  };

  Building() = delete;
  Building(int x_grid, int y_grid, int w_grid, int h_grid,
           Entity::EntityType entity_type);

  float returnResourceAmount(Building::Resources res);

  void adaptResource(Building::Resources res, float delta_amount);

  virtual void update(float time_s);

 private:
  std::map<Building::Resources, float> ResourceToAmount;
};

#endif  // define ECOSIM_BUILDING_H
