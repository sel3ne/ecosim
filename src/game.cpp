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
        sf::Texture* texture1 = new sf::Texture;
        if (!texture1->loadFromFile(
                "data/green-grass-textures_74190-5443.png")) {
          std::cout << "could not load the test texture";
        }
        sf::Vector2i position = sf::Mouse::getPosition(*window_);

        Entity entity1(position.x, position.y, texture1);
        world_->addEntityToEntities(std::move(entity1));
      }
      if (event.type == sf::Event::Closed) {
        window_->close();
      }
    }

    render();
  }
}
