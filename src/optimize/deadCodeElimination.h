//
// Created by wnx on 2021/8/16.
//

#ifndef COMPILER_DEADCODEELIMINATION_H
#define COMPILER_DEADCODEELIMINATION_H

#include "front/ast/AstNode.h"
#include <unordered_map>

class Table {
public:
  Table() = default;

  void setVarLatestUse(compiler::front::ast::Node *node);

  int getTimeStamp(compiler::front::ast::Node *node);

  void setTimeStamp(compiler::front::ast::Node *node);

  void setVar(const compiler::front::ast::Identifier *identifier);

  std::vector<std::vector<compiler::front::ast::Node *>::iterator> deleteList;

  std::unordered_map<compiler::front::ast::Node *, int> timeStamp;

  std::unordered_map<std::string, int> varLatestUse;

private:
  static int id;
};

#endif // COMPILER_DEADCODEELIMINATION_H
