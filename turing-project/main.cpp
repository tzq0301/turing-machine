#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "tm/cli/cli.h"
#include "tm/log/log.hpp"

int main(int argc, const char **argv) {
  turing::cli::Option option;

  try {
    option = turing::cli::parseArgs(argc, argv);
  } catch (const std::invalid_argument &e) {
    std::cerr << e.what() << "\n";
    exit(EXIT_FAILURE);
  }

  if (std::holds_alternative<turing::cli::HelpOption>(option)) {
    std::cout << std::get<turing::cli::HelpOption>(option).message << "\n";
    exit(EXIT_SUCCESS);
  }

  assert(std::holds_alternative<turing::cli::RunOption>(option));

  turing::cli::RunOption runOption = std::get<turing::cli::RunOption>(option);
  if (runOption.verbose) {
    turing::log::enable();
  }

  // TODO

  exit(EXIT_SUCCESS);
}
