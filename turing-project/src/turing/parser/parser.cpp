#include "turing/parser/parser.hpp"
#include "turing/parser/result.h"

namespace turing::parser {

std::optional<std::string> Parser::nextStatement() {
  if (index_ == statements_.size()) {
    return std::nullopt;
  }

  return statements_[index_++];
}

turing::machine::Machine parse(const std::string &filepath) {
  return Parser{filepath}.parse();
}

}