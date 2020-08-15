#ifndef ECOSIM_RANDOM_H
#define ECOSIM_RANDOM_H

#include <cstdint>

// This implements a pseudo random number generator (PRNG). From a starting
// seed it produces numbers that are deterministic and look random. We use it to
// allow reproducing the same sequence of randomness, e.g. for map generation,
// multiple times by just remembering the seed.
// The used algorithm is called xorshift*, see
// https://en.wikipedia.org/wiki/Xorshift#xorshift*
// The main properties are that it has short code and is very fast to execute
// compared to more random alternatives.
//
// Example usage:
// XorShiftRandomGenerator gen(/*seed=*/0);
// int random_number = gen.getInt();
// int random_number2 = gen.getInt();
class XorShiftRandomGenerator {
 public:
  XorShiftRandomGenerator(uint64_t seed);

  void mixIntoState(uint64_t value);

  uint64_t getInt();
  float getFloat(float min, float max);

 private:
  uint64_t state_;
};

// Give XorShiftRandomGenerator a more descriptive name for the usage side. Also
// this makes it easier to change the implementation in case we want another
// algorithm. If we use `XorShiftRandomGenerator` directly, then it becomes more
// work to change all the usages.
using DeterministicRandomGenerator = XorShiftRandomGenerator;

#endif  // define ECOSIM_RANDOM_H
