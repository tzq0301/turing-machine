#include "cli/cli.h"

#include <vector>

Option parseArgs(int argc, const char **argv) {
  static const std::string ILLEGAL_INPUT_STRING_MESSAGE = "illegal input string";
  static const std::string HELP_MESSAGE = "usage: turing [-v|--verbose] [-h|--help] <tm> <input>";

  if (argc == 1) {
    throw std::invalid_argument(ILLEGAL_INPUT_STRING_MESSAGE);
  }

  std::vector<std::string> args;
  for (size_t i = 1; i < argc; ++i) {
    args.emplace_back(argv[i]);
  }

  if (args.empty()) {
    throw std::invalid_argument(ILLEGAL_INPUT_STRING_MESSAGE);
  }

  if (std::find(args.begin(), args.end(), "-h") != args.end() || std::find(args.begin(), args.end(), "--help") != args.end()) {
    return HelpOption{
        .message = HELP_MESSAGE,
    };
  }

  if (args.size() < 2) {
    throw std::invalid_argument(ILLEGAL_INPUT_STRING_MESSAGE);
  }

  RunOption runOption = {
      .verbose = false,
      .tm = args[args.size() - 2],
      .input = args[args.size() - 1],
  };

  if (std::find(args.begin(), args.end(), "-v") != args.end() || std::find(args.begin(), args.end(), "--verbose") != args.end()) {
    runOption.verbose = true;
  }

  return runOption;
}
