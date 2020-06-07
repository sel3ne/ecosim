#ifndef ECOSIM_HUMAN_H
#define ECOSIM_HUMAN_H

#include <SFML/Graphics.hpp>
#include <map>

#include "entity.h"
#include "resources.h"

class Delivery;

class Human : public Entity {
 public:
  enum Job {
    UNEMPLOYED,
    FARMER,
    CARRIER,
  };

  Human() = delete;

  Human(float x_world, float y_world, float w_world, float h_world,
        Entity::EntityType entity_type, Entity* target = nullptr,
        int food_start_amount = 0, Job job = UNEMPLOYED)
      : Entity(entity_type),
        world_pos_(x_world, y_world),
        world_size_(w_world, h_world),
        happiness_(true),
        target_entity_(target),
        job_(job) {
    for (int i = 0; i < _N_RESOURCES; i++) {
      resource_amounts_[static_cast<ResourceId>(i)] = 0.0f;
    }
    resource_amounts_[RESOURCE_FOOD] = food_start_amount;
  }

  void setTargetEntity(Entity* target);
  void assignDelivery(Delivery* delivery);

  virtual void render(sf::RenderWindow& window);

  virtual float worldX();
  virtual float worldY();
  virtual float worldW();
  virtual float worldH();
  virtual sf::Rect<float> worldRect();

  float returnResourceAmount(ResourceId res);
  void addToResourceAmount(ResourceId res, float delta_amount);
  void setResourceAmount(ResourceId res, float set_amount);
  std::string printJobAndResource();

  void setJob(Job job) { job_ = job; }
  Job returnJob() const { return job_; }

  void setEmployer(Entity* employer) { employer_ = employer; };
  Entity* returnEmployer() { return employer_; }

  void setHappiness(bool happy) { happiness_ = happy; }

  virtual void update(float time_s);
  void updateFarmerFindingFarm(float time_s);

  virtual bool isBuilding() { return false; };

 private:
  void handleArrivalAtTarget();

  sf::Vector2f world_pos_;
  sf::Vector2f world_size_;
  bool happiness_;
  Entity* target_entity_;
  Entity* employer_ = nullptr;
  std::map<ResourceId, float> resource_amounts_;
  Job job_ = UNEMPLOYED;
  Delivery* assigned_delivery_;
};

extern const std::map<Human::Job, std::string> kJobNames;

#endif  // define ECOSIM_HUMAN_H
