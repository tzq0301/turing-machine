#include "turing/parser/statement_parser.h"

#include <cctype>
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include "turing/machine/direction.h"
#include "turing/parser/exception.h"
#include "turing/parser/result.h"
#include "turing/util/string.h"

namespace turing::parser {

StatementParser::StatementParser(std::string statement) : statement_(std::move(turing::util::string::trim(statement))), index_(0) {}

StatementResult StatementParser::parse() {
  if (isEmptyLine()) {
    return EmptyStatementResult{};
  }
  
  if (isComment()) {
    return CommentStatementResult{};
  }
  
  if (peekNextChar() == turing::util::string::SHARP) {
    mustNextChar(); // discord '#'
    return parseNormalStatement();
  } else {
    return parseTransitionStatement();
  }
}

StatesResult StatementParser::parseStates() {
  mustSkipLeftBracket();
  
  std::unordered_set<std::string> states;
  
  do {
    std::string state = this->mustNextToken();
    if (state.empty()) {
      throw InvalidSyntaxException("should be a non-empty token");
    }
    states.insert(state);
    
    char ch = peekNextChar();
    if (ch == turing::util::string::RIGHT_BRACKET) {
      mustNextChar(); // discord '}'
      break;
    }
    if (ch != turing::util::string::COMMA) {
      throw InvalidSyntaxException("should be a comma");
    }
    mustNextChar();
  } while (true);
  
  return {
    .states = states,
  };
}

InputAlphabetResult StatementParser::parseInputAlphabet() {
  static std::function<bool(char)> isSignValid = [](char ch) {
    if (!isprint(ch)) {
      return false;
    }

    if (ch == turing::util::string::SPACE ||
        ch == turing::util::string::COMMA ||
        ch == turing::util::string::SEMICOLON ||
        ch == turing::util::string::LEFT_BRACKET ||
        ch == turing::util::string::RIGHT_BRACKET ||
        ch == turing::util::string::STAR ||
        ch == turing::util::string::UNDERSCORE) {
      return false;
    }

    return true; 
  };
  
  mustSkipLeftBracket();

  std::unordered_set<char> alphabet;
  
  do {
    char ch = this->mustNextChar();
    if (!isSignValid(ch)) {
      throw InvalidSyntaxException("sign not valid");
    }
    alphabet.insert(ch);
    
    ch = peekNextChar();
    if (ch == turing::util::string::RIGHT_BRACKET) {
      mustNextChar(); // discord '}'
      break;
    }
    if (ch != turing::util::string::COMMA) {
      throw InvalidSyntaxException("should be a comma");
    }
    mustNextChar();
  } while (true);
  
  return {
    .inputAlphabet = alphabet,
  };
}

TapeAlphabetResult StatementParser::parseTapeAlphabet() {
  static std::function<bool(char)> isSignValid = [](char ch) {
    if (!isprint(ch)) {
      return false;
    }

    if (ch == turing::util::string::SPACE ||
        ch == turing::util::string::COMMA ||
        ch == turing::util::string::SEMICOLON ||
        ch == turing::util::string::LEFT_BRACKET ||
        ch == turing::util::string::RIGHT_BRACKET ||
        ch == turing::util::string::STAR) {
      return false;
    }

    return true; 
  };
  
  mustSkipLeftBracket();

  std::unordered_set<char> alphabet;

  do {
    char ch = mustNextChar();
    if (!isSignValid(ch)) {
      throw InvalidSyntaxException("sign not valid");
    }
    alphabet.insert(ch);

    ch = peekNextChar();
    if (ch == turing::util::string::RIGHT_BRACKET) {
      mustNextChar(); // discord '}'
      break;
    }
    if (ch != turing::util::string::COMMA) {
      throw InvalidSyntaxException("should be a comma");
    }
    mustNextChar();
  } while (true);

  return {
      .tapeAlphabet = alphabet,
  };
}

StartStateResult StatementParser::parseStartState() {
  return {
    .startState = mustNextToken(),
  };
}

FinalStatesResult StatementParser::parseFinalStates() {
  mustSkipLeftBracket();

  std::unordered_set<std::string> states;

  do {
    std::string state = mustNextToken();
    
    if (state.empty()) {
      throw InvalidSyntaxException("should be a non-empty token");
    }
    states.insert(state);

    char ch = peekNextChar();
    if (ch == turing::util::string::RIGHT_BRACKET) {
      mustNextChar(); // discord '}'
      break;
    }
    if (ch != turing::util::string::COMMA) {
      throw InvalidSyntaxException("should be a comma");
    }
    mustNextChar();
  } while (true);

  return FinalStatesResult {
      .finalStates = states,
  };
}

BlankSymbolResult StatementParser::parseBlankSymbol() {
  return {
    .blankSymbol = mustNextChar(),
  };
}

NTapeResult StatementParser::parseNTape() {
  return {
      .nTape = turing::util::string::to_size_t(mustNextToken()),
  };
}

turing::parser::NormalStatementResult turing::parser::StatementParser::parseNormalStatement() {
  char sign = mustNextChar();
  if (sign == 'q') {
    mustNextChar(); // 0 of q0
  }
  mustSkipSpace();
  mustSkipEqualSign();
  mustSkipSpace();
  
  switch (sign) {
  case 'Q':
    return parseStates();
  case 'S':
    return parseInputAlphabet();
  case 'G':
    return parseTapeAlphabet();
  case 'q':
    return parseStartState();
  case 'B':
    return parseBlankSymbol();
  case 'F':
    return parseFinalStates();
  case 'N':
    return parseNTape();
  default:
    throw std::invalid_argument("sign not match");
  }
}

TransitionStatementResult StatementParser::parseTransitionStatement() {
  turing::machine::Transition transition {
    .oldSigns   = std::vector<char>{},
    .newSigns   = std::vector<char>{},
    .directions = std::vector<turing::machine::Direction>{},
  };
  
  transition.oldState = mustNextToken();
  mustSkipSpace();
  
  std::function<void(std::vector<char> &)> readSigns = [this](std::vector<char> &signs) {
    while (this->peekNextChar() != turing::util::string::SPACE) {
      signs.push_back(this->mustNextChar());
    }
  };
  
  readSigns(transition.oldSigns);
  mustSkipSpace();
  readSigns(transition.newSigns);
  mustSkipSpace();
  
  static std::function<void(std::vector<turing::machine::Direction> &)> readDirections = [this](std::vector<turing::machine::Direction> &directions) {
    while (peekNextChar() != turing::util::string::SPACE) {
      char ch = this->mustNextChar();
      switch (ch){
      case 'l':
        directions.push_back(turing::machine::Direction::LEFT);
        break;
      case 'r':
        directions.push_back(turing::machine::Direction::RIGHT);
        break;
      default:
        directions.push_back(turing::machine::Direction::STAY);
        break;
      }
    }
  };
  readDirections(transition.directions);
  
  mustSkipSpace();
  transition.newState = mustNextToken();
  
  return {
    .transition = transition,
  };
}

std::string StatementParser::mustNextToken() {
  std::string s;
  
  while (!reachEnd()) {
    char ch = peekNextChar();
    
    if (std::isalnum(ch) || ch == turing::util::string::UNDERSCORE) {
      s += ch;
      mustNextChar();
      continue;
    }
    
    break;
  }
  
  return s;
}

char StatementParser::peekNextChar() const {
  if (reachEnd()) {
    throw InvalidSyntaxException("End of Statement");
  }
  return statement_[index_];
}

char StatementParser::mustNextChar() {
  if (reachEnd()) {
    throw InvalidSyntaxException("End of Statement");
  }
  return statement_[index_++];
}

void StatementParser::mustSkipSpace() {
  if (mustNextChar() != turing::util::string::SPACE) {
    throw InvalidSyntaxException("should have a space ( )");
  }
}

void StatementParser::mustSkipComma() {
  if (mustNextChar() != turing::util::string::COMMA) {
    throw InvalidSyntaxException("should have a comma (,)");
  }
}

void StatementParser::mustSkipEqualSign() {
  if (mustNextChar() != turing::util::string::EQUAL) {
    throw InvalidSyntaxException("should have a equal (=)");
  }
}

void StatementParser::mustSkipLeftBracket() {
  if (mustNextChar() != turing::util::string::LEFT_BRACKET) {
    throw InvalidSyntaxException("should have a left bracket ({)");
  }
}

void StatementParser::mustSkipRightBracket() {
  if (mustNextChar() != turing::util::string::RIGHT_BRACKET) {
    throw InvalidSyntaxException("should have a right bracket (})");
  }
}

bool StatementParser::isEmptyLine() const {
  return statement_ == "";
}

bool StatementParser::isComment() const {
  return !statement_.empty() && statement_[0] == turing::util::string::SEMICOLON;
}

bool StatementParser::reachEnd() const {
  return statement_.size() == index_;
}

StatementResult parseStatement(const std::string &statement) {
  return StatementParser{statement}.parse();
}
}