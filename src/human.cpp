#include "human.h"

#include <math.h>

#include <iostream>

#include "building.h"
#include "farmhouse.h"
#include "game.h"
#include "resource_manager.h"
#include "settings.h"

float Human::worldX() { return x_world_; }

float Human::worldY() { return y_world_; }

float Human::worldW() { return w_world_; }

float Human::worldH() { return h_world_; }

void Human::render(sf::RenderWindow& window) {
  TextureId texID = happiness_ ? TEXTURE_HUMAN_HAPPY : TEXTURE_HUMAN_UNHAPPY;

  sf::Texture* texture = gResourceManager->getTexture(texID);
  sf::Vector2u texSize = texture->getSize();
  sf::Sprite sprite;
  sprite.setTexture(*texture);
  sprite.setPosition(worldX(), worldY());
  sprite.setScale(worldW() / texSize.x, worldH() / texSize.y);
  window.draw(sprite);
}

void Human::update(float time_s) {
  World& world = gGame->returnWorld();
  int unhappyHuman = world.returnNumberUnhappyHuman();
  int unhappyHouse = world.returnNumberUnhappyHouse();
  if (unhappyHuman < unhappyHouse * kHumansPerHouse && happiness_ == 1) {
    happiness_ = 0;
    world.addNumberUnhappyHuman(1);
    world.addNumberHappyHuman(-1);
  } else if (unhappyHuman > unhappyHouse * kHumansPerHouse && happiness_ == 0) {
    happiness_ = 1;
    world.addNumberUnhappyHuman(-1);
    world.addNumberHappyHuman(1);
  } else if (unhappyHuman * kHumansPerHouse == unhappyHouse) {
    // std::cout << "balanced humans" << std::endl;
  }

  if (target_entity_) {
    sf::Vector2f directionVector;
    directionVector.x = target_entity_->worldX() - worldX();
    directionVector.y = target_entity_->worldY() - worldY();

    float vectorSize = sqrt(directionVector.x * directionVector.x +
                            directionVector.y * directionVector.y);
    if (vectorSize < 0.6 &&
        target_entity_->typeOfEntity() == Entity::FARMHOUSE) {
      Farmhouse* farmhouse = dynamic_cast<Farmhouse*>(target_entity_);
      farmhouse->setResourceToAmount(Building::FOOD,
                                     returnResourceAmount(Human::FOOD));
      setResourceToAmount(Human::FOOD, 0);
      target_entity_ = nullptr;
    }
    sf::Vector2f directionVectorNorm;
    directionVectorNorm.x = directionVector.x / vectorSize;
    directionVectorNorm.y = directionVector.y / vectorSize;

    float move_distance = kSpeedHuman * time_s;
    x_world_ = (float)x_world_ + directionVectorNorm.x * move_distance;
    y_world_ = (float)y_world_ + directionVectorNorm.y * move_distance;
  }
}

void Human::setTargetEntity(Entity* target) { target_entity_ = target; }

float Human::returnResourceAmount(Human::ResourcesCarrie res) {
  return ResourceToAmount.at(res);
}

void Human::adaptResource(Human::ResourcesCarrie res, float delta_amount) {
  float old_amount = Human::returnResourceAmount(res);
  float new_amount = old_amount + delta_amount;
  ResourceToAmount[res] = new_amount;
}

void Human::setResourceToAmount(Human::ResourcesCarrie res, float set_amount) {
  ResourceToAmount[res] = set_amount;
}
