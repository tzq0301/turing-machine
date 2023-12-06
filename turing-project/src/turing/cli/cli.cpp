#include "turing/cli/cli.h"

#include <vector>

namespace turing::cli {
turing::cli::Option parseArgs(int argc, const char **argv) {
  static const std::string ILLEGAL_ARGS_MESSAGE = "illegal args";
  static const std::string HELP_MESSAGE = "usage: turing [-v|--verbose] [-h|--help] <tm> <input>";

  if (argc == 1) {
    throw std::invalid_argument(ILLEGAL_ARGS_MESSAGE);
  }

  std::vector<std::string> args;
  for (size_t i = 1; i < argc; ++i) {
    args.emplace_back(argv[i]);
  }

  if (args.empty()) {
    throw std::invalid_argument(ILLEGAL_ARGS_MESSAGE);
  }

  if (std::find(args.begin(), args.end(), "-h") != args.end() ||
      std::find(args.begin(), args.end(), "--help") != args.end()) {
    return turing::cli::HelpOption{
        .message = HELP_MESSAGE,
    };
  }

  if (args.size() < 2) {
    throw std::invalid_argument(ILLEGAL_ARGS_MESSAGE);
  }

  turing::cli::RunOption runOption = {
      .verbose = false,
      .tm = args[args.size() - 2],
      .input = args[args.size() - 1],
  };

  if (std::find(args.begin(), args.end(), "-v") != args.end() ||
      std::find(args.begin(), args.end(), "--verbose") != args.end()) {
    runOption.verbose = true;
  }

  return runOption;
}
} // namespace turing::cli