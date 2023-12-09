#pragma once

#include <string>

#include "turing/cli/option.h"

namespace turing::cli {
Option parseArgs(int argc, const char **argv);
void run(const Option &option);
} // namespace turing::cli