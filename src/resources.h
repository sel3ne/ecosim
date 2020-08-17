#ifndef ECOSIM_RESOURCES_H
#define ECOSIM_RESOURCES_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

enum ResourceId {
  RESOURCE_FOOD = 0,
  RESOURCE_GOLD,

  // A fake resource only for tests.
  RESOURCE_TEST,

  _N_RESOURCES,
};

extern const std::map<ResourceId, std::string> kResourceNames;

#endif  // ifndef ECOSIM_RESOURCES_H
