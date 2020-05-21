#include "game.h"

void Game::render() {
  window_->clear();
  // window_->draw(shape);
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
