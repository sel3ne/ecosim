#include "random.h"

#include <gtest/gtest.h>

// We wrap tests in a namespace without a name. A namespace without a name
// results in all contained names to become visible only in this file. By doing
// this we can avoid name collisions with other tests in other files that
// accidentally have the same name.
namespace {

TEST(XorShiftRandomGeneratorTest, ProducesExpectedIntValuesSeed0) {
  XorShiftRandomGenerator gen(/*seed=*/0);
  EXPECT_EQ(gen.getInt(), 17954947803125907456ull);
  EXPECT_EQ(gen.getInt(), 10373061909235543779ull);
  EXPECT_EQ(gen.getInt(), 3777559059212774342ull);
  EXPECT_EQ(gen.getInt(), 14219777844791603904ull);
  EXPECT_EQ(gen.getInt(), 7794997853504412167ull);
}

TEST(XorShiftRandomGeneratorTest, ProducesExpectedIntValuesSeed1) {
  XorShiftRandomGenerator gen(/*seed=*/1);
  EXPECT_EQ(gen.getInt(), 5180492295206395165ull);
  EXPECT_EQ(gen.getInt(), 12380297144915551517ull);
  EXPECT_EQ(gen.getInt(), 13389498078930870103ull);
  EXPECT_EQ(gen.getInt(), 5599127315341312413ull);
  EXPECT_EQ(gen.getInt(), 1036278371763004928ull);
}

TEST(XorShiftRandomGeneratorTest, ProducesExpectedFloatValues) {
  XorShiftRandomGenerator gen(/*seed=*/1);
  EXPECT_FLOAT_EQ(gen.getFloat(/*min=*/1.f, /*max=*/5.f), 2.1233401);
  EXPECT_FLOAT_EQ(gen.getFloat(/*min=*/1.f, /*max=*/5.f), 3.6845491);
  EXPECT_FLOAT_EQ(gen.getFloat(/*min=*/1.f, /*max=*/5.f), 3.9033847);
  EXPECT_FLOAT_EQ(gen.getFloat(/*min=*/1.f, /*max=*/5.f), 2.2141171);
  EXPECT_FLOAT_EQ(gen.getFloat(/*min=*/1.f, /*max=*/5.f), 1.2247070);
}

}  // namespace
