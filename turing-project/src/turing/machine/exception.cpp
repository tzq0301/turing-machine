#include "turing/machine/exception.h"

turing::machine::InvalidInputException::InvalidInputException(const std::string &msg) : std::runtime_error(msg) {}