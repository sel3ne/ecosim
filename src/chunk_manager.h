#ifndef ECOSIM_CHUNK_MANAGER_H
#define ECOSIM_CHUNK_MANAGER_H

#include <SFML/Graphics.hpp>

#include "chunk.h"
#include "map_generator.h"

/*
 * Defines how to hash a sf::Vector2i. This is necessary to make it usable
 * as a key in an std::unordered_map.
 */
namespace std {
template <>
struct hash<sf::Vector2i> {
  std::size_t operator()(const sf::Vector2i& vec) const noexcept {
    std::size_t hx = std::hash<int>{}(vec.x);
    std::size_t hy = std::hash<int>{}(vec.y);
    // It doesn't really matter how we combine the two hash values. To make
    // collisions less likely we shift one a bit to prevent pairs of two times
    // the same value to result in a hash of 0.
    return hx ^ (hy << 1);
  }
};
}  // namespace std

class ChunkManager {
 public:
  ChunkManager();

  void generateChunk(int chunk_x, int chunk_y);

  void renderTiles(sf::RenderWindow& window);

 private:
  std::unordered_map<sf::Vector2i, Chunk> chunks_;
  std::unique_ptr<MapGenerator> map_gen_;
};

#endif  // define ECOSIM_CHUNK_MANAGER_H
