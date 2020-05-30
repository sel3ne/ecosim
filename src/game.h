#ifndef ECOSIM_GAME_H
#define ECOSIM_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "resource_manager.h"
#include "ui.h"
#include "world.h"

class Game {
 public:
  Game() = delete;
  Game(std::unique_ptr<sf::RenderWindow> window);

  void render();
  void runMainLoop();
  void update(float time_s);
  World& returnWorld();

  void moveView(float dx, float dy);

 private:
  void handleKeyPress(sf::Event::EventType type,
                      const sf::Event::KeyEvent& key_event);

  std::unique_ptr<World> world_;
  std::unique_ptr<sf::RenderWindow> window_;
  UI ui_;
};

extern Game* gGame;

#endif  // define ECOSIM_GAME_H
