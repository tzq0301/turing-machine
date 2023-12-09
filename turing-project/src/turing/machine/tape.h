#pragma once

#include <cstdlib>
#include <functional>
#include <optional>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "turing/machine/direction.h"
#include "turing/machine/transition.h"

namespace turing::machine {

struct TapeView {
  std::string state;
  std::vector<char> signs;
};

struct TapeRecord {
  int index;
  char sign;
  bool isHead;
};

class Tape {
public:
  Tape(const char blank);
  Tape(const std::string &input, const char blank);

  char currentSign() const;
  void move(const Direction &direction, const char newSign);
  std::optional<std::vector<TapeRecord>> content(bool reserveHead = false);

private:
  std::unordered_map<int, char> tape_;
  int head_;

  const char blank_;
};

class Tapes {
public:
  Tapes(const std::string &input, const std::string &startState, const std::unordered_set<std::string> &finalStates_, const size_t nTape, const char blank);

  void step(const Transition &transition);
  std::string id();
  std::string currentState() const;
  TapeView currentView() const;
  std::optional<std::string> content();
  bool isAccepted();

private:
  std::vector<Tape> tapes_;
  size_t step_;
  std::string currentState_;
  bool accepted_;

  const std::function<std::string(const std::string&)> padLeft_;
  const std::unordered_set<std::string> finalStates_;
};

}