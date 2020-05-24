#include "human.h"

#include <iostream>

#include "game.h"
#include "resource_manager.h"

float Human::worldX() { return x_world_; }

float Human::worldY() { return y_world_; }

float Human::worldW() { return w_world_; }

float Human::worldH() { return h_world_; }

void Human::render(sf::RenderWindow& window) {
  TextureId texID;
  if (happiness_) {
    texID = TEXTURE_HUMAN_HAPPY;
  } else if (!happiness_) {
    texID = TEXTURE_HUMAN_UNHAPPY;
  }
  sf::Texture* texture = gResourceManager->getTexture(texID);
  sf::Vector2u texSize = texture->getSize();
  sf::Sprite sprite;
  sprite.setTexture(*texture);
  sprite.setPosition(worldX(), worldY());
  sprite.setScale(worldW() / texSize.x, worldH() / texSize.y);
  window.draw(sprite);
}

void Human::update(float time_s) {
  World& world = gGame->returnWorld();
  int unhappyHuman = world.returnNumberUnhappyHuman();
  int unhappyHouse = world.returnNumberUnhappyHouse();
  if (unhappyHuman < unhappyHouse * 10 && happiness_ == 1) {
    happiness_ = 0;
    world.addNumberUnhappyHuman(1);
    world.addNumberHappyHuman(-1);
  } else if (unhappyHuman > unhappyHouse * 10 && happiness_ == 0) {
    happiness_ = 1;
    world.addNumberUnhappyHuman(-1);
    world.addNumberHappyHuman(1);
  } else if (unhappyHuman * 10 == unhappyHouse) {
    // std::cout << "balanced humans" << std::endl;
  }
}
