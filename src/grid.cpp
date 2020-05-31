#include "grid.h"

#include <cmath>

sf::Vector2i worldCoordinateToGrid(sf::Vector2f worldCoordinate) {
  int x = std::floor(worldCoordinate.x / kPixelsPerTile);
  int y = std::floor(worldCoordinate.y / kPixelsPerTile);
  return sf::Vector2i(x, y);
}

sf::Vector2i worldCoordinateToGrid(sf::Vector2i worldCoordinate) {
  int x = std::floor(static_cast<float>(worldCoordinate.x) / kPixelsPerTile);
  int y = std::floor(static_cast<float>(worldCoordinate.y) / kPixelsPerTile);
  return sf::Vector2i(x, y);
}
