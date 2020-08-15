#ifndef ECOSIM_MAP_GENERATOR_H
#define ECOSIM_MAP_GENERATOR_H

#include <cstdint>

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

class PerlinNoiseMapGenerator : public MapGenerator {
 public:
  PerlinNoiseMapGenerator(uint64_t seed, float height_period)
      : seed_(seed), height_period_(height_period) {}

  virtual void generateTileIds(int chunk_x, int chunk_y,
                               Tile::TileId tile_ids[kChunkSize * kChunkSize]);

 private:
  uint64_t seed_;
  float height_period_;
};

/* IMPLEMENTATION */
/* Exposed in header for tests. */

float perlinNoise(uint64_t seed, float x, float y);

#endif  // define ECOSIM_MAP_GENERATOR_H
