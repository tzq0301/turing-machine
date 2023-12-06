#pragma once

#include <chrono>
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace turing::log {
void enable();
bool isEnable();

namespace {
template <typename... Args>
void log(std::ostream &out, Args... args) {
  if (!isEnable()) {
    return;
  }
  (out << ... << args) << std::endl;
}
} // namespace

template <typename... Args>
void info(Args... args) {
  log(std::cout, args...);
}

template <typename... Args>
void error(Args... args) {
  log(std::cerr, args...);
}

} // namespace log
