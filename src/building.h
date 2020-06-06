#ifndef ECOSIM_BUILDING_H
#define ECOSIM_BUILDING_H

#include <map>
#include <set>

#include "constructible.h"
#include "resources.h"

class Delivery;

class Building : public Constructible {
 public:
  Building() = delete;
  Building(int x_grid, int y_grid, int w_grid, int h_grid,
           Entity::EntityType entity_type);

  float returnAvailableResourceAmount(ResourceId res);
  float returnAvailableAndReservedResourceAmount(ResourceId res);
  void addToAvailableResourceAmount(ResourceId res, float delta_amount);
  void setAvailableResourceAmount(ResourceId res, float set_amount);
  std::string printNameAndResource();

  const std::set<Building*>& getDeliveryTargets(ResourceId resource);
  void addDeliveryTarget(ResourceId resource, Building* target);
  void removeDeliveryTarget(ResourceId resource, Building* target);

  virtual void update(float time_s);
  virtual bool isBuilding() { return true; };

 private:
  // How many resources are stored and can be used.
  std::map<ResourceId, float> resources_available_;
  // How many resources are stored, but are reserved for pickup.
  std::map<ResourceId, int> resources_reserved_;
  // How many resources are on their way here.
  std::map<ResourceId, int> resources_incoming_;
  // How many resources this building would like to have in total.
  std::map<ResourceId, int> resources_required_;

  // Which buildings we are distributing our outputs to.
  std::map<ResourceId, std::set<Building*>> delivering_to_;
  // Which buildings we receive resources from.
  std::map<ResourceId, std::set<Building*>> receiving_from_;

  // Deliveries currently on their way here.
  std::vector<Delivery*> incoming_deliveries_;
  // Deliveries that will either pick something up from here or that have
  // already done so, but have not reached the target.
  std::vector<Delivery*> outgoing_deliveries_;
};

#endif  // define ECOSIM_BUILDING_H
