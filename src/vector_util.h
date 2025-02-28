#ifndef ECOSIM_VECTOR_UTIL_H
#define ECOSIM_VECTOR_UTIL_H

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstddef>
#include <functional>
#include <ostream>

/*
 * Defines how to hash a sf::Vector2i. This is necessary to make it usable
 * as a key in an std::unordered_map.
 */
namespace std {
template <typename T>
struct hash<sf::Vector2<T>> {
  std::size_t operator()(const sf::Vector2<T>& vec) const noexcept {
    std::size_t hx = std::hash<T>{}(vec.x);
    std::size_t hy = std::hash<T>{}(vec.y);
    // It doesn't really matter how we combine the two hash values. To make
    // collisions less likely we shift one a bit to prevent pairs of two times
    // the same value to result in a hash of 0.
    return hx ^ (hy << 1);
  }
};
}  // namespace std

/*
 * Defining this function makes it possible to write:
 * stream << my_vector;
 * where stream is an std::ostringstream and my_vector is an sf::Vector
 */
template <typename T>
std::ostream& operator<<(std::ostream& out, const sf::Vector2<T>& vec) {
  out << vec.x << "," << vec.y;
  return out;
}

float squaredDistance(sf::Vector2f a, sf::Vector2f b);

template <typename T>
float norm(sf::Vector2<T> vec) {
  return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

template <typename T>
T dotProduct(sf::Vector2<T> v1, sf::Vector2<T> v2) {
  return v1.x * v2.x + v1.y * v2.y;
}

#endif  // ifndef ECOSIM_VECTOR_UTIL_H
