#include "turing/machine/tape.h"

#include <cassert>
#include <cmath>
#include <exception>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <unordered_map>

#include "turing/util/number.hpp"
#include "turing/util/string.h"

namespace turing::machine {

Tape::Tape(const char blank)
    : tape_(std::unordered_map<int, char>()), head_(0), blank_(blank) {
  tape_[head_] = blank;
}

Tape::Tape(const std::string &input, char blank)
    : tape_(std::unordered_map<int, char>()), head_(0), blank_(blank) {
  for (size_t i = 0; i < input.size(); ++i) {
    tape_[i] = input[i];
  }
}

char Tape::currentSign() const { return tape_.at(head_); }

void Tape::move(const Direction &direction, const char newSign) {
  if (newSign != turing::util::string::STAR) {
    tape_[head_] = newSign;
  }

  switch (direction) {
  case Direction::LEFT:
    --head_;
    break;
  case Direction::RIGHT:
    ++head_;
    break;
  case Direction::STAY:
    break;
  default:
    throw std::exception();
  }

  if (!tape_.contains(head_)) {
    tape_[head_] = blank_;
  }
}

std::optional<std::vector<TapeRecord>> Tape::content(bool reserveHead) {
  std::vector<int> keys;

  for (const auto &pair : tape_) {
    keys.push_back(pair.first);
  }
  std::sort(keys.begin(), keys.end());

  int firstIndexOfKeys = std::numeric_limits<int>::max(),
      lastIndexOfKeys = std::numeric_limits<int>::min();

  for (size_t i = 0; i < keys.size(); ++i) {
    int key = keys[i];
    if ((reserveHead && key == head_) || tape_[key] != blank_) {
      firstIndexOfKeys = i;
      break;
    }
  }

  for (size_t i = keys.size(); i > 0; --i) {
    int key = keys[i - 1];
    if ((reserveHead && key == head_) || tape_[key] != blank_) {
      lastIndexOfKeys = i - 1;
      break;
    }
  }

  if (firstIndexOfKeys == std::numeric_limits<int>::max() ||
      lastIndexOfKeys == std::numeric_limits<int>::min()) {
    if (reserveHead) {
      assert(tape_[head_] == blank_);
      return std::vector<TapeRecord>{{
          .index = head_,
          .sign = blank_,
          .isHead = true,
      }};
    } else {
      return std::nullopt;
    }
  }

  std::vector<TapeRecord> records;

  for (int i = firstIndexOfKeys; i <= lastIndexOfKeys; ++i) {
    int index = keys[i];
    records.push_back(TapeRecord{
        .index = index,
        .sign = tape_[index],
        .isHead = index == head_,
    });
  }

  return records;
}

Tapes::Tapes(const std::string &input, const std::string &startState,
             const std::unordered_set<std::string> &finalStates,
             const size_t nTape, const char blank)
    : step_(0), currentState_(startState), finalStates_(finalStates),
      accepted_(finalStates.contains(startState)),
      padLeft_([nTape](const std::string &s) -> std::string {
        return turing::util::string::padRight(
                   s, 5 + turing::util::number::length(nTape - 1) + 1) +
               std::string{turing::util::string::COLON} +
               std::string{turing::util::string::SPACE};
      }) {
  tapes_.emplace_back(input, blank);
  for (size_t i = 1; i < nTape; ++i) {
    tapes_.emplace_back(blank);
  }
}

void Tapes::step(const Transition &transition) {
  currentState_ = transition.newState;
  accepted_ |= finalStates_.contains(currentState_);
  ++step_;

  assert(transition.newSigns.size() == tapes_.size());
  for (size_t i = 0; i < tapes_.size(); ++i) {
    tapes_[i].move(transition.directions[i], transition.newSigns[i]);
  }
}

std::string Tapes::id() {
  std::string s;

  s += padLeft_("Step") + std::to_string(step_) + "\n";
  s += padLeft_("State") + currentState_ + "\n";
  s += padLeft_("Acc") + (accepted_ ? "Yes" : "No") + "\n";

  for (size_t i = 0; i < tapes_.size(); ++i) {
    Tape &tape = tapes_[i];

    auto op = tape.content(true);
    assert(op.has_value());
    std::vector<TapeRecord> &records = op.value();

    std::string indexLine = padLeft_("Index" + std::to_string(i));
    std::string tapeLine = padLeft_("Tape" + std::to_string(i));
    std::string headLine = padLeft_("Head" + std::to_string(i));

    // adapt width by element in index row
    for (const TapeRecord &record : records) {
      size_t paddingLength = turing::util::number::length(record.index);
      indexLine += turing::util::string::padRight(std::to_string(std::abs(record.index)),
                                                  paddingLength) +
                   turing::util::string::SPACE;
      tapeLine += turing::util::string::padRight(std::string{record.sign},
                                                 paddingLength) +
                  turing::util::string::SPACE;
      headLine +=
          turing::util::string::padRight(
              record.isHead ? std::string{turing::util::string::UPARROW}
                            : std::string{turing::util::string::SPACE},
              paddingLength) +
          turing::util::string::SPACE;
    }

    s += indexLine + "\n";
    s += tapeLine + "\n";
    s += headLine + "\n";
  }

  s += "---------------------------------------------\n";

  return s;
}

std::string Tapes::currentState() const { return currentState_; }

TapeView Tapes::currentView() const {
  std::vector<char> signs;
  for (const Tape &tape : tapes_) {
    signs.push_back(tape.currentSign());
  }
  return {
      .state = currentState_,
      .signs = signs,
  };
}

std::optional<std::string> Tapes::content() {
  assert(!tapes_.empty());

  std::optional<std::vector<TapeRecord>> optionalRecords = tapes_[0].content();

  if (!optionalRecords.has_value()) {
    return std::nullopt;
  }

  std::vector<TapeRecord> records = optionalRecords.value();

  std::string s;

  for (const TapeRecord &record : records) {
    s += record.sign;
  }

  return s;
}

bool Tapes::isAccepted() { return accepted_; }

} // namespace turing::machine
