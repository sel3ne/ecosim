#include "map_generator.h"

#include <cstdlib>

void SimpleIslandGenerator::generateTileIds(
    int chunk_x, int chunk_y, Tile::TileId tile_ids[kChunkSize * kChunkSize]) {
  Tile::TileId tile;
  if (chunk_x == 0 && chunk_y == 0) {
    tile = Tile::STONE;
  } else if (abs(chunk_x) < 5 && abs(chunk_y) < 5) {
    tile = Tile::GRASS;
  } else {
    tile = Tile::WATER;
  }
  for (int i = 0; i < kChunkSize * kChunkSize; ++i) {
    tile_ids[i] = tile;
  }
}
