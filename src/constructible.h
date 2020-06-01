#ifndef ECOSIM_CONSTRUCTIBLE_H
#define ECOSIM_CONSTRUCTIBLE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "entity.h"

class Constructible : public Entity {
 public:
  Constructible() = delete;
  Constructible(int x_grid, int y_grid, int w_grid, int h_grid,
                EntityType entity_type)
      : Entity(entity_type),
        x_grid_(x_grid),
        y_grid_(y_grid),
        w_grid_(w_grid),
        h_grid_(h_grid) {}

  // convert grid to world position/size
  virtual float worldX();
  virtual float worldY();
  virtual float worldW();
  virtual float worldH();
  virtual sf::Rect<float> worldRect();

  int gridX() { return x_grid_; }
  int gridY() { return y_grid_; }
  int gridW() { return w_grid_; }
  int gridH() { return h_grid_; }

 private:
  int x_grid_;
  int y_grid_;
  int w_grid_;
  int h_grid_;
};

#endif  // define ECOSIM_CONSTRUCTIBLE_H
