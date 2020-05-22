#ifndef ECOSIM_ENTITY_H
#define ECOSIM_ENTITY_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

class Entity {
 public:
  Entity() = delete;
  Entity(float x, float y, sf::Texture* texture)
      : x_(x), y_(y), texture_(texture) {}
  Entity(Entity&& enti) : x_(enti.x_), y_(enti.y_), texture_(enti.texture_) {}
  Entity(const Entity& enti) = default;

  void render(sf::RenderWindow& window);

 private:
  float x_;
  float y_;
  sf::Texture* texture_;
};

#endif  // define ECOSIM_ENTITY_H