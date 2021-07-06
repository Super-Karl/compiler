//
// Created by wnx on 2021/7/5.
//
#include<iostream>
#include "front/ast/AstNode.h"
#include "mid/ir/ir.h"

namespace compiler::front::ast {

  void Node::genIR(mid::ir::IRList &ir) {
    std::cout << "this node can't genIR" << std::endl;

  }

  void VarDeclare::genIR(mid::ir::IRList &ir) {

  }

  void AssignExpression::genIR(mid::ir::IRList &ir) {

    auto operatorName = mid::ir::OperatorName();
  }

  void FunctionCall::genIR(mid::ir::IRList &ir) {
    auto funCallIR = new mid::ir::FunCallIR(this->name->name);
    for (auto i : args->args) {

    }
  }

  void FunctionDefine::genIR(mid::ir::IRList &ir) {

  }
}