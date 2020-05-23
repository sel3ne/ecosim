#ifndef ECOSIM_MAP_GENERATOR_H
#define ECOSIM_MAP_GENERATOR_H

#include "chunk.h"
#include "tile.h"

class MapGenerator {
 public:
  virtual void generateTileIds(
      int chunk_x, int chunk_y,
      Tile::TileId tile_ids[kChunkSize * kChunkSize]) = 0;
};

/*
 * Simple map generator that makes chunk (0, 0) only STONE, surrounded by 80
 * chunks of GRASS (9x9 grid with the STONE chunk in the middle). Everything
 * else is WATER.
 */
class SimpleIslandGenerator : public MapGenerator {
 public:
  virtual void generateTileIds(int chunk_x, int chunk_y,
                               Tile::TileId tile_ids[kChunkSize * kChunkSize]);
};

#endif  // define ECOSIM_MAP_GENERATOR_H
