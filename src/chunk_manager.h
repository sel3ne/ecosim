#ifndef ECOSIM_CHUNK_MANAGER_H
#define ECOSIM_CHUNK_MANAGER_H

#include <memory>
#include <unordered_map>

#include "chunk.h"
#include "vector_util.h"

class MapGenerator;

class ChunkManager {
 public:
  ChunkManager();
  ~ChunkManager();

  void generateChunk(int chunk_x, int chunk_y);

  void renderTiles(sf::RenderWindow& window);

  Tile& getTile(int grid_x, int grid_y);

 private:
  std::unordered_map<sf::Vector2i, Chunk> chunks_;
  std::unique_ptr<MapGenerator> map_gen_;
};

#endif  // define ECOSIM_CHUNK_MANAGER_H
