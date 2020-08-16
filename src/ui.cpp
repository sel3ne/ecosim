#include "ui.h"

#include <iomanip>
#include <sstream>

#include "building.h"
#include "constructible.h"
#include "game.h"
#include "grid.h"
#include "human.h"
#include "resource_manager.h"
#include "settings.h"
#include "vector_util.h"
#include "world.h"

void UI::renderDebugView(sf::RenderWindow& window, const sf::View& world_view) {
  if (!debug_view_enabled_) {
    return;
  }
  sf::Vector2i mouse_pos_screen = sf::Mouse::getPosition(window);
  sf::Vector2f mouse_pos_world =
      window.mapPixelToCoords(mouse_pos_screen, world_view);
  sf::Vector2i mouse_pos_grid = worldCoordinateToGrid(mouse_pos_world);

  std::ostringstream oss;
  oss << "Mouse pos: " << mouse_pos_screen << " (Window) / " << mouse_pos_world
      << " (World) / " << mouse_pos_grid << " (Grid)\n";
  oss << "View Size: " << world_view.getSize();
  sf::Text mouse_pos_text(oss.str(), *gResourceManager->getFont(FONT_COURIER),
                          20);
  mouse_pos_text.setPosition(0, kTopBarHeight);
  mouse_pos_text.setStyle(sf::Text::Bold);

  window.draw(mouse_pos_text);
}

void UI::renderFoodStatus(sf::RenderWindow& window) {
  int base_x = window.getSize().x - kFoodStatusX;

  // Icon.
  sf::Sprite grain_icon;
  sf::Texture* texture = gResourceManager->getTexture(TEXTURE_GRAIN);
  grain_icon.setTexture(*texture);
  grain_icon.setPosition(base_x, 0);
  float icon_scale = kTopBarHeight / texture->getSize().y;
  grain_icon.setScale(icon_scale, icon_scale);
  window.draw(grain_icon);

  // Numbers
  int num_happy = gGame->returnWorld().returnNumberHappyHuman();
  int num_total = num_happy + gGame->returnWorld().returnNumberUnhappyHuman();
  std::ostringstream food_oss;
  food_oss << num_happy << "/" << num_total;
  sf::Text food_text(food_oss.str(), *gResourceManager->getFont(FONT_COURIER),
                     kTopBarTextSize);
  food_text.setPosition(base_x + kTopBarHeight, 0);
  food_text.setStyle(sf::Text::Bold);
  if (num_happy == num_total) {
    food_text.setFillColor({0, 0, 0});
  } else {
    food_text.setFillColor({210, 0, 0});
  }
  window.draw(food_text);
}

void UI::renderTopBar(sf::RenderWindow& window) {
  // Top bar background.
  sf::RectangleShape background;
  background.setPosition(0, 0);
  background.setSize(
      sf::Vector2f(static_cast<float>(window.getSize().x), kTopBarHeight));
  background.setTexture(gResourceManager->getTexture(TEXTURE_STONE_BAR));
  background.setOutlineThickness(2.f);
  background.setOutlineColor(sf::Color(50, 50, 50));
  window.draw(background);

  // Game time display.
  std::ostringstream time_oss;
  time_oss << static_cast<int>(gGame->returnWorld().totalTimePlayed());
  time_oss << "(" << std::fixed << std::setprecision(1)
           << gGame->gameSpeedMultipler() << ")";
  sf::Text time_played_text(time_oss.str(),
                            *gResourceManager->getFont(FONT_COURIER),
                            kTopBarTextSize);
  time_played_text.setPosition(
      window.getSize().x - time_played_text.getLocalBounds().width - 20, 0);
  time_played_text.setStyle(sf::Text::Bold);
  time_played_text.setFillColor({0, 0, 0});
  window.draw(time_played_text);

  // Food status.
  renderFoodStatus(window);
}

void UI::renderEntityInfo(sf::RenderWindow& window) {
  // draw background
  sf::RectangleShape background;
  background.setPosition(0, window.getSize().y - kEntityBarHeight);
  background.setSize(
      sf::Vector2f(static_cast<float>(window.getSize().x), kEntityBarHeight));
  background.setTexture(gResourceManager->getTexture(TEXTURE_STONE_BAR));
  background.setOutlineThickness(2.f);
  background.setOutlineColor(sf::Color(50, 50, 50));
  window.draw(background);

  // Draw first Entity Info
  if (clicked_entity_) {
    std::string text_string;
    if (clicked_entity_->typeOfEntity() == Entity::HUMAN) {
      Human* human_ptr = dynamic_cast<Human*>(clicked_entity_);
      text_string = human_ptr->printJobAndResource();
    }

    if (clicked_entity_->isBuilding()) {
      Building* building_ptr = dynamic_cast<Building*>(clicked_entity_);
      text_string = building_ptr->printNameAndResource();
    }

    sf::Text text(text_string, *gResourceManager->getFont(FONT_COURIER),
                  kTopBarTextSize);
    text.setPosition(5, window.getSize().y - kEntityBarHeight + 5);
    text.setStyle(sf::Text::Bold);
    text.setFillColor({0, 0, 0});
    text.setCharacterSize(15);
    window.draw(text);
  }
}

void UI::renderTooltip(sf::RenderWindow& window) {}

void UI::renderArrowBuilding(sf::RenderWindow& window, Building* building) {
  building->renderArrows(window);
}

void UI::renderVisualizeClickedEntity(sf::RenderWindow& window) {
  clicked_entity_->visualizeClickedEntity(window);
}

void UI::render(sf::RenderWindow& window) {
  if (ghost_arrow_ != std::pair<Building*, Building*>{nullptr, nullptr}) {
    // draw ghost arrow
    sf::Vector2f end = ghost_arrow_.second->worldPos();
    end = end + ghost_arrow_.second->worldSize() / 2.f;

    sf::Vector2f start = ghost_arrow_.first->worldPos();
    start = start + ghost_arrow_.first->worldSize() / 2.f;
    ghost_arrow_.first->drawArrow(start, end,
                                  /*thickness=*/2, sf::Color(0, 0, 100, 100),
                                  window);
  }
  if (clicked_entity_) {
    renderVisualizeClickedEntity(window);
    if (clicked_entity_->isBuilding()) {
      Building* clicked_building_ptr = dynamic_cast<Building*>(clicked_entity_);
      renderArrowBuilding(window, clicked_building_ptr);
    }

    // ToDo draw arrwos for humans
  }

  sf::View saved_view = window.getView();
  window.setView(window.getDefaultView());

  renderTopBar(window);
  renderEntityInfo(window);

  renderDebugView(window, saved_view);

  window.setView(saved_view);
}

void UI::handleClickEvent(sf::Vector2i window_mouse_position,
                          sf::RenderWindow& window) {
  ghost_arrow_ = {nullptr, nullptr};
  World& world = gGame->returnWorld();
  sf::Vector2f world_pos = window.mapPixelToCoords(window_mouse_position);
  Human* clicked_human = nullptr;
  std::function<void(Human&)> find_clicked_human =
      [world_pos, &clicked_human](Human& hum) {
        if (!clicked_human) {
          sf::Rect<float> rect_human = hum.worldRect();
          if (rect_human.contains(world_pos)) {
            clicked_human = &hum;
          }
        }
      };
  world.doForAllHumans(find_clicked_human);
  if (clicked_human) {
    clicked_entity_ = clicked_human;
    return;
  }

  if (!clicked_human) {
    // sf::Vector2i grid_pos = worldCoordinateToGrid(world_pos);
    Constructible* clicked_constructible = nullptr;
    std::function<void(Constructible&)> find_clicked_constructible =
        [world_pos, &clicked_constructible](Constructible& construct) {
          if (!clicked_constructible) {
            sf::Rect<float> rect_constructible = construct.worldRect();
            if (rect_constructible.contains(world_pos.x, world_pos.y)) {
              clicked_constructible = &construct;
            }
          }
        };
    world.doForAllConstructibles(find_clicked_constructible);
    clicked_entity_ = clicked_constructible;
  }
}

void UI::setGhostArrow(sf::Vector2i window_mouse_position,
                       sf::RenderWindow& window) {
  // check if there is already an entity clicked
  if (!clicked_entity_) {
    return;
  }
  // check if clicked_entity is anything but a building
  if (!clicked_entity_->isBuilding()) {
    return;
  }
  // not possible to make arrows from farm
  if (clicked_entity_->typeOfEntity() == Entity::FARM) {
    return;
  }

  World& world = gGame->returnWorld();
  sf::Vector2f world_pos = window.mapPixelToCoords(window_mouse_position);

  Constructible* clicked_constructible = nullptr;
  std::function<void(Constructible&)> find_clicked_constructible =
      [world_pos, &clicked_constructible](Constructible& construct) {
        if (!clicked_constructible) {
          sf::Rect<float> rect_constructible = construct.worldRect();
          if (rect_constructible.contains(world_pos.x, world_pos.y)) {
            clicked_constructible = &construct;
          }
        }
      };
  world.doForAllConstructibles(find_clicked_constructible);
  // check if nothing is clicked
  if (!clicked_constructible) {
    return;
  }
  Entity* right_clicked_entity = clicked_constructible;
  // check if righht_clicked_entity is anything but a building
  if (!right_clicked_entity->isBuilding()) {
    return;
  }
  // not possible to make arrows to farm
  if (right_clicked_entity->typeOfEntity() == Entity::FARM) {
    return;
  }

  // save arrows
  Building* left_clicked_building = dynamic_cast<Building*>(clicked_entity_);
  Building* right_clicked_building =
      dynamic_cast<Building*>(right_clicked_entity);

  // no arrows between the same building
  if (right_clicked_building == left_clicked_building) {
    return;
  }

  ghost_arrow_ = {left_clicked_building, right_clicked_building};
}

void UI::toggleArrow(sf::Vector2i window_mouse_position,
                     sf::RenderWindow& window, ResourceId resource) {
  if (ghost_arrow_ == std::pair<Building*, Building*>{nullptr, nullptr}) {
    return;
  }
  ghost_arrow_.first->toggleDeliveryTarget(resource, ghost_arrow_.second);
}
