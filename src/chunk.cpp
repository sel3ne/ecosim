#include "chunk.h"

const Tile& Chunk::getRelativeTile(int offset_x, int offset_y) const {
  return tiles_[kChunkSize * offset_y + offset_x];
}

void Chunk::render(sf::RenderWindow& window) const {
  for (int offset_x = 0; offset_x < kChunkSize; ++offset_x) {
    for (int offset_y = 0; offset_y < kChunkSize; ++offset_y) {
      int tile_x = chunk_x_ * kChunkSize + offset_x;
      int tile_y = chunk_y_ * kChunkSize + offset_y;
      getRelativeTile(offset_x, offset_y).render(window, tile_x, tile_y);
    }
  }
}
