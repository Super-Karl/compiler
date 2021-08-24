//
// Created by karl on 2021/7/7.
//
#pragma once
#ifndef COMPILER_ASTPASS_H
#define COMPILER_ASTPASS_H

#include "../enum/enums.h"
//#include "parser.hpp"
#include "../front/ast/AstNode.h"
#include <unordered_map>
#include <vector>

using Hash = std::unordered_map<string, int>;
namespace compiler::astpassir {
  bool caluExpersion(compiler::front::ast::Expression *exp, int &result);

  void FirstPassRoot(compiler::front::ast::AST *root, Hash constTbale);

  void FirstPassNode(compiler::front::ast::Block *node, Hash constTbale);

  void FirstPassStmt(compiler::front::ast::Stmt *stmt, Hash constTbale);

  void FirstPassArray(compiler::front::ast::Declare *array, Hash constTbale);

  compiler::front::ast::Expression *FirstPassExpr(compiler::front::ast::Expression *expr, Hash constTbale);

  vector<compiler::front::ast::Expression *> FirstPassArrayLinelize(int sizeIndex, vector<compiler::front::ast::Expression *> &index, vector<compiler::front::ast::Expression *> &values);
}// namespace compiler::astpassir

#endif//COMPILER_ASTPASS_H
