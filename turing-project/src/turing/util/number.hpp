#pragma once

#include <cmath>
#include <cstddef>

namespace turing::util::number {

constexpr size_t length(const int num) {
  return num == 0 ? 1 : (num < 0 ? static_cast<size_t>(log10(std::abs(num))) + 1 : static_cast<size_t>(log10(num)) + 1);
}

}
