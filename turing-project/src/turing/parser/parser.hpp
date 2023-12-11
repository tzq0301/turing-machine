#pragma once

#include <optional>
#include <string>
#include <vector>

#include "turing/machine/machine.h"
#include "turing/parser/result.h"
#include "turing/parser/statement_parser.h"
#include "turing/util/file.h"

namespace turing::parser {
namespace {
template <class... Ts> struct overload : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overload(Ts...) -> overload<Ts...>;
} // namespace

class Parser {
public:
  explicit Parser(const std::string &filepath);

  turing::machine::Machine parse() {
    std::unordered_set<std::string> states;
    std::unordered_set<char> inputAlphabet;
    std::unordered_set<char> tapeAlphabet;
    std::string startState;
    char blankSymbol;
    std::unordered_set<std::string> finalStates;
    size_t nTape;
    std::unordered_map<std::string, std::vector<turing::machine::Transition>> transitions;

    std::optional<std::string> statement;
    while ((statement = this->nextStatement()) != std::nullopt) {
      std::visit(
          overload{
              [&states, &inputAlphabet, &tapeAlphabet, &startState,
               &finalStates, &blankSymbol,
               &nTape](const NormalStatementResult &normalStatementResult) {
                std::visit(
                    overload{
                        [&states](const StatesResult &statesResult) {
                          states = statesResult.states;
                        },
                        [&inputAlphabet](
                            const InputAlphabetResult &inputAlphabetResult) {
                          inputAlphabet = inputAlphabetResult.inputAlphabet;
                        },
                        [&tapeAlphabet](
                            const TapeAlphabetResult &tapeAlphabetResult) {
                          tapeAlphabet = tapeAlphabetResult.tapeAlphabet;
                        },
                        [&startState](
                            const StartStateResult &startStateResult) {
                          startState = startStateResult.startState;
                        },
                        [&finalStates](
                            const FinalStatesResult &finalStatesResult) {
                          finalStates = finalStatesResult.finalStates;
                        },
                        [&blankSymbol](
                            const BlankSymbolResult &blankSymbolResult) {
                          blankSymbol = blankSymbolResult.blankSymbol;
                        },
                        [&nTape](const NTapeResult &nTapeResult) {
                          nTape = nTapeResult.nTape;
                        }},
                    normalStatementResult);
              },
              [&transitions](
                  const TransitionStatementResult &transitionStatementResult) {
                auto transition = transitionStatementResult.transition;
                transitions[transition.oldState].emplace_back(transition);
              },
              [](const EmptyStatementResult &) {},
              [](const CommentStatementResult &) {},
          },
          turing::parser::parseStatement(*statement));
    }

    return machine::Machine{states, inputAlphabet, tapeAlphabet, startState, blankSymbol, finalStates, nTape, transitions};
  }

private:
  const std::vector<std::string> statements_;
  size_t index_;

  std::optional<std::string> nextStatement();
};

turing::machine::Machine parse(const std::string &filepath);

} // namespace turing::parser