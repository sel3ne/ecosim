#ifndef ECOSIM_ENTITY_H
#define ECOSIM_ENTITY_H

#include <SFML/Graphics/Rect.hpp>
#include <map>

namespace sf {
class RenderWindow;
}

class Entity {
 public:
  enum EntityType {
    LIGHTHOUSE,
    HOUSE,
    ROAD,
    HUMAN,
    FARMHOUSE,
    FARM,
  };

  Entity() = delete;
  Entity(EntityType entity_type) : entity_type_(entity_type) {}
  Entity(Entity&& enti) : entity_type_(enti.entity_type_) {}
  Entity(const Entity& enti) = default;

  virtual void render(sf::RenderWindow& window);

  EntityType typeOfEntity();
  virtual float worldX() = 0;
  virtual float worldY() = 0;
  virtual float worldW() = 0;
  virtual float worldH() = 0;
  virtual sf::Rect<float> worldRect() = 0;
  virtual void update(float time_s) = 0;
  virtual bool isBuilding() = 0;

 private:
  EntityType entity_type_;
};

extern const std::map<Entity::EntityType, std::string> kEntityNames;

#endif  // define ECOSIM_ENTITY_H
