#include "building.h"

#include <iostream>

std::map<Building::Resources, int> ResourceToAmount = {
    {Building::FOOD, 100},
    {Building::GOLD, 100},
};

int Building::returnResourceAmount(Building::Resources res) {
  return ResourceToAmount.at(res);
}
