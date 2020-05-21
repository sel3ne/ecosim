#include "world.h"

void World::render(sf::RenderWindow& window) {
  chunk_manager_.renderTiles(window);
}