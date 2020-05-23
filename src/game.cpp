#include "game.h"

#include <iostream>

#include "constructible.h"
#include "grid.h"
#include "resource_manager.h"

Game::Game(std::unique_ptr<sf::RenderWindow> window)
    : window_(std::move(window)) {
  world_ = std::make_unique<World>();
}

void Game::render() {
  window_->clear();
  world_->render(*window_);
  window_->display();
}

void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom) {
  const sf::Vector2f beforeCoord{window.mapPixelToCoords(pixel)};
  sf::View view{window.getView()};
  view.zoom(zoom);
  window.setView(view);
  const sf::Vector2f afterCoord{window.mapPixelToCoords(pixel)};
  const sf::Vector2f offsetCoords{beforeCoord - afterCoord};
  view.move(offsetCoords);
  window.setView(view);
}

void Game::runMainLoop() {
  while (window_->isOpen()) {
    sf::Event event;
    sf::View currentView = window_->getView();

    while (window_->pollEvent(event)) {
      if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i position = sf::Mouse::getPosition(*window_);
        std::cout << position.x << std::endl << position.y << std::endl;
        sf::Vector2f worldPos = window_->mapPixelToCoords(position);
        sf::Vector2i gridPos = worldCoordinateToGrid(worldPos);
        std::cout << gridPos.x << std::endl << gridPos.y << std::endl;
        sf::Texture* house_tex = gResourceManager->getTexture(TEXTURE_HOUSE);

        sf::Vector2f worldPosEntity = window_->mapPixelToCoords(position);
        sf::Vector2i gridPosEntity = worldCoordinateToGrid(worldPosEntity);
        std::unique_ptr<Entity> constructible = std::make_unique<Constructible>(
            gridPosEntity.x, gridPosEntity.y, 20, 30, house_tex);
        world_->addEntityToEntities(std::move(constructible));
      } else if ((event.type == sf::Event::KeyPressed &&
                  event.key.code == sf::Keyboard::Left) ||
                 (event.type == sf::Event::KeyPressed &&
                  event.key.code == sf::Keyboard::A)) {
        // Move left
        currentView.move(-4.0f, 0.f);
        window_->setView(currentView);
      } else if ((event.type == sf::Event::KeyPressed &&
                  event.key.code == sf::Keyboard::Right) ||
                 (event.type == sf::Event::KeyPressed &&
                  event.key.code == sf::Keyboard::D)) {
        // Move right
        currentView.move(4.0f, 0.f);
        window_->setView(currentView);
      } else if ((event.type == sf::Event::KeyPressed &&
                  event.key.code == sf::Keyboard::Up) ||
                 (event.type == sf::Event::KeyPressed &&
                  event.key.code == sf::Keyboard::W)) {
        // Move up
        currentView.move(0.0f, -4.0f);
        window_->setView(currentView);
      } else if ((event.type == sf::Event::KeyPressed &&
                  event.key.code == sf::Keyboard::Down) ||
                 (event.type == sf::Event::KeyPressed &&
                  event.key.code == sf::Keyboard::S)) {
        // Move down
        currentView.move(0.0f, 4.0f);
        window_->setView(currentView);
      } else if (event.type == sf::Event::MouseWheelScrolled) {
        // Zoom around mouse position
        const float zoomAmount{1.1f};
        if (event.mouseWheelScroll.delta > 0)
          zoomViewAt({event.mouseWheelScroll.x, event.mouseWheelScroll.y},
                     *window_, (1.f / zoomAmount));
        else if (event.mouseWheelScroll.delta < 0)
          zoomViewAt({event.mouseWheelScroll.x, event.mouseWheelScroll.y},
                     *window_, zoomAmount);
      }
      if (event.type == sf::Event::Closed) {
        window_->close();
      }
    }

    render();
  }
}
