#include "chunk.h"

#include <iostream>

#include "grid.h"

Chunk::Chunk(int chunk_x, int chunk_y, Tile::TileId tile_ids[])
    : chunk_x_(chunk_x), chunk_y_(chunk_y), tiles_() {
  for (int i = 0; i < kChunkSize * kChunkSize; ++i) {
    tiles_[i] = Tile(tile_ids[i]);
  }
}

const Tile& Chunk::getRelativeTile(int offset_x, int offset_y) const {
  return tiles_[kChunkSize * offset_y + offset_x];
}

sf::IntRect calculateVisibleRect(sf::RenderWindow& window) {
  sf::Vector2f view_center = window.getView().getCenter();
  sf::Vector2f view_size = window.getView().getSize();
  sf::FloatRect visible_rect(
      /*position=*/view_center - view_size / 2.f,
      /*size=*/view_size);
  return sf::IntRect(visible_rect);
}

void Chunk::render(sf::RenderWindow& window) const {
  sf::IntRect chunk_rect(/*rectLeft=*/chunk_x_ * kChunkSize * kPixelsPerTile,
                         /*rectTop=*/chunk_y_ * kChunkSize * kPixelsPerTile,
                         /*rectWidth=*/kChunkSize * kPixelsPerTile,
                         /*rectHeight=*/kChunkSize * kPixelsPerTile);
  sf::IntRect visible_rect = calculateVisibleRect(window);
  if (!chunk_rect.intersects(visible_rect)) {
    return;
  }

  for (int offset_x = 0; offset_x < kChunkSize; ++offset_x) {
    for (int offset_y = 0; offset_y < kChunkSize; ++offset_y) {
      int tile_x = chunk_x_ * kChunkSize + offset_x;
      int tile_y = chunk_y_ * kChunkSize + offset_y;
      getRelativeTile(offset_x, offset_y).render(window, tile_x, tile_y);
    }
  }
}
