#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace log {
namespace {
bool enable_ = false;

std::string currentDateTime() {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
  return ss.str();
}

template <typename... Args>
void log(std::ostream &out, Args... args) {
  if (!enable_) {
    return;
  }
  out << "[" << currentDateTime() << "] ";
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
