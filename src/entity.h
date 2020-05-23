#ifndef ECOSIM_ENTITY_H
#define ECOSIM_ENTITY_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

class Entity {
 public:
  enum EntityType {
    LIGHTHOUSE,
    HOUSE,
    ROAD,
    HUMAN,
  };

  Entity() = delete;
  Entity(EntityType entity_type) : entity_type_(entity_type) {}
  Entity(Entity&& enti) : entity_type_(enti.entity_type_) {}
  Entity(const Entity& enti) = default;

  void render(sf::RenderWindow& window);

  EntityType typeOfEntity();
  virtual float worldX() = 0;
  virtual float worldY() = 0;
  virtual float worldW() = 0;
  virtual float worldH() = 0;
  virtual void update(float time_s) = 0;

 private:
  EntityType entity_type_;
};

#endif  // define ECOSIM_ENTITY_H
