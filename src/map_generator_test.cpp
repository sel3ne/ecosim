#include "map_generator.h"

#include <gtest/gtest.h>

// We wrap tests in a namespace without a name. A namespace without a name
// results in all contained names to become visible only in this file. By doing
// this we can avoid name collisions with other tests in other files that
// accidentally have the same name.
namespace {

float kEpsilon = 1e-6f;
uint64_t kSeed = 1;

TEST(PerlinNoiseTest, TestZeroNearGridPoints) {
  float kEpsilon = 0.00001f;
  for (float x : {-kEpsilon, 0.f, kEpsilon}) {
    for (float y : {-kEpsilon, 0.f, kEpsilon}) {
      EXPECT_NEAR(perlinNoise(kSeed, x, y), 0., 1e-4);
    }
  }
}

TEST(PerlinNoiseTest, TestContinuousAroundPoint) {
  const float kX = 5.4321;
  const float kY = 1.2345;

  float central_value = perlinNoise(kSeed, kX, kY);

  for (float x_offset : {-kEpsilon, 0.f, kEpsilon}) {
    for (float y_offset : {-kEpsilon, 0.f, kEpsilon}) {
      EXPECT_NEAR(perlinNoise(kSeed, kX + x_offset, kY + y_offset),
                  central_value, 1e-4);
    }
  }
}

TEST(PerlinNoiseTest, TestRangeIsCorrect) {
  const int kNumSamples = 1000000;

  std::vector<float> noise_samples(kNumSamples);
  float x = 0.;
  float y = 0.;
  for (int i = 0; i < kNumSamples; ++i) {
    noise_samples[i] = perlinNoise(kSeed, x, y);
    // We sample along a line that should pass through some interesting points.
    // We make the step sizes some weird enough numbers to hopefully hit all
    // kinds of locations within a grid cell. Values will be more extreme
    // towards the middle of grid cells, so let's hope we hit some centers as
    // well.
    x += 0.11;
    y += 0.17;
  }

  // Check that the min and max values generated in this large sample are near,
  // but not outside the boundaries of [-1, 1].
  const auto [min, max] =
      std::minmax_element(noise_samples.begin(), noise_samples.end());
  EXPECT_GT(*min, -1.);
  EXPECT_LT(*min, -.95);
  EXPECT_GT(*max, .95);
  EXPECT_LT(*max, 1.);
}

}  // namespace
