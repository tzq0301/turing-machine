#pragma once

#include <string>
#include <variant>

#include "turing/cli/option.h"

namespace turing::cli {
Option parseArgs(int argc, const char **argv);
} // namespace turing::cli