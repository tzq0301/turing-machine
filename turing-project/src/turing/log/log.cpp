#include "turing/log/log.hpp"

bool enable_ = false;

void turing::log::enable() {
  enable_ = true;
}

bool turing::log::isEnable() {
  return enable_;
}