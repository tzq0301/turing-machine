#include "turing/cli/cli.h"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "turing/cli/exception.h"
#include "turing/cli/option.h"
#include "turing/log/log.hpp"
#include "turing/machine/exception.h"
#include "turing/machine/machine.h"
#include "turing/parser/parser.hpp"

namespace turing::cli {

Option parseArgs(int argc, const char **argv) {
  static const std::string ILLEGAL_ARGS_MESSAGE = "illegal args";
  static const std::string HELP_MESSAGE =
      "usage: turing [-v|--verbose] [-h|--help] <tm> <input>";

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
    return HelpOption{
        .message = HELP_MESSAGE,
    };
  }

  if (args.size() < 2) {
    throw std::invalid_argument(ILLEGAL_ARGS_MESSAGE);
  }

  RunOption runOption = {
      .verbose = false,
  };

  if (std::find(args.begin(), args.end(), "-v") != args.end() ||
      std::find(args.begin(), args.end(), "--verbose") != args.end()) {
    runOption.verbose = true;
    if (args.size() < 3) {
      throw std::invalid_argument(ILLEGAL_ARGS_MESSAGE);
    }
  }

  runOption.tm = args[args.size() - 2];
  runOption.input = args[args.size() - 1];

  return runOption;
}

class CliVisitor {
public:
  void operator()(const HelpOption &option) {
    std::cout << option.message << "\n";
  }

  void operator()(const RunOption &option) {
    if (option.verbose) {
      turing::log::verbose();
    }

    turing::machine::Machine tm = turing::parser::parse(option.tm);

    try {
      tm.run(option.input);
    } catch (const turing::machine::InvalidInputException &e) {
      throw turing::cli::CliException(e);
    }
  }
};

void run(const Option &option) {
  static CliVisitor visitor;
  std::visit(visitor, option);
}

} // namespace turing::cli
