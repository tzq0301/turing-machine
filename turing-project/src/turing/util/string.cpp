#include "turing/util/string.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

std::string turing::util::string::ltrim(const std::string &s) {
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

std::string turing::util::string::rtrim(const std::string &s) {
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string turing::util::string::trim(const std::string &s) {
  return rtrim(ltrim(s));
}

size_t turing::util::string::to_size_t(const std::string number) {
  size_t sizeT;
  std::istringstream iss(number);
  iss >> sizeT;
  if (iss.fail()) {
    return std::numeric_limits<size_t>::max();
  } else {
    return sizeT;
  }
}