#pragma once

#include <iostream>

namespace turing::machine {
enum class Direction {
  LEFT, RIGHT, STAY
};

std::string to_string(const Direction&);
}