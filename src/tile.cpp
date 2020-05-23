#include "tile.h"

#include "grid.h"
#include "resource_manager.h"

const std::unordered_map<Tile::TileId, TextureID> kTexturesOfTiles = {
    {Tile::GRASS, TEXTURE_GRASS},
    {Tile::WATER, TEXTURE_GRASS},
    {Tile::STONE, TEXTURE_GRASS},
};

void Tile::render(sf::RenderWindow& window, int grid_x, int grid_y) const {
  TextureID tex_id = kTexturesOfTiles.at(id_);
  sf::Texture* texture = gResourceManager->getTexture(tex_id);
  sf::Vector2u tex_size = texture->getSize();
  sf::Sprite sprite;
  sprite.setPosition(grid_x * kPixelsPerTile, grid_y * kPixelsPerTile);
  sprite.setScale(kPixelsPerTile / tex_size.x, kPixelsPerTile / tex_size.y);
  window.draw(sprite);
}