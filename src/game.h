#ifndef ECOSIM_GAME_H
#define ECOSIM_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "resource_manager.h"
#include "ui.h"
#include "world.h"

// For a given factor the player is trying to zoom in/out, returns the nearest
// legal value that we can actually zoom. I.e. the value gets clamped in case
// we are zooming in or out too far.
float limitZoomFactor(const sf::View& view, const sf::Vector2f& min_size,
                      const sf::Vector2f& max_size, float zoom);

class Game {
 public:
  Game() = delete;
  Game(std::unique_ptr<sf::RenderWindow> window);

  void render();
  void runMainLoop();
  void update(float time_s);
  World& returnWorld();

  void moveView(float dx, float dy);

  template <typename EntityClass>
  EntityClass* buildConstructibleAtMouse(int w_grid, int h_grid,
                                         Entity::EntityType entity_type);

  float totalTimePlayed() { return total_time_played_; }

 private:
  void handleKeyPress(const sf::Event::KeyEvent& key_event);

  std::unique_ptr<World> world_;
  std::unique_ptr<sf::RenderWindow> window_;
  UI ui_;

  float total_time_played_ = 0.f;
};

extern Game* gGame;

#endif  // define ECOSIM_GAME_H
