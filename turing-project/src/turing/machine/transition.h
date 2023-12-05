#pragma once

#include <string>
#include <vector>

#include "turing/machine/direction.h"

namespace turing::machine {
struct Transition {
  std::string oldState;
  std::vector<char> oldSigns;
  std::vector<char> newSigns;
  std::vector<turing::machine::Direction> directions;
  std::string newState;

  bool operator==(const Transition &other) const {
    return this->oldState == other.oldState &&
           this->oldSigns == other.oldSigns &&
           this->newSigns == other.newSigns &&
           this->directions == other.directions &&
           this->newState == other.newState;
  }
};
} // namespace turing::machine
