#ifndef ECOSIM_BUILDING_H
#define ECOSIM_BUILDING_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "constructible.h"

class Building : public Constructible {
 public:
  enum Resources {
    FOOD,
    GOLD,
  };

  Building() = delete;
  Building(int x_grid, int y_grid, int w_grid, int h_grid,
           Entity::EntityType entity_type)
      : Constructible(x_grid, y_grid, w_grid, h_grid, entity_type) {}

  int returnResourceAmount(Building::Resources res);

 private:
  std::map<Building::Resources, int> ResourceToAmount;
};

#endif  // define ECOSIM_BUILDING_H
