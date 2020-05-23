#include "entity.h"

#include <iostream>

#include "grid.h"
#include "resource_manager.h"

const std::map<Entity::EntityType, TextureID> kEntityTypeToTextureID = {
    {Entity::HOUSE, TEXTURE_HOUSE},
    {Entity::LIGHTHOUSE, TEXTURE_LIGHTHOUSE},
    {Entity::HUMAN, TEXTURE_HUMAN},
};

void Entity::render(sf::RenderWindow& window) {
  TextureID texID = kEntityTypeToTextureID.at(entity_type_);
  sf::Texture* texture = gResourceManager->getTexture(texID);
  sf::Vector2u texSize = texture->getSize();
  sf::Sprite sprite;
  sprite.setTexture(*texture);
  sprite.setPosition(worldX(), worldY());
  sprite.setScale(worldW() / texSize.x, worldH() / texSize.y);
  window.draw(sprite);
}

Entity::EntityType Entity::typeOfEntity() { return entity_type_; }
