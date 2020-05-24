#include "farmhouse.h"

#include <iostream>

#include "game.h"
#include "settings.h"

void Farm::update(float time_s) {
  std::cout << "Food in Farm: " << returnResourceAmount(Building::FOOD)
            << std::endl;
  if (returnResourceAmount(Building::FOOD) != 1) {
    float delta_amount = kFoodProductionFarm * time_s;
    adaptResource(Building::FOOD, delta_amount);

    if (returnResourceAmount(Building::FOOD) > 1) {
      setResourceToAmount(Building::FOOD, 1.0f);
    }
  }
}

void Farm::render(sf::RenderWindow& window) {
  TextureID texID = TEXTURE_FARMHOUSE;

  sf::Texture* texture = gResourceManager->getTexture(texID);
  sf::Vector2u texSize = texture->getSize();
  sf::Sprite sprite;
  sprite.setTexture(*texture);
  sprite.setPosition(worldX(), worldY());
  sprite.setScale(worldW() / texSize.x, worldH() / texSize.y);
  window.draw(sprite);

  if (returnResourceAmount(Building::FOOD) == 1) {
    TextureID texID1 = TEXTURE_ONE;
    sf::Texture* texture1 = gResourceManager->getTexture(texID1);
    sf::Vector2u texSize1 = texture->getSize();
    sf::Sprite sprite1;
    sprite1.setTexture(*texture1);
    sprite1.setPosition(worldX(), worldY());
    sprite1.setScale(worldW() / texSize1.x / 10, worldH() / texSize1.y / 10);
    window.draw(sprite1);
  }
}
