#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>

#include "turing/machine/transition.h"

namespace turing::parser {
struct Result {
  std::unordered_set<std::string> states; // 状态集    Q
  std::unordered_set<char> inputAlphabet; // 输入符号集 S
  std::unordered_set<char> tapeAlphabet;  // 纸带符号集 G
  std::string startState;                        // 初始状态   q0
  char blankSymbol;                       // 空格符号   B
  std::unordered_set<std::string> finalStates;   // 终结状态集  F
  size_t nTape;                                  // 纸带数     N
  std::unordered_map<std::string, turing::machine::Transition> transitions; // 状态函数 delta
};

struct StatesResult {
  std::unordered_set<std::string> states;
};

struct InputAlphabetResult {
  std::unordered_set<char> inputAlphabet;
};

struct TapeAlphabetResult {
  std::unordered_set<char> tapeAlphabet;
};

struct StartStateResult {
  std::string startState;
};

struct FinalStatesResult {
  std::unordered_set<std::string> finalStates;
};

struct BlankSymbolResult {
  char blankSymbol;
};

struct NTapeResult {
  size_t nTape;
};

using NormalStatementResult = std::variant<StatesResult, InputAlphabetResult, TapeAlphabetResult, StartStateResult, FinalStatesResult, BlankSymbolResult, NTapeResult>;

struct TransitionStatementResult {
  turing::machine::Transition transition;
};

struct EmptyStatementResult {};

struct CommentStatementResult {};

using StatementResult = std::variant<NormalStatementResult, TransitionStatementResult, EmptyStatementResult, CommentStatementResult>;
}