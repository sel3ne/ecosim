#include <SFML/Graphics.hpp>
#include <iostream>

#include "game.h"
#include "resource_manager.h"

using std::make_unique;
using std::unique_ptr;

int main(int argc, char *argv[]) {
  // Creating the ResourceManager loads all resources.
  gResourceManager = new ResourceManager;

  unique_ptr<sf::RenderWindow> window = make_unique<sf::RenderWindow>(
      sf::VideoMode(1920, 1080), "Economy Simulator");

  Game game(std::move(window));
  game.runMainLoop();
  return 0;
}
