#include "map_generator.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "random.h"
#include "vector_util.h"

constexpr float kPi = 3.1415926;
constexpr float kSqrt2 = 1.41421356237;
constexpr int kNumOctaves = 20;

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

float gridPointPlaneValue(uint64_t seed, int grid_x, int grid_y, float x,
                          float y) {
  DeterministicRandomGenerator rand_gen(seed);
  rand_gen.mixIntoState(grid_x);
  rand_gen.mixIntoState(grid_y);
  float gradient_angle = rand_gen.getFloat(/*min=*/0, /*max=*/2 * kPi);
  sf::Vector2f gradient(std::cos(gradient_angle), std::sin(gradient_angle));

  return dotProduct(gradient, sf::Vector2f(x - grid_x, y - grid_y));
}

float linear_interpolate(float a, float b, float w) {
  return a * w + b * (1. - w);
}

// Algorithm to create some noise like pattern on a grid. See:
// https://en.wikipedia.org/wiki/Perlin_noise#Implementation
// The noise created by this is around a grid of 0 values, so we need to combine
// multiple of these with various frequencies to make the grid disappear.
float perlinNoise(uint64_t seed, float x, float y) {
  // Find noise grid coordinates around the point.
  int left_x = std::floor(x);
  int right_x = left_x + 1;
  int top_y = std::floor(y);
  int bottom_y = top_y + 1;

  // Value on the planes of the four nearest grid points.
  float left_top_value = gridPointPlaneValue(seed, left_x, top_y, x, y);
  float left_bottom_value = gridPointPlaneValue(seed, left_x, bottom_y, x, y);
  float right_top_value = gridPointPlaneValue(seed, right_x, top_y, x, y);
  float right_bottom_value = gridPointPlaneValue(seed, right_x, bottom_y, x, y);

  // Interpolate those values based on distances.
  float left_weight = 1 - (x - left_x);
  float top_weight = 1 - (y - top_y);
  float top_value =
      linear_interpolate(left_top_value, right_top_value, left_weight);
  float bottom_value =
      linear_interpolate(left_bottom_value, right_bottom_value, left_weight);
  float value = linear_interpolate(top_value, bottom_value, top_weight);

  // Scale by a factor of sqrt(2) to get to range [-1, 1].
  return kSqrt2 * value;
}

// Make pink noise in range [-1, 1] with a base period of `period`
float pinkNoise(uint64_t seed, float period, float x, float y) {
  float kPersistence = 0.5;
  float kInvLacunarity = 0.5;

  float value = 0.;
  float amplitude = .5;
  for (int octave_idx = 0; octave_idx < kNumOctaves; ++octave_idx) {
    value += amplitude * perlinNoise(seed + octave_idx, x / period, y / period);
    period *= kInvLacunarity;
    amplitude *= kPersistence;
  }
  return value;
}

void PerlinNoiseMapGenerator::generateTileIds(
    int chunk_x, int chunk_y, Tile::TileId tile_ids[kChunkSize * kChunkSize]) {
  for (int relative_y = 0; relative_y < kChunkSize; relative_y++) {
    for (int relative_x = 0; relative_x < kChunkSize; relative_x++) {
      int x = kChunkSize * chunk_x + relative_x;
      int y = kChunkSize * chunk_y + relative_y;

      float height = pinkNoise(seed_, height_period_, x, y);

      Tile::TileId tile_id;
      if (height < -0.05) {
        tile_id = Tile::WATER;
      } else if (height < 0.) {
        tile_id = Tile::SAND;
      } else if (height < 0.5) {
        tile_id = Tile::GRASS;
      } else {
        tile_id = Tile::STONE;
      }
      tile_ids[relative_y * kChunkSize + relative_x] = tile_id;
    }
  }
}
