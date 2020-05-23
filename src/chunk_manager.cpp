#include "chunk_manager.h"

ChunkManager::ChunkManager() {
  chunks_.emplace(std::make_pair(sf::Vector2i(0, 0), Chunk(0, 0)));
}

void ChunkManager::renderTiles(sf::RenderWindow& window) {
  for (const auto& [unused_chunk_coords, chunk] : chunks_) {
    chunk.render(window);
  }
}
