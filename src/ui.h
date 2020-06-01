#ifndef ECOSIM_UI_H
#define ECOSIM_UI_H

#include <SFML/Graphics.hpp>

class UI {
 public:
  UI() : debug_view_enabled_(false) {}

  void render(sf::RenderWindow& window);

  void toggleDebugView() { debug_view_enabled_ = !debug_view_enabled_; }

 private:
  void renderDebugView(sf::RenderWindow& window);
  void renderTopBar(sf::RenderWindow& window);
  void renderTooltip(sf::RenderWindow& window);
  void renderFoodStatus(sf::RenderWindow& window);

  bool debug_view_enabled_;
};

#endif
