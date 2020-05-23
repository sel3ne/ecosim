#ifndef ECOSIM_GRID_H
#define ECOSIM_GRID_H

#include <SFML/Graphics.hpp>
#include <memory>

sf::Vector2i worldCoordinateToGrid(sf::Vector2f worldCoordinate);
sf::Vector2i worldCoordinateToGrid(sf::Vector2i worldCoordinate);

const int kPixelsPerTile = 10;

#endif  // define ECOSIM_GRID_H
