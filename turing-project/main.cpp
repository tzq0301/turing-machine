#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "cli/cli.h"
#include "log/log.hpp"

int main(int argc, const char **argv) {
  Option option;

  try {
    option = parseArgs(argc, argv);
  } catch (const std::invalid_argument &e) {
    std::cerr << e.what() << "\n";
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

  // TODO

  exit(EXIT_SUCCESS);
}
