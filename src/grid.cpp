#include "grid.h"

sf::Vector2i worldCoordinateToGrid(sf::Vector2f worldCoordinate) {
  int x = worldCoordinate.x / kPixelsPerTile;
  int y = worldCoordinate.y / kPixelsPerTile;
  return sf::Vector2i(x, y);
}

sf::Vector2i worldCoordinateToGrid(sf::Vector2i worldCoordinate) {
  int x = worldCoordinate.x / kPixelsPerTile;
  int y = worldCoordinate.y / kPixelsPerTile;
  return sf::Vector2i(x, y);
}
