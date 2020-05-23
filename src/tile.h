#ifndef ECOSIM_TILE_H
#define ECOSIM_TILE_H

#include <SFML/Graphics.hpp>

class Tile {
 public:
  enum TileId { GRASS, WATER, STONE };

  Tile() : id_(GRASS) {}
  Tile(TileId id) : id_(id) {}
  TileId id() { return id_; }

  void render(sf::RenderWindow& window, int grid_x, int grid_y) const;

 private:
  TileId id_;
};

#endif  // define ECOSIM_TILE_H