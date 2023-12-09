#include "turing/machine/machine.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "turing/log/log.hpp"
#include "turing/machine/direction.h"
#include "turing/machine/exception.h"
#include "turing/machine/tape.h"
#include "turing/machine/transition.h"
#include "turing/util/string.h"

namespace turing::machine {

Machine::Machine(
    std::unordered_set<std::string> states,
    std::unordered_set<char> inputAlphabet,
    std::unordered_set<char> tapeAlphabet, std::string startState,
    char blankSymbol, std::unordered_set<std::string> finalStates, size_t nTape,
    std::unordered_map<std::string, std::vector<Transition>> transitions)
    : states_(states), inputAlphabet_(inputAlphabet),
      tapeAlphabet_(tapeAlphabet), startState_(startState),
      blankSymbol_(blankSymbol), finalStates_(finalStates), nTape_(nTape),
      transitions_(transitions) {}

void Machine::run(const std::string &input) {
  if (turing::log::isVerbose()) {
    turing::log::info("Input: ", input);
  }

  auto validResult = this->isInputValid(input);
  if (std::holds_alternative<size_t>(validResult)) {
    if (turing::log::isVerbose()) {
      size_t index = std::get<size_t>(validResult);
      turing::log::error("==================== ERR ====================");
      turing::log::error(
          "error: Symbol \"", input[index],
          "\" in input is not defined in the set of input symbols");
      turing::log::error("Input: ", input);
      turing::log::error("       ",
                         turing::util::string::times(
                             std::string{turing::util::string::SPACE}, index),
                         std::string{turing::util::string::UPARROW});
      turing::log::error("==================== END ====================");
    } else {
      turing::log::error("illegal input string");
    }
    throw InvalidInputException(input);
  }

  assert(std::holds_alternative<bool>(validResult));
  assert(std::get<bool>(validResult));

  if (turing::log::isVerbose()) {
    turing::log::info("==================== RUN ====================");
  }

  Tapes tapes = Tapes{input, startState_, finalStates_, nTape_, blankSymbol_};
  bool accepted = finalStates_.contains(tapes.currentState());

  if (turing::log::isVerbose()) {
    turing::log::info<false>(tapes.id());
  }

  for (std::optional<Transition> transition =
           determineTransition(tapes.currentView());
       transition != std::nullopt;
       transition = determineTransition(tapes.currentView())) {
    tapes.step(transition.value());

    if (turing::log::isVerbose()) {
      turing::log::info<false>(tapes.id());
    }
  }

  if (turing::log::isVerbose()) {
    if (tapes.isAccepted()) {
      turing::log::info("ACCEPTED");
    } else {
      turing::log::info("UNACCEPTED");
    }
    auto content = tapes.content();
    if (content.has_value()) {
      turing::log::info("Result:", " ", tapes.content().value());
    }
    turing::log::info("==================== END ====================");
  } else {
    if (tapes.isAccepted()) {
      turing::log::info("(ACCEPTED)", " ", tapes.content().value_or(""));
    } else {
      turing::log::info("(UNACCEPTED)", " ", tapes.content().value_or(""));
    }
  }
}

std::variant<bool, size_t> Machine::isInputValid(const std::string &input) {
  auto it =
      std::find_if_not(input.begin(), input.end(), [this](char ch) -> bool {
        return inputAlphabet_.contains(ch);
      });
  if (it == input.end()) {
    return true;
  } else {
    return static_cast<size_t>(std::distance(input.begin(), it));
  }
}

std::string Machine::to_string() {
  std::string s;

  static std::function<std::string(std::unordered_set<std::string>)>
      joinStrSet = [](std::unordered_set<std::string> set) -> std::string {
    std::string s;

    for (const auto &item : set) {
      s += item + ",";
    }

    return s.substr(0, s.size() - 1);
  };

  static std::function<std::string(std::unordered_set<char>)> joinCharSet =
      [](std::unordered_set<char> set) -> std::string {
    std::string s;

    for (const auto &item : set) {
      s += std::string{item} + ",";
    }

    return s.substr(0, s.size() - 1);
  };

  s += "#Q = {" + joinStrSet(states_) + "}\n";
  s += "#S = {" + joinCharSet(inputAlphabet_) + "}\n";
  s += "#G = {" + joinCharSet(tapeAlphabet_) + "}\n";
  s += "#q0 = " + startState_ + "\n";
  s += "#B = " + std::string{blankSymbol_} + "\n";
  s += "#F = {" + joinStrSet(finalStates_) + "}\n";
  s += "#N = " + std::to_string(nTape_) + "\n";

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

std::optional<Transition> Machine::determineTransition(const TapeView &view) {
  if (!transitions_.contains(view.state)) {
    return std::nullopt; // halt
  }

  std::vector<Transition> &transitions = transitions_[view.state];

  auto it = std::find_if(transitions.begin(), transitions.end(),
                         [&view](const Transition &transition) -> bool {
                           return view.signs == transition.oldSigns;
                         });
  if (it != transitions.end()) {
    return *it;
  }

  std::vector<Transition> candidates = transitions;

  if (!transitions.empty()) {
    assert(view.signs.size() == transitions[0].oldSigns.size());
  }

  // handle '*' pattern
  for (size_t i = 0; i < view.signs.size(); ++i) {
    char sign = view.signs[i];
    // FIXME !
    std::erase_if(candidates, [sign, i](const Transition &transition) -> bool {
      char tSign = transition.oldSigns[i];
      return sign != tSign && turing::util::string::STAR != tSign;
    });
  }

  if (candidates.empty()) {
    return std::nullopt;
  }

  return candidates[0]; // assume only contains candidate transition
}

} // namespace turing::machine
