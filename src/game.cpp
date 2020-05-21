#include "game.h"

#include <iostream>

#include "entity.h"

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
      if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i position = sf::Mouse::getPosition(*window_);
        sf::Texture* grass_tex = resource_mgr_.getTexture(TEXTURE_GRASS);
        Entity entity1(position.x, position.y, grass_tex);
        world_->addEntityToEntities(std::move(entity1));
      }
      if (event.type == sf::Event::Closed) {
        window_->close();
      }
    }

    render();
  }
}
