#include "turing/log/log.hpp"

bool verbose_ = false;

void turing::log::verbose() {
  verbose_ = true;
}

bool turing::log::isVerbose() {
  return verbose_;
}