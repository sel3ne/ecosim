#include "human.h"

#include <math.h>

#include <iostream>

#include "building.h"
#include "delivery.h"
#include "farmhouse.h"
#include "game.h"
#include "resource_manager.h"
#include "settings.h"
#include "vector_util.h"

const std::map<Human::Job, std::string> kJobNames{
    {Human::UNEMPLOYED, "Unemployed"},
    {Human::FARMER, "Farmer"},
    {Human::CARRIER, "Carrier"}};

float Human::worldX() { return world_pos_.x; }

float Human::worldY() { return world_pos_.y; }

float Human::worldW() { return world_size_.x; }

float Human::worldH() { return world_size_.y; }

sf::Rect<float> Human::worldRect() { return sf::Rect(world_pos_, world_size_); }

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
  if (!target_entity_) {
    if (returnResourceAmount(RESOURCE_FOOD) == 0) {
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

void Human::handleArrivalAtTarget() {
  switch (job_) {
    case FARMER:
      if (target_entity_->typeOfEntity() == Entity::FARMHOUSE) {
        Farmhouse* farmhouse = dynamic_cast<Farmhouse*>(target_entity_);
        farmhouse->addToAvailableResourceAmount(
            RESOURCE_FOOD, returnResourceAmount(RESOURCE_FOOD));
        setResourceAmount(RESOURCE_FOOD, 0);
        target_entity_ = nullptr;
      } else if (target_entity_->typeOfEntity() == Entity::FARM) {
        Farm* farm = dynamic_cast<Farm*>(target_entity_);
        setResourceAmount(RESOURCE_FOOD,
                          farm->returnAvailableResourceAmount(RESOURCE_FOOD));
        farm->setAvailableResourceAmount(RESOURCE_FOOD, 0.0f);
        farm->set_assigned(false);
        target_entity_ = returnEmployer();
      }
      break;
    case CARRIER: {
      Building* target_building = dynamic_cast<Building*>(target_entity_);
      ResourceId carried_resource = assigned_delivery_->getResource();
      if (target_building == assigned_delivery_->getSource()) {
        target_building->addToReservedResourceAmount(carried_resource,
                                                     -kCarrierCapacity);
        addToResourceAmount(carried_resource, kCarrierCapacity);
        setTargetEntity(assigned_delivery_->getDestination());
      } else if (target_entity_ == assigned_delivery_->getDestination()) {
        addToResourceAmount(carried_resource, -kCarrierCapacity);
        target_building->addToAvailableResourceAmount(carried_resource,
                                                      kCarrierCapacity);
        target_building->addToIncomingResourceAmount(carried_resource,
                                                     -kCarrierCapacity);
        setTargetEntity(nullptr);
        setJob(UNEMPLOYED);
      } else {
        std::cerr << "Carrier arrived at an unknown target!" << std::endl;
        exit(1);
      }
      break;
    }
    default:;
  }
}

void Human::update(float time_s) {
  if (target_entity_) {
    sf::Vector2f direction_vector = target_entity_->worldPos() - worldPos();
    float target_distance = norm(direction_vector);
    sf::Vector2f norm_direction_vector = direction_vector / target_distance;

    float move_distance = kSpeedHuman * time_s;
    world_pos_ += move_distance * norm_direction_vector;

    if (target_distance < 0.6) {
      handleArrivalAtTarget();
    }
  }

  if (job_ == FARMER) {
    updateFarmerFindingFarm(time_s);
  }
}

void Human::setTargetEntity(Entity* target) { target_entity_ = target; }

void Human::assignDelivery(Delivery* delivery) {
  assigned_delivery_ = delivery;
  delivery->setCarrier(this);
  setJob(CARRIER);
  setTargetEntity(delivery->getSource());
}

float Human::returnResourceAmount(ResourceId res) {
  return resource_amounts_.at(res);
}

std::string Human::printJobAndResource() {
  std::string result = "";
  result.append(kJobNames.at(job_));
  result.append("\n");
  for (int resource_idx = 0; resource_idx < _N_RESOURCES; resource_idx++) {
    ResourceId resource = static_cast<ResourceId>(resource_idx);
    std::string resource_name = kResourceNames.at(resource);
    result.append(resource_name);
    result.append(" ");
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
