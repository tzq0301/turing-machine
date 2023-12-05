#pragma once

#include <string>
#include <variant>

namespace turing::cli {
struct RunOption {
  bool verbose;
  std::string tm;
  std::string input;
};

struct HelpOption {
  std::string message;
};

using Option = std::variant<RunOption, HelpOption>;
} // namespace turing::cli