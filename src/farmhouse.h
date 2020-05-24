#ifndef ECOSIM_FARMHOUSE_H
#define ECOSIM_FARMHOUSE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "building.h"

class Farm;

class Farmhouse : public Building {
 public:
  Farmhouse() = delete;
  Farmhouse(int grid_x, int grid_y, int grid_w, int grid_h,
            EntityType entity_type)
      : Building(grid_x, grid_y, grid_w, grid_h, entity_type) {}

  // virtual void render(sf::RenderWindow& window);

  // virtual void update(float time_s);
  Farm* closestFullFarm();

 private:
};

class Farm : public Building {
 public:
  Farm() = delete;
  Farm(int grid_x, int grid_y, int grid_w, int grid_h, EntityType entity_type)
      : Building(grid_x, grid_y, grid_w, grid_h, entity_type){};

  virtual void update(float time_s);
  virtual void render(sf::RenderWindow& window);

 private:
};

#endif  // define ECOSIM_FARMHOUSE_H
