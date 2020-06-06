#ifndef ECOSIM_FARMHOUSE_H
#define ECOSIM_FARMHOUSE_H

#include "building.h"

class Farm;

class Farmhouse : public Building {
 public:
  Farmhouse() = delete;
  Farmhouse(int grid_x, int grid_y, int grid_w, int grid_h,
            EntityType entity_type)
      : Building(grid_x, grid_y, grid_w, grid_h, entity_type) {}

  Farm* closestFullFarm();

 private:
};

class Farm : public Building {
 public:
  Farm() = delete;
  Farm(int grid_x, int grid_y, int grid_w, int grid_h, EntityType entity_type)
      : Building(grid_x, grid_y, grid_w, grid_h, entity_type) {
    is_assigned_to_farmer_ = false;
  };

  virtual void update(float time_s);
  virtual void render(sf::RenderWindow& window);
  bool is_assigned() { return is_assigned_to_farmer_; }
  void set_assigned(bool true_or_false) {
    is_assigned_to_farmer_ = true_or_false;
  }

 private:
  bool is_assigned_to_farmer_;
};

#endif  // define ECOSIM_FARMHOUSE_H
