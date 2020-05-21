#ifndef ECOSIM_GAME_H
#define ECOSIM_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>

class Game {
 public:
  Game() = delete;
  Game(std::unique_ptr<sf::RenderWindow> window) : window_(std::move(window)) {}

  void render();
  void runMainLoop();

 private:
  std::unique_ptr<sf::RenderWindow> window_;
};

#endif  // define ECOSIM_GAME_H