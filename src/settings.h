#ifndef ECOSIM_SETTINGS_H
#define ECOSIM_SETTINGS_H

#include <SFML/System/Vector2.hpp>

// Minimum/maximum viewable world area.
const sf::Vector2f kMinViewSize = {160, 90};
const sf::Vector2f kMaxViewSize = {1600, 900};

// Scroll speed in screen heights per second.
const float kScrollSpeed = 0.8;

const int kCarrierCapacity = 100;
const float kFoodDecay = 100. / 60.;
const int kHumansPerHouse = 10;
const float kFoodProductionFarm = 100. / 60;
const float kSpeedHuman = 8;
const float kUnhappinessSpeedMultipler = 0.2;
const float kMaxFoodPerFarm = 1000.0f;

const float kTopBarHeight = 30.f;
const int kTopBarTextSize = 22;
const float kEntityBarHeight = 60.f;
const int kFoodStatusX = 400;
const int kXSizeHuman = 8;
const int kYSizeHuman = 8;

#endif  // define ECOSIM_SETTINGS_H
