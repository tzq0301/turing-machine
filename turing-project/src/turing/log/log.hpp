#pragma once

#include <chrono>
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace turing::log {
void verbose();
bool isVerbose();

namespace {
template <bool newline = true, typename... Args>
void log(std::ostream &out, Args... args) {
//  (out << ... << args) << std::endl;
  (out << ... << args);
  if constexpr (newline) {
    out << std::endl;   
  }
}
} // namespace

template <bool newline = true, typename... Args>
void info(Args... args) {
  log<newline>(std::cout, args...);
}

template <bool newline = true, typename... Args>
void error(Args... args) {
  log<newline>(std::cerr, args...);
}

} // namespace log
