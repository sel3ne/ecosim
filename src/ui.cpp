#include "ui.h"

#include <sstream>

#include "game.h"
#include "grid.h"
#include "resource_manager.h"
#include "settings.h"

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

void UI::renderFoodStatus(sf::RenderWindow& window) {
  int base_x = window.getSize().x - kFoodStatusX;

  sf::Sprite grain_icon;
  sf::Texture* texture = gResourceManager->getTexture(TEXTURE_GRAIN);
  grain_icon.setTexture(*texture);
  grain_icon.setPosition(base_x, 0);
  float icon_scale = kTopBarHeight / texture->getSize().y;
  grain_icon.setScale(icon_scale, icon_scale);
  window.draw(grain_icon);

  int num_happy = gGame->returnWorld().returnNumberHappyHuman();
  int num_total = num_happy + gGame->returnWorld().returnNumberUnhappyHuman();
  std::ostringstream food_oss;
  food_oss << num_happy << "/" << num_total;
  sf::Text food_text(food_oss.str(), *gResourceManager->getFont(FONT_COURIER),
                     kTopBarTextSize);
  food_text.setPosition(base_x + kTopBarHeight, 0);
  food_text.setStyle(sf::Text::Bold);
  if (num_happy == num_total) {
    food_text.setFillColor({0, 0, 0});
  } else {
    food_text.setFillColor({210, 0, 0});
  }
  window.draw(food_text);
}

void UI::renderTopBar(sf::RenderWindow& window) {
  // Top bar background.
  sf::RectangleShape background;
  background.setPosition(0, 0);
  background.setSize(
      sf::Vector2f(static_cast<float>(window.getSize().x), kTopBarHeight));
  background.setTexture(gResourceManager->getTexture(TEXTURE_STONE_BAR));
  background.setOutlineThickness(2.f);
  background.setOutlineColor(sf::Color(50, 50, 50));
  window.draw(background);

  // Game time display.
  std::ostringstream time_oss;
  time_oss << static_cast<int>(gGame->totalTimePlayed());
  sf::Text time_played_text(time_oss.str(),
                            *gResourceManager->getFont(FONT_COURIER),
                            kTopBarTextSize);
  time_played_text.setPosition(
      window.getSize().x - time_played_text.getLocalBounds().width - 20, 0);
  time_played_text.setStyle(sf::Text::Bold);
  time_played_text.setFillColor({0, 0, 0});
  window.draw(time_played_text);

  // Food status.
  renderFoodStatus(window);
}

void UI::renderTooltip(sf::RenderWindow& window) {}

void UI::render(sf::RenderWindow& window) {
  sf::View saved_view = window.getView();
  window.setView(window.getDefaultView());

  renderTopBar(window);

  renderDebugView(window);

  window.setView(saved_view);
}
