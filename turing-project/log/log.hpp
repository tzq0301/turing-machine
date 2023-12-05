#include <chrono>
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace log {
namespace {
bool enable_ = false;

enum class Level {
  INFO, ERROR
};

std::string to_string(Level level) {
  switch (level) {
  case Level::INFO:
    return "INFO ";
  case Level::ERROR:
    return "ERROR";
  }
  throw std::invalid_argument("level should be one of INFO, ERROR");
}

std::string currentDateTime() {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
  return ss.str();
}

template <typename... Args>
void log(std::ostream &out, Level level, Args... args) {
  if (!enable_) {
    return;
  }
  out << "[" << to_string(level) << "] [" << currentDateTime() << "] ";
  (out << ... << args) << std::endl;
}
} // namespace

void enable() {
  enable_ = true;
}

template <typename... Args>
void info(Args... args) {
  log(std::cout, Level::INFO, args...);
}

template <typename... Args>
void error(Args... args) {
  log(std::cerr, Level::ERROR, args...);
}

} // namespace log
