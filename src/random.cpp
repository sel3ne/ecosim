#include "random.h"

XorShiftRandomGenerator::XorShiftRandomGenerator(uint64_t seed)
    : state_(seed ? seed : UINT64_MAX)  // State 0 is illegal, so just avoid it.
{}

void XorShiftRandomGenerator::mixIntoState(uint64_t value) {
  state_ ^= value;
  getInt();
}

uint64_t XorShiftRandomGenerator::getInt() {
  state_ ^= state_ >> 12;
  state_ ^= state_ << 25;
  state_ ^= state_ >> 27;
  return state_ * 0x2545F4914F6CDD1Dull;
}

float XorShiftRandomGenerator::getFloat(float min, float max) {
  uint64_t i = getInt();
  return i * (max - min) / static_cast<float>(UINT64_MAX) + min;
}
