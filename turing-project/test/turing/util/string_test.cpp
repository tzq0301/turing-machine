#include <cassert>
#include <string>

#include "turing/util/string.h"

int main() {
  assert(turing::util::string::padRight("abc", 5) == "abc  ");
  assert(turing::util::string::padRight("abc", 5, '&') == "abc&&");
  assert(turing::util::string::padRight("abc", 2) == "abc");
}