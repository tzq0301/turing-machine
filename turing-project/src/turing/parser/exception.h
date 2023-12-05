#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include <string_view>

namespace turing::parser {
class InvalidSyntaxException : public std::runtime_error {
public:
  explicit InvalidSyntaxException(const std::string &msg) : std::runtime_error(msg) {}
  
  const char* what() const noexcept override {
    return "syntax error";
  }
};
}
