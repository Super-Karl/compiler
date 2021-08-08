//
// Created by wnx on 2021/6/13.
//

#ifndef COMPILER_CONTROLLER_H
#define COMPILER_CONTROLLER_H

#include "../front/ast/AstNode.h"
#include "../mid/ir/ir.h"
#include <iostream>
#include <fstream>

namespace compiler::controller {
  class ArgParser {
  public:
    bool isOptimize = false;
    bool printAST = false;
    bool printIR = false;
    FILE *input;
    std::ostream *output = nullptr;

    ArgParser(int argc, char **argv);
  };
  namespace generator {
    extern compiler::front::ast::AST *root;

    compiler::front::ast::AST *generate(FILE *input = stdin);
  }
}


#endif //COMPILER_CONTROLLER_H
