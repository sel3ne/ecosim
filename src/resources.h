#ifndef ECOSIM_RESOURCES_H
#define ECOSIM_RESOURCES_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

enum ResourceId {
  RESOURCE_FOOD = 0,
  RESOURCE_GOLD,

  _N_RESOURCES,
};

// std::vector<std::string> ResourceNames{"Food", "Gold"};

#endif  // ifndef ECOSIM_RESOURCES_H
