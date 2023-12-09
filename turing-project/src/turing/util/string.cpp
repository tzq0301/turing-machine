#include "turing/util/string.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

namespace turing::util::string {

std::string ltrim(const std::string &s) {
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s) {
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s) {
  return rtrim(ltrim(s));
}

size_t to_size_t(const std::string number) {
  size_t sizeT;
  std::istringstream iss(number);
  iss >> sizeT;
  if (iss.fail()) {
    return std::numeric_limits<size_t>::max();
  } else {
    return sizeT;
  }
}

std::string times(std::string base, size_t time) {
  std::string s;

  for (size_t i = 0; i < time; ++i) {
    s += base;
  }

  return s;
}

std::string padRight(const std::string &s, const size_t size, const char padChar) {
  std::string result = s;

  for (size_t i = s.size(); i < size; ++i) {
    result += padChar;
  }

  return result;
}

}