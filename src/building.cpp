#include "building.h"

#include <iostream>

std::map<Building::Ressources, int> RessourceToAmount = {
    {Building::FOOD, 100},
    {Building::GOLD, 100},
};

int Building::returnRessourceAmount(Building::Ressources res) {
  return RessourceToAmount.at(res);
}
