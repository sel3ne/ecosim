#ifndef ECOSIM_HUMAN_H
#define ECOSIM_HUMAN_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "constructible.h"

class Human : public Entity {
 public:
  enum ResourcesCarrie {
    FOOD = 0,
    GOLD,

    _N_RESOURCES,
  };

  enum Job {
    UNEMPOLYED,
    FARMER,

  };

  Human() = delete;
  Human(float x_world, float y_world, float w_world, float h_world,
        Entity::EntityType entity_type)
      : Entity(entity_type),
        x_world_(x_world),
        y_world_(y_world),
        w_world_(w_world),
        h_world_(h_world) {
    happiness_ = 1;
    target_entity_ = nullptr;
    for (int i = 0; i < _N_RESOURCES; i++) {
      Human::ResourceToAmount[static_cast<ResourcesCarrie>(i)] = 0.0f;
    }
  }

  Human(float x_world, float y_world, float w_world, float h_world,
        Entity::EntityType entity_type, Entity* target)
      : Entity(entity_type),
        x_world_(x_world),
        y_world_(y_world),
        w_world_(w_world),
        h_world_(h_world),
        target_entity_(target) {
    happiness_ = 1;
    for (int i = 0; i < _N_RESOURCES; i++) {
      Human::ResourceToAmount[static_cast<ResourcesCarrie>(i)] = 0.0f;
    }
  }

  Human(float x_world, float y_world, float w_world, float h_world,
        Entity::EntityType entity_type, Entity* target, int food_start_amount)
      : Entity(entity_type),
        x_world_(x_world),
        y_world_(y_world),
        w_world_(w_world),
        h_world_(h_world),
        target_entity_(target) {
    happiness_ = 1;
    for (int i = 0; i < _N_RESOURCES; i++) {
      Human::ResourceToAmount[static_cast<ResourcesCarrie>(i)] = 0.0f;
    }
    Human::ResourceToAmount[FOOD] = food_start_amount;
  }

  Human(float x_world, float y_world, float w_world, float h_world,
        Entity::EntityType entity_type, Entity* target, int food_start_amount,
        Job job)
      : Entity(entity_type),
        x_world_(x_world),
        y_world_(y_world),
        w_world_(w_world),
        h_world_(h_world),
        target_entity_(target) {
    happiness_ = 1;
    job_ = job;
    for (int i = 0; i < _N_RESOURCES; i++) {
      Human::ResourceToAmount[static_cast<ResourcesCarrie>(i)] = 0.0f;
    }
    Human::ResourceToAmount[FOOD] = food_start_amount;
  }

  void printHappiness() { std::cout << happiness_ << std::endl; }

  void setTargetEntity(Entity* target);

  virtual void render(sf::RenderWindow& window);

  virtual float worldX();
  virtual float worldY();
  virtual float worldW();
  virtual float worldH();
  virtual sf::Rect<float> worldRect();

  float returnResourceAmount(Human::ResourcesCarrie res);

  void adaptResource(Human::ResourcesCarrie res, float delta_amount);

  void setResourceToAmount(Human::ResourcesCarrie res, float set_amount);

  void setJob(Job job) { job_ = job; }

  Job returnJob() { return job_; }

  void setEmployer(Entity* employer) { employer_ = employer; };

  void set_happiness(bool happy) { happiness_ = happy; }

  Entity* returnEmployer() { return employer_; }

  virtual void update(float time_s);
  void updateFarmerFindingFarm(float time_s);

 private:
  float x_world_;
  float y_world_;
  float w_world_;
  float h_world_;
  bool happiness_;
  Entity* target_entity_;
  Entity* employer_ = nullptr;
  std::map<Human::ResourcesCarrie, float> ResourceToAmount;
  Job job_ = UNEMPOLYED;
};

#endif  // define ECOSIM_HUMAN_H
