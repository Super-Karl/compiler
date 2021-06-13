//
// Created by wnx on 2021/6/13.
//

#ifndef COMPILER_CONTROLLER_H
#define COMPILER_CONTROLLER_H

#include <front/ast/AstNode.h>
#include <mid/ir/ir.h>

using namespace compiler::front;

namespace compiler::controller{
  class ArgParser{

  };

  class FrontGenerator{
  public:
    ast::AST* generateAst();
    ir::IR* generateIR(ast::AST *root);
  };

  class Controller{
  public:

  };
}

#endif //COMPILER_CONTROLLER_H
