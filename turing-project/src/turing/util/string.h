#pragma once

#include <algorithm>
#include <iostream>
#include <string>

namespace turing::util::string {
const std::string WHITESPACE = " \n\r\t\f\v";
const char STAR = '*';
const char COMMA = ',';
const char SPACE = ' ';
const char EQUAL = '=';
const char SHARP = '#';
const char UPARROW = '^';
const char SEMICOLON = ';';
const char UNDERSCORE = '_';
const char LEFT_BRACKET = '{';
const char RIGHT_BRACKET = '}';

std::string ltrim(const std::string &s);

std::string rtrim(const std::string &s);

std::string trim(const std::string &s);

size_t to_size_t(const std::string number);

std::string times(std::string base, size_t time);
} // namespace turing::util::string