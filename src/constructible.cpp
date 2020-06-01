#include "constructible.h"

#include <iostream>

#include "grid.h"

float Constructible::worldX() { return x_grid_ * kPixelsPerTile; }

float Constructible::worldY() { return y_grid_ * kPixelsPerTile; }

float Constructible::worldW() { return w_grid_ * kPixelsPerTile; }

float Constructible::worldH() { return h_grid_ * kPixelsPerTile; }

sf::Rect<float> Constructible::worldRect() {
  return sf::Rect(worldX(), worldY(), worldW(), worldH());
}
