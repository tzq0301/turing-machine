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

turing::parser::StatementParser::StatementParser(std::string statement) : statement_(std::move(turing::util::string::trim(statement))), index_(0) {}

turing::parser::StatementResult turing::parser::StatementParser::parse() {
  if (this->isEmptyLine()) {
    return EmptyStatementResult{};
  }
  
  if (this->isComment()) {
    return CommentStatementResult{};
  }
  
  if (this->peekNextChar() == turing::util::string::SHARP) {
    this->mustNextChar(); // discord '#'
    return this->parseNormalStatement();
  } else {
    return this->parseTransitionStatement();
  }
}

turing::parser::StatesResult turing::parser::StatementParser::parseStates() {
  mustSkipLeftBracket();
  
  std::unordered_set<std::string> states;
  
  do {
    std::string state = this->mustNextToken();
    if (state.empty()) {
      throw InvalidSyntaxException("should be a non-empty token");
    }
    states.insert(state);
    
    char ch = this->peekNextChar();
    if (ch == turing::util::string::RIGHT_BRACKET) {
      this->mustNextChar(); // discord '}'
      break;
    }
    if (ch != turing::util::string::COMMA) {
      throw InvalidSyntaxException("should be a comma");
    }
    this->mustNextChar();
  } while (true);
  
  return turing::parser::StatesResult {
    .states = states,
  };
}

turing::parser::InputAlphabetResult turing::parser::StatementParser::parseInputAlphabet() {
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
    
    ch = this->peekNextChar();
    if (ch == turing::util::string::RIGHT_BRACKET) {
      this->mustNextChar(); // discord '}'
      break;
    }
    if (ch != turing::util::string::COMMA) {
      throw InvalidSyntaxException("should be a comma");
    }
    this->mustNextChar();
  } while (true);
  
  return turing::parser::InputAlphabetResult {
    .inputAlphabet = alphabet,
  };
}

turing::parser::TapeAlphabetResult turing::parser::StatementParser::parseTapeAlphabet() {
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
    char ch = this->mustNextChar();
    if (!isSignValid(ch)) {
      throw InvalidSyntaxException("sign not valid");
    }
    alphabet.insert(ch);

    ch = this->peekNextChar();
    if (ch == turing::util::string::RIGHT_BRACKET) {
      this->mustNextChar(); // discord '}'
      break;
    }
    if (ch != turing::util::string::COMMA) {
      throw InvalidSyntaxException("should be a comma");
    }
    this->mustNextChar();
  } while (true);

  return turing::parser::TapeAlphabetResult {
      .tapeAlphabet = alphabet,
  };
}

turing::parser::StartStateResult turing::parser::StatementParser::parseStartState() {
  return turing::parser::StartStateResult {
    .startState = mustNextToken(),
  };
}

turing::parser::FinalStatesResult turing::parser::StatementParser::parseFinalStates() {
  mustSkipLeftBracket();

  std::unordered_set<std::string> states;

  do {
    std::string state = this->mustNextToken();
    
    if (state.empty()) {
      throw InvalidSyntaxException("should be a non-empty token");
    }
    states.insert(state);

    char ch = this->peekNextChar();
    if (ch == turing::util::string::RIGHT_BRACKET) {
      this->mustNextChar(); // discord '}'
      break;
    }
    if (ch != turing::util::string::COMMA) {
      throw InvalidSyntaxException("should be a comma");
    }
    this->mustNextChar();
  } while (true);

  return turing::parser::FinalStatesResult {
      .finalStates = states,
  };
}

turing::parser::BlankSymbolResult turing::parser::StatementParser::parseBlankSymbol() {
  return turing::parser::BlankSymbolResult {
    .blankSymbol = mustNextChar(),
  };
}

turing::parser::NTapeResult turing::parser::StatementParser::parseNTape() {
  return turing::parser::NTapeResult {
      .nTape = turing::util::string::to_size_t(mustNextToken()),
  };
}

turing::parser::NormalStatementResult turing::parser::StatementParser::parseNormalStatement() {
  char sign = this->mustNextChar();
  if (sign == 'q') {
    mustNextChar(); // 0 of q0
  }
  this->mustSkipSpace();
  this->mustSkipEqualSign();
  this->mustSkipSpace();
  
  switch (sign) {
  case 'Q':
    return this->parseStates();
  case 'S':
    return this->parseInputAlphabet();
  case 'G':
    return this->parseTapeAlphabet();
  case 'q':
    return this->parseStartState();
  case 'B':
    return this->parseBlankSymbol();
  case 'F':
    return this->parseFinalStates();
  case 'N':
    return this->parseNTape();
  default:
    throw std::invalid_argument("sign not match");
  }
}

turing::parser::TransitionStatementResult turing::parser::StatementParser::parseTransitionStatement() {
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
    while (this->peekNextChar() != turing::util::string::SPACE) {
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
  
  return turing::parser::TransitionStatementResult {
    .transition = transition,
  };
}

std::string turing::parser::StatementParser::mustNextToken() {
  std::string s;
  
  while (!this->reachEnd()) {
    char ch = this->peekNextChar();
    
    if (std::isalnum(ch) || ch == turing::util::string::UNDERSCORE) {
      s += ch;
      this->mustNextChar();
      continue;
    }
    
    break;
  }
  
  return s;
}

char turing::parser::StatementParser::peekNextChar() const {
  if (this->reachEnd()) {
    throw InvalidSyntaxException("End of Statement");
  }
  return this->statement_[this->index_];
}

char turing::parser::StatementParser::mustNextChar() {
  if (this->reachEnd()) {
    throw InvalidSyntaxException("End of Statement");
  }
  return this->statement_[this->index_++];
}

void turing::parser::StatementParser::mustSkipSpace() {
  if (this->mustNextChar() != turing::util::string::SPACE) {
    throw InvalidSyntaxException("should have a space ( )");
  }
}

void turing::parser::StatementParser::mustSkipComma() {
  if (this->mustNextChar() != turing::util::string::COMMA) {
    throw InvalidSyntaxException("should have a comma (,)");
  }
}

void turing::parser::StatementParser::mustSkipEqualSign() {
  if (this->mustNextChar() != turing::util::string::EQUAL) {
    throw InvalidSyntaxException("should have a equal (=)");
  }
}

void turing::parser::StatementParser::mustSkipLeftBracket() {
  if (this->mustNextChar() != turing::util::string::LEFT_BRACKET) {
    throw InvalidSyntaxException("should have a left bracket ({)");
  }
}

void turing::parser::StatementParser::mustSkipRightBracket() {
  if (this->mustNextChar() != turing::util::string::RIGHT_BRACKET) {
    throw InvalidSyntaxException("should have a right bracket (})");
  }
}

bool turing::parser::StatementParser::isEmptyLine() const {
  return this->statement_ == "";
}

bool turing::parser::StatementParser::isComment() const {
  return !this->statement_.empty() && this->statement_[0] == turing::util::string::SEMICOLON;
}

bool turing::parser::StatementParser::reachEnd() const {
  return this->statement_.size() == this->index_;
}

turing::parser::StatementResult turing::parser::parseStatement(const std::string &statement) {
  return StatementParser{statement}.parse();
}
