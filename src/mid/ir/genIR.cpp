//
// Created by wnx on 2021/7/5.
//
#include "front/ast/AstNode.h"
#include "mid/ir/ir.h"
#include <iostream>

namespace compiler::front::ast {
  using namespace compiler::mid::ir;
  void Node::genIR(mid::ir::IRList &ir, RecordTable *record) {
    std::cout << "this node can't genIR" << std::endl;
  }

  void VarDeclare::genIR(mid::ir::IRList &ir, RecordTable *record) {
    if (record->getFarther() == nullptr) {
      auto varInfo = new VarInfo("@" + std::to_string(record->getID()), INT32_MIN);
      record->insertVar(name->name, varInfo);
    } else {
      auto varInfo = new VarInfo("%" + std::to_string(record->getID()), INT32_MIN);
      record->insertVar(name->name, varInfo);
    }
  }

  void VarDeclareWithInit::genIR(mid::ir::IRList &ir, RecordTable *record) {
    if (record->getFarther() == nullptr) {
      auto varInfo = new VarInfo("@" + std::to_string(record->getID()), );
      auto assign = new AssignExpression(name, value);
      assign->genIR(ir, record);
    } else {
      auto varInfo = VarInfo("%" + std::to_string(record.getID()), INT32_MIN);
      record.insertVar(name->name, varInfo);
      auto assign = AssignExpression(this->name, this->value);
      assign.genIR(ir, record);
    }
  }

  void ConstDeclare::genIR(mid::ir::IRList &ir, RecordTable *record) {
    if (record->getFarther() != nullptr) {
      auto varInfo = new VarInfo("@" + std::to_string(record->getID()), , true);
      record->insertVar(name->name, varInfo);
    } else {
      auto varInfo = new VarInfo("%" + std::to_string(record->getID()), , true);
      record->insertVar(name->name, varInfo);
    }
  }

  void ArrayDeclare::genIR(mid::ir::IRList &ir, RecordTable *record) {
    if (record->getFarther() != nullptr) {
      auto varInfo = new VarInfo("@" + std::to_string(record->getID()), , , );
    } else {
    }
  }

  void DeclareStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
    for (auto i : declareList)
      i->genIR(ir, record);
  }

  void AssignExpression::genIR(mid::ir::IRList &ir, RecordTable *record) {

    auto operatorName = mid::ir::OperatorName();
    operatorName.value = this->name->name;
    this->rightExpr
  }

  void FunctionCall::genIR(mid::ir::IRList &ir, RecordTable *record) {
    auto funCallIR = new mid::ir::FunCallIR(this->name->name);
    for (auto i : args->args) {
    }
  }

  void FunctionDefine::genIR(mid::ir::IRList &ir, RecordTable *record) {
    auto funcdef = new FunDefIR(retType, name->name);
    auto newTable = new RecordTable(record);
    args->genIR(ir, newTable);
    body->genIR(ir, newTable);
  }

  void FunctionDefArgList::genIR(mid::ir::IRList &ir, RecordTable *record) {
    for (auto i : args)
      i->genIR(ir, record);
  }

  void FunctionDefArg::genIR(mid::ir::IRList &ir, RecordTable *record) {
    auto varInfo = new VarInfo(name->name, INT32_MIN);
  }

  void Block::genIR(mid::ir::IRList &ir, RecordTable *record) {
    for (auto item : blockItem)
      item->genIR(ir, record);
  }

  void FunctionCall::genIR(mid::ir::IRList &ir, RecordTable *record) {
    auto funCall = new FunCallIR(name->name);
    for (auto i : args) {
      auto opname = OperatorName()
    }
  }
}// namespace compiler::front::ast

/*
 * genIR
 * */

namespace compiler::front::ast {
  OperatorName NumberExpression::eval() {
    auto operatorName = OperatorName(Type::Imm);
    operatorName.name = "";
    operatorName.value = this->value;
    return operatorName;
  }


}// namespace compiler::front::ast