#pragma once

#include <string>
#include <variant>

struct RunOption {
  bool verbose;
  std::string tm;
  std::string input;
};

struct HelpOption {
  std::string message;
};

using Option = std::variant<RunOption, HelpOption>;

Option parseArgs(int argc, const char **argv);