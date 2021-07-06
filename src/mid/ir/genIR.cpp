//
// Created by wnx on 2021/7/5.
//
#include "front/ast/AstNode.h"
#include "mid/ir/ir.h"
#include <iostream>

namespace compiler::front::ast {
  using namespace compiler::mid::ir;
  void Node::genIR(mid::ir::IRList &ir, mid::ir::RecordTable &record) {
    std::cout << "this node can't genIR" << std::endl;
  }

  void VarDeclare::genIR(mid::ir::IRList &ir, mid::ir::RecordTable &record) {
    if (global) {

    } else {
      auto varInfo = VarInfo("%" + std::to_string(record.getID()), INT32_MIN);
      record.insertVar(name->name, varInfo);
    }
  }

  void VarDeclareWithInit::genIR(mid::ir::IRList &ir, mid::ir::RecordTable &record) {
    if (global) {

    } else {
      auto varInfo = VarInfo("%" + std::to_string(record.getID()), INT32_MIN);
      record.insertVar(name->name, varInfo);
      auto assign = AssignExpression(this->name, this->value);
      assign.genIR(ir, record);
    }
  }

  void AssignExpression::genIR(mid::ir::IRList &ir, mid::ir::RecordTable &record) {

    auto operatorName = mid::ir::OperatorName();
    operatorName.value = this->name->name;
    this->rightExpr
  }

  void FunctionCall::genIR(mid::ir::IRList &ir, mid::ir::RecordTable &record) {
    auto funCallIR = new mid::ir::FunCallIR(this->name->name);
    for (auto i : args->args) {
    }
  }

  void FunctionDefine::genIR(mid::ir::IRList &ir, mid::ir::RecordTable &record) {
  }
}// namespace compiler::front::ast