#ifndef ECOSIM_GAME_H
#define ECOSIM_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "resource_manager.h"
#include "world.h"

class Game {
 public:
  Game() = delete;
  Game(std::unique_ptr<sf::RenderWindow> window);

  void render();
  void runMainLoop();

 private:
  std::unique_ptr<World> world_;
  std::unique_ptr<sf::RenderWindow> window_;
  ResourceManager resource_mgr_;
};

#endif  // define ECOSIM_GAME_H