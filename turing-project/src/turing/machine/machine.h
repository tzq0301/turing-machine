#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "turing/machine/transition.h"

namespace turing::machine {
class Machine {
public:
  Machine(
      std::unordered_set<std::string> states,
      std::unordered_set<char> inputAlphabet,
      std::unordered_set<char> tapeAlphabet, std::string startState,
      char blankSymbol, std::unordered_set<std::string> finalStates,
      size_t nTape,
      std::unordered_map<std::string, std::vector<turing::machine::Transition>> transitions);

  std::string to_string();

private:
  std::unordered_set<std::string> states_;      // 状态集    Q
  std::unordered_set<char> inputAlphabet_;      // 输入符号集 S
  std::unordered_set<char> tapeAlphabet_;       // 纸带符号集 G
  std::string startState_;                      // 初始状态   q0
  char blankSymbol_;                            // 空格符号   B
  std::unordered_set<std::string> finalStates_; // 终结状态集  F
  size_t nTape_;                                // 纸带数     N
  std::unordered_map<std::string, std::vector<turing::machine::Transition>> transitions_; // 状态函数 delta
};
} // namespace turing::machine
