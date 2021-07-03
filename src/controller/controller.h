//
// Created by wnx on 2021/6/13.
//

#ifndef COMPILER_CONTROLLER_H
#define COMPILER_CONTROLLER_H

#include <front/ast/AstNode.h>
#include <mid/ir/ir.h>
#include <iostream>
#include <fstream>

using namespace compiler::front;

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

  class FrontGenerator {
  public:
    ast::AST *root;

    ast::AST *generateAst(FILE *input);

    ir::IR *generateIR(ast::AST *root);
  };

  class Controller {
  public:

  };
}

#endif //COMPILER_CONTROLLER_H
