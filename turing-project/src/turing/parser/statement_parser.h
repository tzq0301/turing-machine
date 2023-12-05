#pragma once

#include <string>
#include <unordered_set>

#include "turing/parser/result.h"

namespace turing::parser {
class StatementParser {
public:
  explicit StatementParser(std::string statement);
  StatementResult parse();

private:
  const std::string statement_;
  size_t index_;

  StatesResult parseStates();
  InputAlphabetResult parseInputAlphabet();
  TapeAlphabetResult parseTapeAlphabet();
  StartStateResult parseStartState();
  FinalStatesResult parseFinalStates();
  BlankSymbolResult parseBlankSymbol();
  NTapeResult parseNTape();
  NormalStatementResult parseNormalStatement();
  TransitionStatementResult parseTransitionStatement();
  std::string mustNextToken();
  char peekNextChar() const;
  char mustNextChar();
  void mustSkipSpace();
  void mustSkipComma();
  void mustSkipEqualSign();
  void mustSkipLeftBracket();
  void mustSkipRightBracket();
  bool isEmptyLine() const;
  bool isComment() const;
  bool reachEnd() const;
};

StatementResult parseStatement(const std::string &statement);

} // namespace turing::parser
