#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "turing/cli/cli.h"
#include "turing/cli/option.h"
#include "turing/cli/exception.h"
#include "turing/log/log.hpp"

int main(int argc, const char **argv) {
  turing::cli::Option option;
  
  try {
    option = turing::cli::parseArgs(argc, argv);
  } catch (const std::invalid_argument &e) {
    std::cerr << e.what() << "\n";
    exit(EXIT_FAILURE);
  }

  try {
    turing::cli::run(option);
  } catch (const turing::cli::CliException &e) {
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
