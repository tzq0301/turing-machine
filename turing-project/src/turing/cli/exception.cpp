#include "turing/cli/exception.h"

#include <stdexcept>

turing::cli::CliException::CliException(const runtime_error& e) : std::runtime_error(e) {}