#include <cassert>

#include "turing/util/number.hpp"

int main() {
  assert(turing::util::number::length(-123) == 3);
  assert(turing::util::number::length(-12) == 2);
  assert(turing::util::number::length(-1) == 1);
  assert(turing::util::number::length(0) == 1);
  assert(turing::util::number::length(1) == 1);
  assert(turing::util::number::length(12) == 2);
  assert(turing::util::number::length(123) == 3);
}