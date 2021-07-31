//
// Created by wnx on 2021/6/11.
//
#include "astpass/astpass.h"
#include "controller/controller.h"
#include "front/ast/AstNode.h"
#include <iostream>
#include <unordered_map>

using namespace std;

using Hash = std::unordered_map<string, int>;
int main(int argc, char **argv) {
  auto *argParser = new compiler::controller::ArgParser(argc, argv);

  auto *root = compiler::controller::generator::generate(argParser->input);

//  root->print();
  Hash constTbale;
  compiler::astpassir::FirstPassRoot(root, constTbale);

  auto ir = compiler::controller::generator::genIR(root);
  freopen("gen.ir","w",stdout);
  compiler::controller::generator::printIR(ir);
  return 0;
}
