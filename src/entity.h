#ifndef ECOSIM_ENTITY_H
#define ECOSIM_ENTITY_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

class Entity {
 public:
  Entity() = delete;
  Entity(sf::Texture* texture) : texture_(texture) {}
  Entity(Entity&& enti) : texture_(enti.texture_) {}
  Entity(const Entity& enti) = default;

  void render(sf::RenderWindow& window);

  virtual float worldX() = 0;
  virtual float worldY() = 0;
  virtual float worldW() = 0;
  virtual float worldH() = 0;

 private:
  // float x_;
  // float y_;
  sf::Texture* texture_;
};

#endif  // define ECOSIM_ENTITY_H