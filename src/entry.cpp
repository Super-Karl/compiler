//
// Created by wnx on 2021/6/11.
//
#include <iostream>
#include "front/ast/AstNode.h"
#include "controller/controller.h"

using namespace std;


int main(int argc, char **argv) {
  auto *argParser = new compiler::controller::ArgParser(argc,argv);

  auto *root = compiler::controller::generator::generate(argParser->input);
  if(argParser->printAST)
    root->print();
  return 0;
}