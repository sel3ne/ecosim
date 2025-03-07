#ifndef ECOSIM_UI_H
#define ECOSIM_UI_H

#include <SFML/Graphics/Rect.hpp>

#include "building.h"

namespace sf {
class RenderWindow;
class View;
}  // namespace sf
class Entity;

class UI {
 public:
  UI() : debug_view_enabled_(false) {}

  void render(sf::RenderWindow& window);

  void toggleDebugView() { debug_view_enabled_ = !debug_view_enabled_; }

  void handleClickEvent(sf::Vector2i window_mouse_position,
                        sf::RenderWindow& window);

  void setGhostArrow(sf::Vector2i window_mouse_position,
                     sf::RenderWindow& window);

  void toggleArrow(sf::Vector2i window_mouse_position, sf::RenderWindow& window,
                   ResourceId resource);

 private:
  void renderDebugView(sf::RenderWindow& window, const sf::View& world_view);
  void renderTopBar(sf::RenderWindow& window);
  void renderTooltip(sf::RenderWindow& window);
  void renderFoodStatus(sf::RenderWindow& window);
  void renderEntityInfo(sf::RenderWindow& window);
  void renderArrowBuilding(sf::RenderWindow& window, Building* building);
  void renderVisualizeClickedEntity(sf::RenderWindow& window);

  bool debug_view_enabled_;

  Entity* clicked_entity_ = nullptr;
  std::pair<Building*, Building*> ghost_arrow_ = {nullptr, nullptr};
};

#endif
