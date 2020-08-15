#include "chunk_manager.h"

#include <cmath>

#include "map_generator.h"

int gridToChunkCoordinate(int grid_coord) {
  return std::floor((float)grid_coord / (float)kChunkSize);
}

int gridToRelativeTile(int grid_coord) {
  int temp = grid_coord % kChunkSize;
  if (temp < 0) {
    return temp + kChunkSize;
  }
  return temp;
}

ChunkManager::ChunkManager() {
  map_gen_ = std::make_unique<PerlinNoiseMapGenerator>(/*seed=*/1,
                                                       /*height_period=*/64.);
  for (int x = -10; x <= 10; ++x) {
    for (int y = -10; y <= 10; ++y) {
      generateChunk(x, y);
    }
  }
}

ChunkManager::~ChunkManager() {}

void ChunkManager::generateChunk(int chunk_x, int chunk_y) {
  sf::Vector2i chunk_pos(chunk_x, chunk_y);
  if (chunks_.find(chunk_pos) == chunks_.end()) {
    Tile::TileId tile_ids[kChunkSize * kChunkSize];
    map_gen_->generateTileIds(chunk_x, chunk_y, tile_ids);
    chunks_.emplace(
        std::make_pair(chunk_pos, Chunk(chunk_x, chunk_y, tile_ids)));
  }
}

void ChunkManager::renderTiles(sf::RenderWindow& window) {
  for (auto& [unused_chunk_coords, chunk] : chunks_) {
    chunk.render(window);
  }
}

Tile& ChunkManager::getTile(int grid_x, int grid_y) {
  int chunk_coord_x = gridToChunkCoordinate(grid_x);
  int chunk_coord_y = gridToChunkCoordinate(grid_y);
  Chunk& chunk = chunks_.at({chunk_coord_x, chunk_coord_y});
  int relative_tile_x = gridToRelativeTile(grid_x);
  int relative_tile_y = gridToRelativeTile(grid_y);
  Tile& tile = chunk.getRelativeTile(relative_tile_x, relative_tile_y);
  return tile;
}
