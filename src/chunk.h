#ifndef ECOSIM_CHUNK_H
#define ECOSIM_CHUNK_H

#include <SFML/Graphics.hpp>

#include "tile.h"

const int kChunkSize = 64;

class Chunk {
 public:
  Chunk() = delete;
  Chunk(const Chunk&) = delete;
  Chunk(int chunk_x, int chunk_y)
      : chunk_x_(chunk_x), chunk_y_(chunk_y), tiles_() {}
  Chunk(Chunk&& chunk) = default;

  // Returns the tile at (offset_x, offset_y) relative to the chunk location.
  const Tile& getRelativeTile(int offset_x, int offset_y) const;

  void render(sf::RenderWindow& window) const;

 private:
  int chunk_x_;
  int chunk_y_;
  Tile tiles_[kChunkSize * kChunkSize];
};

#endif  // define ECOSIM_CHUNK_H
