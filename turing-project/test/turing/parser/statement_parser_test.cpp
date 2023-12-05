#include "turing/machine/direction.h"
#include "turing/machine/transition.h"
#include "turing/parser/result.h"
#include "turing/parser/statement_parser.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_set>
#include <variant>

void testEmpty(const std::string &&statement) {
  turing::parser::StatementResult statementResult = turing::parser::parseStatement(statement);
  assert(std::holds_alternative<turing::parser::EmptyStatementResult>(statementResult));
}

void testComment(const std::string &&statement) {
  turing::parser::StatementResult statementResult = turing::parser::parseStatement(statement);
  assert(std::holds_alternative<turing::parser::CommentStatementResult>(statementResult));
}

void testParseNTape(const std::string &&statement, const size_t nTape) {
  turing::parser::StatementResult statementResult = turing::parser::parseStatement(statement);
  assert(std::holds_alternative<turing::parser::NormalStatementResult>(statementResult));
  turing::parser::NormalStatementResult normalStatementResult = std::get<turing::parser::NormalStatementResult>(statementResult);
  assert(std::holds_alternative<turing::parser::NTapeResult>(normalStatementResult));
  turing::parser::NTapeResult result = std::get<turing::parser::NTapeResult>(normalStatementResult);
  assert(result.nTape == nTape);
}

void testBlankSymbol(const std::string &&statement, char blankSymbol) {
  turing::parser::StatementResult statementResult = turing::parser::parseStatement(statement);
  assert(std::holds_alternative<turing::parser::NormalStatementResult>(statementResult));
  turing::parser::NormalStatementResult normalStatementResult = std::get<turing::parser::NormalStatementResult>(statementResult);
  assert(std::holds_alternative<turing::parser::BlankSymbolResult>(normalStatementResult));
  turing::parser::BlankSymbolResult result = std::get<turing::parser::BlankSymbolResult>(normalStatementResult);
  assert(result.blankSymbol == blankSymbol);
}

void testStartState(const std::string &&statement, std::string &&startState) {
  turing::parser::StatementResult statementResult = turing::parser::parseStatement(statement);
  assert(std::holds_alternative<turing::parser::NormalStatementResult>(statementResult));
  turing::parser::NormalStatementResult normalStatementResult = std::get<turing::parser::NormalStatementResult>(statementResult);
  assert(std::holds_alternative<turing::parser::StartStateResult>(normalStatementResult));
  turing::parser::StartStateResult result = std::get<turing::parser::StartStateResult>(normalStatementResult);
  assert(result.startState == startState);
}

void testFinalStates(const std::string &&statement, std::unordered_set<std::string> &&finalStates) {
  turing::parser::StatementResult statementResult = turing::parser::parseStatement(statement);
  assert(std::holds_alternative<turing::parser::NormalStatementResult>(statementResult));
  turing::parser::NormalStatementResult normalStatementResult = std::get<turing::parser::NormalStatementResult>(statementResult);
  assert(std::holds_alternative<turing::parser::FinalStatesResult>(normalStatementResult));
  turing::parser::FinalStatesResult result = std::get<turing::parser::FinalStatesResult>(normalStatementResult);
  assert(result.finalStates == finalStates);
}

void testInputAlphabet(const std::string &&statement, std::unordered_set<char> &&inputAlphabet) {
  turing::parser::StatementResult statementResult = turing::parser::parseStatement(statement);
  assert(std::holds_alternative<turing::parser::NormalStatementResult>(statementResult));
  turing::parser::NormalStatementResult normalStatementResult = std::get<turing::parser::NormalStatementResult>(statementResult);
  assert(std::holds_alternative<turing::parser::InputAlphabetResult>(normalStatementResult));
  turing::parser::InputAlphabetResult result = std::get<turing::parser::InputAlphabetResult>(normalStatementResult);
  assert(result.inputAlphabet == inputAlphabet);
}

void testTapeAlphabet(const std::string &&statement, std::unordered_set<char> &&tapeAlphabet) {
  turing::parser::StatementResult statementResult = turing::parser::parseStatement(statement);
  assert(std::holds_alternative<turing::parser::NormalStatementResult>(statementResult));
  turing::parser::NormalStatementResult normalStatementResult = std::get<turing::parser::NormalStatementResult>(statementResult);
  assert(std::holds_alternative<turing::parser::TapeAlphabetResult>(normalStatementResult));
  turing::parser::TapeAlphabetResult result = std::get<turing::parser::TapeAlphabetResult>(normalStatementResult);
  assert(result.tapeAlphabet == tapeAlphabet);
}

void testStates(const std::string &&statement, std::unordered_set<std::string> &&states) {
  turing::parser::StatementResult statementResult = turing::parser::parseStatement(statement);
  assert(std::holds_alternative<turing::parser::NormalStatementResult>(statementResult));
  turing::parser::NormalStatementResult normalStatementResult = std::get<turing::parser::NormalStatementResult>(statementResult);
  assert(std::holds_alternative<turing::parser::StatesResult>(normalStatementResult));
  turing::parser::StatesResult result = std::get<turing::parser::StatesResult>(normalStatementResult);
  assert(result.states == states);
}

void testTransition(const std::string &&statement, turing::machine::Transition transition) {
  turing::parser::StatementResult statementResult = turing::parser::parseStatement(statement);
  assert(std::holds_alternative<turing::parser::TransitionStatementResult>(statementResult));
  turing::parser::TransitionStatementResult result = std::get<turing::parser::TransitionStatementResult>(statementResult);
  assert(result.transition == transition);
}

int main() {
  testEmpty("");
  
  testComment(";");
  testComment("; ");
  testComment(";comment");
  testComment("; comment");
  testComment(" ; comment");
  
  testParseNTape("#N = 2", 2);
  testParseNTape("#N = 2 ", 2);
  testParseNTape("#N = 13", 13);
  testParseNTape("#N = 132", 132);
  testParseNTape("#N = 132;", 132);
  testParseNTape("#N = 132 ;comment", 132);
  testParseNTape("  \t #N = 132 ;comment", 132);
  
  testBlankSymbol("#B = _", '_');
  testBlankSymbol("#B = B", 'B');
  
  testStartState("#q0 = 0", "0");
  testStartState("#q0 = start", "start");
  testStartState("#q0 = accept4", "accept4");
  
  testFinalStates("#F = {halt_accept}", {"halt_accept"});
  testFinalStates("#F = {halt_accept,reject1}", {"halt_accept", "reject1"});
  testFinalStates(" #F = {halt_accept,reject1};asd", {"halt_accept", "reject1"});
  
  testInputAlphabet("#S = {0,1}", {'0', '1'});
  testInputAlphabet("#S = {a,b,c,#,@,.}", {'a', 'b', 'c', '#', '@', '.'});
  
  testTapeAlphabet("#G = {0,1,_,t,r,u,e,f,a,l,s}", {'0', '1', '_', 't', 'r', 'u', 'e', 'f', 'a', 'l', 's'});
  
  testStates("#Q = {0,cp,cmp,mh,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject}", {"0", "cp", "cmp", "mh", "accept", "accept2", "accept3", "accept4", "halt_accept", "reject", "reject2", "reject3", "reject4", "reject5", "halt_reject"});
  
  testTransition("0 0_ 0_ ** cp", turing::machine::Transition{
    .oldState = "0",
    .oldSigns = {'0', '_'},
    .newSigns = {'0', '_'},
    .directions = {turing::machine::Direction::STAY, turing::machine::Direction::STAY},
    .newState = "cp"
  });
  testTransition("0 1_ 1_ ** cp", turing::machine::Transition{
    .oldState = "0",
    .oldSigns = {'1', '_'},
    .newSigns = {'1', '_'},
    .directions = {turing::machine::Direction::STAY, turing::machine::Direction::STAY},
    .newState = "cp"
  });
  testTransition("0 __ __ ** accept ; empty input", turing::machine::Transition{
    .oldState = "0",
    .oldSigns = {'_', '_'},
    .newSigns = {'_', '_'},
    .directions = {turing::machine::Direction::STAY, turing::machine::Direction::STAY},
    .newState = "accept"
  });
  testTransition("mh 00 00 l* mh", turing::machine::Transition{
    .oldState = "mh",
    .oldSigns = {'0', '0'},
    .newSigns = {'0', '0'},
    .directions = {turing::machine::Direction::LEFT, turing::machine::Direction::STAY},
    .newState = "mh"
  });

  testTransition("accept2 __ r_ r* accept3", turing::machine::Transition{
    .oldState = "accept2",
    .oldSigns = {'_', '_'},
    .newSigns = {'r', '_'},
    .directions = {turing::machine::Direction::RIGHT, turing::machine::Direction::STAY},
    .newState = "accept3"
  });
}
