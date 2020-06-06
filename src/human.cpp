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

sf::Rect<float> Human::worldRect() {
  return sf::Rect(x_world_, y_world_, w_world_, h_world_);
}

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

void Human::updateFarmerFindingFarm(float time_s) {
  if (target_entity_) {
    sf::Vector2f directionVector;
    directionVector.x = target_entity_->worldX() - worldX();
    directionVector.y = target_entity_->worldY() - worldY();

    float vectorSize = sqrt(directionVector.x * directionVector.x +
                            directionVector.y * directionVector.y);

    sf::Vector2f directionVectorNorm;
    directionVectorNorm.x = directionVector.x / vectorSize;
    directionVectorNorm.y = directionVector.y / vectorSize;

    float move_distance = kSpeedHuman * time_s;
    x_world_ = (float)x_world_ + directionVectorNorm.x * move_distance;
    y_world_ = (float)y_world_ + directionVectorNorm.y * move_distance;

    if (vectorSize < 0.6 &&
        target_entity_->typeOfEntity() == Entity::FARMHOUSE) {
      Farmhouse* farmhouse = dynamic_cast<Farmhouse*>(target_entity_);
      farmhouse->setAvailableResourceAmount(
          RESOURCE_FOOD, returnResourceAmount(RESOURCE_FOOD));
      setResourceAmount(RESOURCE_FOOD, 0);
      target_entity_ = nullptr;
    } else if (vectorSize < 0.6 &&
               target_entity_->typeOfEntity() == Entity::FARM) {
      Farm* farm = dynamic_cast<Farm*>(target_entity_);
      setResourceAmount(RESOURCE_FOOD,
                        farm->returnAvailableResourceAmount(RESOURCE_FOOD));
      farm->setAvailableResourceAmount(RESOURCE_FOOD, 0.0f);
      farm->set_assigned(false);
      target_entity_ = nullptr;
    }
  } else {
    if (returnResourceAmount(RESOURCE_FOOD) != 0) {
      // human is at farm, new target will be his farmhouse aka employer
      target_entity_ = returnEmployer();
      // farm_farmhouse_toggle = !farm_farmhouse_toggle;
    } else if (returnResourceAmount(RESOURCE_FOOD) == 0) {
      // human is at farmhouse, new target will be closest full farm
      Farmhouse* employer_farmhouse = dynamic_cast<Farmhouse*>(employer_);
      Farm* next_farm = employer_farmhouse->closestFullFarm();

      // set this farm as assigned, so no one else will go there
      if (next_farm) {
        target_entity_ = next_farm;

        next_farm->set_assigned(true);
      }
    }
  }
}

void Human::update(float time_s) {
  if (job_ == FARMER) {
    updateFarmerFindingFarm(time_s);
  }
}

void Human::setTargetEntity(Entity* target) { target_entity_ = target; }

float Human::returnResourceAmount(ResourceId res) {
  return resource_amounts_.at(res);
}

std::string Human::printResource() {
  std::string result = "";
  result.append(JobNames[(int)job_]);
  result.append("\n");
  for (int i = 0; i < _N_RESOURCES; i++) {
    std::string resource_name = ResourceNames[i];
    result.append(resource_name);
    result.append(" ");
    ResourceId resource = static_cast<ResourceId>(i);
    result.append(std::to_string(returnResourceAmount(resource)));
    result.append("\n");
  }
  return result;
}

void Human::addToResourceAmount(ResourceId res, float delta_amount) {
  float old_amount = Human::returnResourceAmount(res);
  float new_amount = old_amount + delta_amount;
  resource_amounts_[res] = new_amount;
}

void Human::setResourceAmount(ResourceId res, float set_amount) {
  resource_amounts_[res] = set_amount;
}
