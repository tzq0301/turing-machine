#include "turing/machine/machine.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "turing/log/log.hpp"
#include "turing/machine/direction.h"
#include "turing/machine/exception.h"
#include "turing/machine/transition.h"
#include "turing/util/string.h"

turing::machine::Machine::Machine(
    std::unordered_set<std::string> states,
    std::unordered_set<char> inputAlphabet,
    std::unordered_set<char> tapeAlphabet,
    std::string startState,
    char blankSymbol,
    std::unordered_set<std::string> finalStates,
    size_t nTape,
    std::unordered_map<std::string, std::vector<turing::machine::Transition>> transitions)
    : states_(states), inputAlphabet_(inputAlphabet),
      tapeAlphabet_(tapeAlphabet), startState_(startState),
      blankSymbol_(blankSymbol), finalStates_(finalStates),
      nTape_(nTape), transitions_(transitions) {}

void turing::machine::Machine::run(const std::string &input) {
  turing::log::info("Input: ", input);

  auto validResult = this->isInputValid(input);
  if (std::holds_alternative<size_t>(validResult)) {
    size_t index = std::get<size_t>(validResult);
    turing::log::error("==================== ERR ====================");
    turing::log::error("error: Symbol \"", input[index], "\" in input is not defined in the set of input symbols");
    turing::log::error("Input: ", input);
    turing::log::error("       ", turing::util::string::times(std::string{turing::util::string::SPACE}, index), std::string{turing::util::string::UPARROW});
    turing::log::error("==================== END ====================");
    throw turing::machine::InvalidInputException(input);
  }

  assert(std::holds_alternative<bool>(validResult));
  assert(std::get<bool>(validResult));
  turing::log::info("==================== RUN ====================");

}

std::variant<bool, size_t> turing::machine::Machine::isInputValid(const std::string &input) {
  auto it = std::find_if_not(input.begin(), input.end(), [this](char ch) -> bool {
    return this->inputAlphabet_.contains(ch);
  });
  if (it == input.end()) {
    return true;
  } else {
    return static_cast<size_t>(std::distance(input.begin(), it));
  }
}

std::string turing::machine::Machine::to_string() {
  std::string s;
  
  static std::function<std::string(std::unordered_set<std::string>)> joinStrSet = [](std::unordered_set<std::string> set) -> std::string {
    std::string s;
    
    for (const auto &item : set) {
      s += item + ",";
    }
    
    return s.substr(0, s.size() - 1);
  };
  
  static std::function<std::string(std::unordered_set<char>)> joinCharSet = [](std::unordered_set<char> set) -> std::string {
    std::string s;
  
    for (const auto &item : set) {
      s += std::string{item} + ",";
    }
  
    return s.substr(0, s.size() - 1);
  };
  
  s += "#Q = {" + joinStrSet(this->states_) + "}\n";
  s += "#S = {" + joinCharSet(this->inputAlphabet_) + "}\n";
  s += "#G = {" + joinCharSet(this->tapeAlphabet_) + "}\n";
  s += "#q0 = " + this->startState_ + "\n";
  s += "#B = " + std::string{this->blankSymbol_} + "\n";
  s += "#F = {" + joinStrSet(this->finalStates_) + "}\n";
  s += "#N = " + std::to_string(this->nTape_) + "\n";
  
  for (const auto &[_, subTransition] : this->transitions_) {
    for (const auto &transition : subTransition) {
      s += transition.oldState + " ";
      for (const auto &oldSign : transition.oldSigns) {
        s += oldSign;
      }
      s += " ";
      for (const auto &newSign : transition.newSigns) {
        s += newSign;
      }
      s += " ";
      for (const auto &direction : transition.directions) {
        s += turing::machine::to_string(direction);
      }
      s += " ";
      s += transition.newState + "\n";
    }
  }
  
  return s;
}