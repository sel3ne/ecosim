#include "game.h"

#include <gtest/gtest.h>

#include <SFML/Graphics/View.hpp>

TEST(LimitZoomFactorTest, LimitsTooSmallByHeight) {
  sf::View view(/*center=*/{0, 0}, /*size=*/{1000, 100});
  view.zoom(limitZoomFactor(view,
                            /*min_size=*/{10, 10},
                            /*max_size=*/{1000, 1000}, 0.0001));
  EXPECT_EQ(view.getSize(), sf::Vector2f(100, 10));
}

TEST(LimitZoomFactorTest, LimitsTooSmallByWidth) {
  sf::View view(/*center=*/{0, 0}, /*size=*/{100, 1000});
  view.zoom(limitZoomFactor(view,
                            /*min_size=*/{10, 10},
                            /*max_size=*/{1000, 1000}, 0.0001));
  EXPECT_EQ(view.getSize(), sf::Vector2f(10, 100));
}

TEST(LimitZoomFactorTest, LimitsTooBigByWidth) {
  sf::View view(/*center=*/{0, 0}, /*size=*/{500, 50});
  view.zoom(limitZoomFactor(view,
                            /*min_size=*/{10, 10},
                            /*max_size=*/{1000, 1000}, 100.));
  EXPECT_EQ(view.getSize(), sf::Vector2f(1000, 100));
}

TEST(LimitZoomFactorTest, LimitsTooBigByHeight) {
  sf::View view(/*center=*/{0, 0}, /*size=*/{50, 500});
  view.zoom(limitZoomFactor(view,
                            /*min_size=*/{10, 10},
                            /*max_size=*/{1000, 1000}, 100.));
  EXPECT_EQ(view.getSize(), sf::Vector2f(100, 1000));
}
