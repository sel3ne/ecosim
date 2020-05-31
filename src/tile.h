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

  bool returnOccupied() { return is_occupied_; }
  void setOccupied(bool occupied) { is_occupied_ = occupied; }

 private:
  TileId id_;
  bool is_occupied_ = 0;
};

#endif  // define ECOSIM_TILE_H
