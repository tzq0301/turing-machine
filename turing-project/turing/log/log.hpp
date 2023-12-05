#include <chrono>
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace turing::log {
namespace {
bool enable_ = false;

template <typename... Args>
void log(std::ostream &out, Args... args) {
  if (!enable_) {
    return;
  }
  (out << ... << args) << std::endl;
}
} // namespace

void enable() {
  enable_ = true;
}

template <typename... Args>
void info(Args... args) {
  log(std::cout, args...);
}

template <typename... Args>
void error(Args... args) {
  log(std::cerr, args...);
}

} // namespace log
