#include "entity.h"

#include <iostream>

#include "grid.h"

void Entity::render(sf::RenderWindow& window) {
  sf::Vector2u texSize = texture_->getSize();
  sf::Sprite sprite;
  sprite.setTexture(*texture_);
  sprite.setPosition(worldX(), worldY());
  sprite.setScale(worldW() / kPixelsPerTile / texSize.x,
                  worldH() / kPixelsPerTile / texSize.y);
  window.draw(sprite);
}