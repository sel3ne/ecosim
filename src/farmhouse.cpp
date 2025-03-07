#include "farmhouse.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

#include "game.h"
#include "resource_manager.h"
#include "settings.h"
#include "world.h"

Farm* Farmhouse::closestFullFarm() {
  Farm* nearest_full_farm = nullptr;
  std::function<void(Entity&)> store_min = [this,
                                            &nearest_full_farm](Entity& ent) {
    if (ent.typeOfEntity() == Entity::FARM) {
      Farm* farm = dynamic_cast<Farm*>(&ent);
      // check if farm is already assigned
      if (!farm->is_assigned()) {
        // check if farm is full
        if (farm->returnAvailableResourceAmount(RESOURCE_FOOD) >=
            kMaxFoodPerFarm) {
          if (!nearest_full_farm) {
            // if it is the first farm, return it
            nearest_full_farm = farm;
          } else {
            int x_farm = farm->gridX() - gridX();
            int y_farm = farm->gridY() - gridY();
            int x_nearest_farm = nearest_full_farm->gridX() - gridX();
            int y_nearest_farm = nearest_full_farm->gridY() - gridY();
            int distance_farm = x_farm * x_farm + y_farm + y_farm;
            int distance_nearest_farm = x_nearest_farm * x_nearest_farm +
                                        y_nearest_farm * y_nearest_farm;
            // check if it is closer than the previos farm
            if (distance_farm < distance_nearest_farm) {
              nearest_full_farm = farm;
            }
          }
        }
      }
    }
  };

  gWorld->doForAllEntities(store_min);

  return nearest_full_farm;
}

void Farm::update(float time_s) {
  if (returnAvailableResourceAmount(RESOURCE_FOOD) != kMaxFoodPerFarm) {
    float delta_amount = kFoodProductionFarm * time_s;
    addToAvailableResourceAmount(RESOURCE_FOOD, delta_amount);

    if (returnAvailableResourceAmount(RESOURCE_FOOD) > kMaxFoodPerFarm) {
      setAvailableResourceAmount(RESOURCE_FOOD, kMaxFoodPerFarm);
    }
  }
}

void Farm::render(sf::RenderWindow& window) {
  TextureId texID = TEXTURE_FARM;

  sf::Texture* texture = gResourceManager->getTexture(texID);
  sf::Vector2u texSize = texture->getSize();
  sf::Sprite sprite;
  sprite.setTexture(*texture);
  sprite.setPosition(worldX(), worldY());
  sprite.setScale(worldW() / texSize.x, worldH() / texSize.y);
  window.draw(sprite);

  if (returnAvailableResourceAmount(RESOURCE_FOOD) == kMaxFoodPerFarm) {
    TextureId texID1 = TEXTURE_ONE;
    sf::Texture* texture1 = gResourceManager->getTexture(texID1);
    sf::Vector2u texSize1 = texture->getSize();
    sf::Sprite sprite1;
    sprite1.setTexture(*texture1);
    sprite1.setPosition(worldX(), worldY());
    sprite1.setScale(worldW() / texSize1.x / 10, worldH() / texSize1.y / 10);
    window.draw(sprite1);
  }
}
