#include "turing/machine/direction.h"

#include <iostream>

std::string turing::machine::to_string(const turing::machine::Direction& direction) {
  using namespace turing::machine;
  switch (direction) {
  case Direction::LEFT:
    return "l";
  case Direction::RIGHT:
    return "r";
  default:
    return "*";
  }
}