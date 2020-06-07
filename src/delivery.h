#ifndef ECOSIM_DELIVERY_H
#define ECOSIM_DELIVERY_H

#include "resources.h"

class Building;
class Human;

class Delivery {
 public:
  Delivery() = delete;
  Delivery(Building* source, Building* destination, Human* carrier,
           ResourceId resource)
      : source_(source),
        destination_(destination),
        carrier_(carrier),
        resource_(resource) {}

  Building* getSource() { return source_; }
  Building* getDestination() { return destination_; }
  Human* getCarrier() { return carrier_; }
  ResourceId getResource() { return resource_; }

  void setCarrier(Human* human) { carrier_ = human; }

 private:
  Building* source_;
  Building* destination_;
  Human* carrier_;
  ResourceId resource_;
};

#endif  // define ECOSIM_DELIVERY_H
