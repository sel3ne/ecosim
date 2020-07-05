#include "game.h"

#include <iostream>
#include <random>

#include "building.h"
#include "delivery.h"
#include "farmhouse.h"
#include "grid.h"
#include "human.h"
#include "map_generator.h"
#include "resource_manager.h"
#include "settings.h"
#include "world.h"

Game* gGame = nullptr;

float limitZoomFactor(const sf::View& view, const sf::Vector2f& min_size,
                      const sf::Vector2f& max_size, float zoom) {
  float min_zoom_horizontal = min_size.x / view.getSize().x;
  float min_zoom_vertical = min_size.y / view.getSize().y;
  float min_zoom = std::max(min_zoom_horizontal, min_zoom_vertical);
  float max_zoom_horizontal = max_size.x / view.getSize().x;
  float max_zoom_vertical = max_size.y / view.getSize().y;
  float max_zoom = std::min(max_zoom_horizontal, max_zoom_vertical);
  return std::clamp(zoom, min_zoom, max_zoom);
}

Game::Game(std::unique_ptr<sf::RenderWindow> window)
    : window_(std::move(window)) {
  world_ = std::make_unique<World>();

  Building* house =
      world_->buildConstructible<Building>(3, 3, 3, 3, Entity::HOUSE);
  for (int i = 0; i < 11; i++) {
    float x_coord = house->worldX() + i;
    float y_coord = house->worldY() + i;
    std::unique_ptr<Entity> human =
        std::make_unique<Human>(x_coord, y_coord, 8, 8, Entity::HUMAN);
    Human* human_ptr = dynamic_cast<Human*>(human.get());
    world_->addHappyUnemployedHumans(human_ptr);
    world_->addEntityToEntities(std::move(human));
  }

  Farm* farm = world_->buildConstructible<Farm>(7, 7, 3, 3, Entity::FARM);

  farm->addDeliveryTarget(RESOURCE_FOOD, house);
}

void Game::render() {
  window_->clear();
  world_->render(*window_);
  ui_.render(*window_);
  window_->display();
}

World& Game::returnWorld() { return *world_; }

void Game::update(float time_s) {
  float scroll_distance =
      kScrollSpeed * window_->getView().getSize().y * time_s;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    moveView(0.0f, -scroll_distance);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    moveView(-scroll_distance, 0.f);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    moveView(scroll_distance, 0.f);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    moveView(0.0f, scroll_distance);
  }

  total_time_played_ += time_s;

  world_->update(time_s);
}

void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom) {
  sf::View view = window.getView();
  zoom = limitZoomFactor(view, kMinViewSize, kMaxViewSize, zoom);
  const sf::Vector2f before_coord{window.mapPixelToCoords(pixel)};
  view.zoom(zoom);
  window.setView(view);
  const sf::Vector2f after_coord{window.mapPixelToCoords(pixel)};
  const sf::Vector2f offset_coords = before_coord - after_coord;
  view.move(offset_coords);
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
        // world_->addNumberHappyHuman(1);
        float x_coord = house->worldX() +
                        RandomFloat(-kPixelsPerTile * 2, kPixelsPerTile * 2);
        float y_coord = house->worldY() +
                        RandomFloat(-kPixelsPerTile * 2, kPixelsPerTile * 2);
        std::unique_ptr<Entity> human =
            std::make_unique<Human>(x_coord, y_coord, 8, 8, Entity::HUMAN);
        Human* human_ptr = dynamic_cast<Human*>(human.get());
        world_->addHappyUnemployedHumans(human_ptr);
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
      float x_coord = farmhouse_ptr->worldX() +
                      RandomFloat(-kPixelsPerTile * 2, kPixelsPerTile * 2);
      float y_coord = farmhouse_ptr->worldY() +
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
      } else if (event.type == sf::Event::MouseButtonPressed &&
                 event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i position = sf::Mouse::getPosition(*window_);

        ui_.handleClickEvent(position, *window_);
      } else if (event.type == sf::Event::MouseButtonPressed &&
                 event.mouseButton.button == sf::Mouse::Right) {
        // sf::Vector2i position = sf::Mouse::getPosition(*window_);

        // todo
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
