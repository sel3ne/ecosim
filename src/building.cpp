#include "building.h"

#include <iostream>

std::map<Building::Ressources, int> RessourceToAmount = {
    {FOOD, 100},
    {GOLD, 100},
};

int Building::returnRessourceAmount(Building::Ressources res) {
  return RessourceToAmount.at(res)
}