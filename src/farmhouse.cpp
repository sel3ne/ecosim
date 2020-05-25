#include "farmhouse.h"

#include <iostream>

#include "game.h"
#include "settings.h"

Farm* Farmhouse::closestFullFarm() {
  World& world = gGame->returnWorld();

  Farm* nearest_full_farm = nullptr;
  std::function<void(Entity&)> store_min = [this,
                                            &nearest_full_farm](Entity& ent) {
    if (ent.typeOfEntity() == Entity::FARM) {
      Farm* farm = dynamic_cast<Farm*>(&ent);
      // check if farm is already assigned
      if (!farm->is_assigned()) {
        // check if farm is full
        if (farm->returnResourceAmount(Building::FOOD) >= kMaxFoodPerFarm) {
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

  world.doForAllEntities(store_min);

  return nearest_full_farm;
}

void Farm::update(float time_s) {
  std::cout << "Food in Farm: " << returnResourceAmount(Building::FOOD)
            << std::endl;
  if (returnResourceAmount(Building::FOOD) != kMaxFoodPerFarm) {
    float delta_amount = kFoodProductionFarm * time_s;
    adaptResource(Building::FOOD, delta_amount);

    if (returnResourceAmount(Building::FOOD) > kMaxFoodPerFarm) {
      setResourceToAmount(Building::FOOD, kMaxFoodPerFarm);
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

  if (returnResourceAmount(Building::FOOD) == kMaxFoodPerFarm) {
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
