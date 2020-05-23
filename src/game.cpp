#include "game.h"

#include <iostream>
#include <random>

#include "building.h"
#include "grid.h"
#include "human.h"
#include "resource_manager.h"

Game* gGame = nullptr;

Game::Game(std::unique_ptr<sf::RenderWindow> window)
    : window_(std::move(window)) {
  world_ = std::make_unique<World>();
  window_->setFramerateLimit(60);
}

void Game::render() {
  window_->clear();
  world_->render(*window_);
  window_->display();
}

World& Game::returnWorld() { return *world_; }

void Game::update(float time_s) { world_->update(time_s); }

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

float RandomFloat(float beg, float end) {
  std::random_device
      rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(beg, end);
  return dis(gen);
}

void Game::runMainLoop() {
  sf::Clock clock;
  sf::Event event;
  while (window_->isOpen()) {
    sf::View currentView = window_->getView();

    while (window_->pollEvent(event)) {
      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::O) {
        // House event
        sf::Vector2i position = sf::Mouse::getPosition(*window_);
        // std::cout << position.x << std::endl << position.y << std::endl;
        // sf::Vector2f worldPos = window_->mapPixelToCoords(position);
        // sf::Vector2i gridPos = worldCoordinateToGrid(worldPos);
        sf::Vector2f worldPosEntity = window_->mapPixelToCoords(position);
        sf::Vector2i gridPosEntity = worldCoordinateToGrid(worldPosEntity);
        std::unique_ptr<Entity> constructible = std::make_unique<Building>(
            gridPosEntity.x, gridPosEntity.y, 3, 3, Entity::HOUSE);
        world_->addNumberHappyHouse(1);

        world_->addEntityToEntities(std::move(constructible));
        // add the 10 humans per house

        for (int i = 0; i < 10; i++) {
          world_->addNumberHappyHuman(1);
          int x_coord = worldPosEntity.x +
                        RandomFloat(-kPixelsPerTile * 2, kPixelsPerTile * 2);
          int y_coord = worldPosEntity.y +
                        RandomFloat(-kPixelsPerTile * 2, kPixelsPerTile * 2);
          std::unique_ptr<Entity> human =
              std::make_unique<Human>(x_coord, y_coord, 8, 8, Entity::HUMAN);
          world_->addEntityToEntities(std::move(human));
        }

      } else if (event.type == sf::Event::KeyPressed &&
                 event.key.code == sf::Keyboard::P) {
        // Lighthouse event
        sf::Vector2i position = sf::Mouse::getPosition(*window_);
        // std::cout << position.x << std::endl << posit<<ion.y << std::endl;
        // sf::Vector2f worldPos = window_->mapPixelToCoords(position);
        // sf::Vector2i gridPos = worldCoordinateToGrid(worldPos);
        sf::Vector2f worldPosEntity = window_->mapPixelToCoords(position);
        sf::Vector2i gridPosEntity = worldCoordinateToGrid(worldPosEntity);
        std::unique_ptr<Entity> constructible = std::make_unique<Building>(
            gridPosEntity.x, gridPosEntity.y, 2, 4, Entity::LIGHTHOUSE);
        world_->addNumberLighthouse(1);
        std::cout << world_->returnNumberLighthouse() << std::endl;
        world_->addEntityToEntities(std::move(constructible));

      }

      else if ((event.type == sf::Event::KeyPressed &&
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

    sf::Time elapsed = clock.restart();
    update(elapsed.asSeconds());

    render();
  }
}
