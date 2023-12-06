#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <exception>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "turing/cli/cli.h"
#include "turing/log/log.hpp"
#include "turing/machine/exception.h"
#include "turing/machine/machine.h"
#include "turing/parser/parser.hpp"

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

  turing::machine::Machine tm = turing::parser::parse(runOption.tm);
  
//  std::cout << std::boolalpha << std::get<bool>(tm.isInputValid("1001001")) << "\n";

  // std::cout << tm.to_string();
  
  try {
    tm.run(runOption.input);
  } catch (const turing::machine::InvalidInputException &e) {
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
