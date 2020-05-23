#include "chunk_manager.h"

ChunkManager::ChunkManager() {
  map_gen_ = std::make_unique<SimpleIslandGenerator>();
  for (int x = -10; x <= 10; ++x) {
    for (int y = -10; y <= 10; ++y) {
      generateChunk(x, y);
    }
  }
}

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
  for (const auto& [unused_chunk_coords, chunk] : chunks_) {
    chunk.render(window);
  }
}
