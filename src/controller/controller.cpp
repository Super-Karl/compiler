//
// Created by wnx on 2021/6/13.
//

#include "controller.h"

using namespace compiler::controller;

extern int yydebug;

ArgParser::ArgParser(int argc, char **argv) {
  bool writeToFile = false;
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if (std::string(argv[i]) == "-o") {
        writeToFile = true;
      }
      if (std::string(argv[i]) == "-Optimize") {
        this->isOptimize = true;
      }
      if (std::string(argv[i]) == "-printAST") {
        this->printAST = true;
      }
      if (std::string(argv[i]) == "-printIR") {
        this->printIR = true;
      }
      if (std::string(argv[i]) == "-debug") {
        yydebug = true;
      }
    } else {
      if (writeToFile) {
        output = new std::ofstream(argv[i],std::ofstream::out);
        input = fopen(argv[i],"r");
      } else {
        output = &std::cout;
        input = fopen(argv[i],"r");
      }
      writeToFile = true;
    }
  }
}