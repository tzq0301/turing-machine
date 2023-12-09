#pragma once

#include <stdexcept>

namespace turing::machine {
class InvalidInputException : public std::runtime_error {
public:
  explicit InvalidInputException(const std::string &msg);
}; 
}