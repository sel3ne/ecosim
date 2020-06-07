#include "vector_util.h"

float squaredDistance(sf::Vector2f a, sf::Vector2f b) {
  sf::Vector2f distance_vector = a - b;
  return distance_vector.x * distance_vector.x +
         distance_vector.y * distance_vector.y;
}
