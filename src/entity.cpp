#include "entity.h"

#include <iostream>

void Entity::render(sf::RenderWindow& window) {
  sf::Sprite sprite;
  sprite.setTexture(*texture_);
  sprite.setPosition(x_, y_);
  window.draw(sprite);
}