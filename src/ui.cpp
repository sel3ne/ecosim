#include "ui.h"

#include <sstream>

#include "grid.h"
#include "resource_manager.h"

/*
 * Defining this function makes it possible to write:
 * stream << my_vector;
 * where stream is an std::ostringstream and my_vector is an sf::Vector
 */
template <typename T>
std::ostream& operator<<(std::ostream& out, const sf::Vector2<T>& vec) {
  out << "(" << vec.x << "," << vec.y << ")";
  return out;
}

void UI::renderDebugView(sf::RenderWindow& window) {
  if (!debug_view_enabled_) {
    return;
  }
  sf::Vector2i mouse_pos_screen = sf::Mouse::getPosition(window);
  sf::Vector2f mouse_pos_world = window.mapPixelToCoords(mouse_pos_screen);
  sf::Vector2i mouse_pos_grid = worldCoordinateToGrid(mouse_pos_world);

  std::ostringstream oss;
  oss << "Mouse pos: " << mouse_pos_screen << " / " << mouse_pos_world << " / "
      << mouse_pos_grid;
  sf::Text mouse_pos_text(oss.str(), *gResourceManager->getFont(FONT_COURIER),
                          20);
  mouse_pos_text.setPosition(0, 0);
  mouse_pos_text.setStyle(sf::Text::Bold);

  window.draw(mouse_pos_text);
}

void UI::render(sf::RenderWindow& window) {
  sf::View saved_view = window.getView();
  window.setView(window.getDefaultView());

  renderDebugView(window);

  window.setView(saved_view);
}
