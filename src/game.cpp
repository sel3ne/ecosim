#include "game.h"

#include <iostream>
#include <random>

#include "building.h"
#include "farmhouse.h"
#include "grid.h"
#include "human.h"
#include "resource_manager.h"
#include "settings.h"

Game* gGame = nullptr;

Game::Game(std::unique_ptr<sf::RenderWindow> window)
    : window_(std::move(window)) {
  world_ = std::make_unique<World>();
  window_->setFramerateLimit(60);
}

void Game::render() {
  window_->clear();
  world_->render(*window_);
  ui_.render(*window_);
  window_->display();
}

World& Game::returnWorld() { return *world_; }

void Game::update(float time_s) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    moveView(0.0f, -4.0f);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    moveView(-4.0f, 0.f);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    moveView(4.0f, 0.f);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    moveView(0.0f, 4.0f);
  }

  total_time_played_ += time_s;

  world_->update(time_s);
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

float RandomFloat(float beg, float end) {
  std::random_device
      rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(beg, end);
  return dis(gen);
}

void Game::moveView(float dx, float dy) {
  sf::View view = window_->getView();
  view.move(dx, dy);
  window_->setView(view);
}

template <typename EntityClass>
EntityClass* Game::buildConstructibleAtMouse(int w_grid, int h_grid,
                                             Entity::EntityType entity_type) {
  // get mouse coordinates
  sf::Vector2i position = sf::Mouse::getPosition(*window_);
  sf::Vector2f world_pos = window_->mapPixelToCoords(position);
  sf::Vector2i grid_pos = worldCoordinateToGrid(world_pos);

  return world_->buildConstructible<EntityClass>(grid_pos.x, grid_pos.y, w_grid,
                                                 h_grid, entity_type);
}

void Game::handleKeyPress(const sf::Event::KeyEvent& key_event) {
  switch (key_event.code) {
    case sf::Keyboard::O: {
      // House event
      Building* house =
          buildConstructibleAtMouse<Building>(3, 3, Entity::HOUSE);
      if (!house) {
        break;
      }
      world_->addNumberHappyHouse(1);

      // add the 10 humans per house
      for (int i = 0; i < kHumansPerHouse; i++) {
        world_->addNumberHappyHuman(1);
        int x_coord = house->worldX() +
                      RandomFloat(-kPixelsPerTile * 2, kPixelsPerTile * 2);
        int y_coord = house->worldY() +
                      RandomFloat(-kPixelsPerTile * 2, kPixelsPerTile * 2);
        std::unique_ptr<Entity> human =
            std::make_unique<Human>(x_coord, y_coord, 8, 8, Entity::HUMAN);
        world_->addEntityToEntities(std::move(human));
      }
      break;
    }
    case sf::Keyboard::P: {
      // Lighthouse event
      Building* lighthouse =
          buildConstructibleAtMouse<Building>(2, 4, Entity::LIGHTHOUSE);
      if (!lighthouse) {
        break;
      }
      world_->addNumberLighthouse(1);

      break;
    }
    case sf::Keyboard::I: {
      // Farmhouse event
      Farmhouse* farmhouse_ptr =
          buildConstructibleAtMouse<Farmhouse>(3, 3, Entity::FARMHOUSE);
      if (!farmhouse_ptr) {
        break;
      }
      world_->addNumberFarmhouse(1);

      // add farmer-dude
      int x_coord = farmhouse_ptr->worldX() +
                    RandomFloat(-kPixelsPerTile * 2, kPixelsPerTile * 2);
      int y_coord = farmhouse_ptr->worldY() +
                    RandomFloat(-kPixelsPerTile * 2, kPixelsPerTile * 2);
      std::unique_ptr<Entity> human =
          std::make_unique<Human>(x_coord, y_coord, 8, 8, Entity::HUMAN,
                                  farmhouse_ptr, 118, Human::FARMER);
      Human* human_with_human_entity = dynamic_cast<Human*>(human.get());
      human_with_human_entity->setEmployer(farmhouse_ptr);

      world_->addEntityToEntities(std::move(human));
      break;
    }
    case sf::Keyboard::F: {
      // Farm event
      Farm* farm = buildConstructibleAtMouse<Farm>(4, 4, Entity::FARM);
      if (!farm) {
        break;
      }
      break;
    }
    case sf::Keyboard::F5: {
      ui_.toggleDebugView();
      break;
    }
    // All other keys do nothing.
    default:;
  }
}

void Game::runMainLoop() {
  sf::Clock clock;
  sf::Event event;
  while (window_->isOpen()) {
    while (window_->pollEvent(event)) {
      if (event.type == sf::Event::KeyPressed) {
        handleKeyPress(event.key);
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
