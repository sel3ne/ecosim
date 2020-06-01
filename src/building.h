#ifndef ECOSIM_BUILDING_H
#define ECOSIM_BUILDING_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "constructible.h"
#include "resources.h"

class Building : public Constructible {
 public:
  Building() = delete;
  Building(int x_grid, int y_grid, int w_grid, int h_grid,
           Entity::EntityType entity_type);

  float returnResourceAmount(ResourceId res);

  void adaptResource(ResourceId res, float delta_amount);

  void setResourceToAmount(ResourceId res, float set_amount);

  virtual void update(float time_s);

 private:
  std::map<ResourceId, float> ResourceToAmount;
};

#endif  // define ECOSIM_BUILDING_H
