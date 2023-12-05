#include "turing/util/file.h"

#include <exception>
#include <fstream>
#include <stdexcept>

std::vector<std::string> turing::util::file::readLines(std::string filepath) {
  std::ifstream file(filepath);

  if (!file.is_open()) {
    throw std::invalid_argument("invalid filepath");
  }

  std::vector<std::string> lines;
  std::string line;

  while (std::getline(file, line)) {
    lines.emplace_back(line);
  }

  return lines;
}