#include "entity.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

#include "grid.h"
#include "resource_manager.h"

const std::map<Entity::EntityType, TextureId> kEntityTypeToTextureId = {
    {Entity::HOUSE, TEXTURE_HOUSE},
    {Entity::LIGHTHOUSE, TEXTURE_LIGHTHOUSE},
    {Entity::FARMHOUSE, TEXTURE_FARMHOUSE},
    {Entity::FARM, TEXTURE_FARM},
};

void Entity::render(sf::RenderWindow& window) {
  TextureId texID = kEntityTypeToTextureId.at(entity_type_);
  sf::Texture* texture = gResourceManager->getTexture(texID);
  sf::Vector2u texSize = texture->getSize();
  sf::Sprite sprite;
  sprite.setTexture(*texture);
  sprite.setPosition(worldX(), worldY());
  sprite.setScale(worldW() / texSize.x, worldH() / texSize.y);
  window.draw(sprite);
}

Entity::EntityType Entity::typeOfEntity() { return entity_type_; }

void Entity::visualizeClickedEntity(sf::RenderWindow& window) {
  sf::RectangleShape rectangle(worldSize());
  rectangle.setFillColor(sf::Color::Transparent);
  rectangle.setOutlineThickness(1.f);
  rectangle.setOutlineColor(sf::Color::Black);
  rectangle.setPosition(worldX(), worldY());
  window.draw(rectangle);
}

const std::map<Entity::EntityType, std::string> kEntityNames = {
    {Entity::LIGHTHOUSE, "Lighthouse"},
    {Entity::HOUSE, "House"},
    {Entity::ROAD, "Road"},
    {Entity::HUMAN, "Human"},
    {Entity::FARMHOUSE, "Farmhouse"},
    {Entity::FARM, "Farmli"},
};
