#pragma once

#include <stdexcept>

namespace turing::cli {
class CliException : public std::runtime_error {
public:
  CliException(const runtime_error&);
};
}