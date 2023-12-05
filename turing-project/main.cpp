#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "log/log.hpp"

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

int main(int argc, const char **argv) {
  Option option;

  try {
    option = parseArgs(argc, argv);
  } catch (const std::invalid_argument &e) {
    exit(EXIT_FAILURE);
  }

  if (std::holds_alternative<HelpOption>(option)) {
    std::cout << std::get<HelpOption>(option).message << "\n";
    exit(EXIT_SUCCESS);
  }

  assert(std::holds_alternative<RunOption>(option));

  RunOption runOption = std::get<RunOption>(option);
  if (runOption.verbose) {
    log::enable();
  }

  log::info("tm:    " + runOption.tm);
  log::info("input: " + runOption.input);

  exit(EXIT_SUCCESS);
}

Option parseArgs(int argc, const char **argv) {
  std::vector<std::string> args;
  for (size_t i = 1; i < argc; ++i) {
    args.emplace_back(argv[i]);
  }

  if (args.empty()) {
    throw std::invalid_argument("need parse arguments");
  }

  if (std::find(args.begin(), args.end(), "-h") != args.end() || std::find(args.begin(), args.end(), "--help") != args.end()) {
    return HelpOption{
      .message = "usage: turing [-v|--verbose] [-h|--help] <tm> <input>",
    };
  }

  if (args.size() < 2) {
    throw std::invalid_argument("invalid arguments");
  }

  RunOption runOption = {
    .verbose = false,
    .tm = args[args.size() - 2],
    .input = args[args.size() - 1],
  };

  if (std::find(args.begin(), args.end(), "-v") != args.end() || std::find(args.begin(), args.end(), "--verbose") != args.end()) {
    runOption.verbose = true;
  }

  // TODO

  return runOption;
}
