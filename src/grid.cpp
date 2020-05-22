#include "grid.h"

sf::Vector2i worldCoordinateToGrid(sf::Vector2f worldCoordinate) {
  int x = worldCoordinat.x / 10;
  int y = worldCoordinat.y / 10;
  return sf::Vector2i(x, y);
}

sf::Vector2i worldCoordinateToGrid(sf::Vector2i worldCoordinate) {
  int x = worldCoordinat.x / 10;
  int y = worldCoordinat.y / 10;
  return sf::Vector2i(x, y);
}