#ifndef ECOSIM_HUMAN_H
#define ECOSIM_HUMAN_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "constructible.h"

class Human : public Entity {
 public:
  // enum ResourcesCarrie {
  //   FOOD = 0,
  //   GOLD,

  //   _N_RESOURCES,
  // };

  Human() = delete;
  Human(int x_world, int y_world, int w_world, int h_world,
        Entity::EntityType entity_type)
      : Entity(entity_type),
        x_world_(x_world),
        y_world_(y_world),
        w_world_(w_world),
        h_world_(h_world) {
    happiness_ = 1;
  }

  virtual void render(sf::RenderWindow& window);

  virtual void update(float time_s);
  virtual float worldX();
  virtual float worldY();
  virtual float worldW();
  virtual float worldH();

 private:
  int x_world_;
  int y_world_;
  int w_world_;
  int h_world_;
  bool happiness_;
};

#endif  // define ECOSIM_HUMAN_H
