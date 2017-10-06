#ifndef TESTUTILITY_HPP
#define TESTUTILITY_HPP
#include <cstdio>
#include <array>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>

static std::string getCommand(std::string&& _command) {
  std::stringstream buffer;
  FILE* in = popen(_command.c_str(), "r");
  if (in) {
    std::array<char, 512> buff;
    while (fgets(buff.data(), buff.size(), in) != nullptr) {
      // remove ***\n*** from buffer
      auto pos = strlen(buff.data());
      if (buff[pos - 1] == '\n') buff[pos - 1] = 0;
      buffer << buff.data();
    }
  }
  return buffer.str();
}

#endif  // TESTUTILITY_HPP
