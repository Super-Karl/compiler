//
// Created by wnx on 2021/6/11.
//
#include "controller/controller.h"
#include "front/ast/AstNode.h"
#include <iostream>

using namespace std;


int main(int argc, char **argv) {
  auto *argParser = new compiler::controller::ArgParser(argc, argv);

  auto root = compiler::controller::generator::generate(argParser->input);
  auto ir = compiler::controller::generator::genIR(root);
  if (argParser->printAST)
    root->print();
  compiler::controller::generator::printIR(ir);
  return 0;
}
