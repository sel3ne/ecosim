#include "game.h"

#include <iostream>

Game::Game(std::unique_ptr<sf::RenderWindow> window)
    : window_(std::move(window)) {
  world_ = std::make_unique<World>();
}

void Game::render() {
  window_->clear();
  world_->render(*window_);
  window_->display();
}

void Game::runMainLoop() {
  while (window_->isOpen()) {
    sf::Event event;
    while (window_->pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window_->close();
      }
    }

    render();
  }
}
