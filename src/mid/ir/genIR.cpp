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

  void AssignStmt::genIR(mid::ir::IRList &ir, RecordTable *record) {
    auto assign = new AssignIR();
    assign->dest = name->eval(record);

  }

  //完成
  void FunctionDefine::genIR(mid::ir::IRList &ir, RecordTable *record) {
    auto funcdef = new FunDefIR(retType, name->name);
    auto newTable = new RecordTable(record);
    args->genIR(body, newTable);
    body->genIR(funcdef, newTable);
  }
  //完成
  void FunctionDefArgList::genIR(mid::ir::IRList &ir, RecordTable *record) {
    for (auto i : args)
      i->genIR(ir, record);
  }
  //fine
  void FunctionDefArg::genIR(mid::ir::IRList &ir, RecordTable *record) {
    auto varInfo = new VarInfo(name->name, INT32_MIN);
    record->insertVar(name->name, varInfo);
  }
  //完成
  void Block::genIR(mid::ir::IRList &ir, RecordTable *record) {
    auto newTable = new RecordTable(record);
    for (auto item : blockItem)
      item->genIR(ir, newTable);
    delete newTable;
  }
  //完成
  void FunctionCall::genIR(mid::ir::IRList &ir, RecordTable *record) {
    auto funCall = new FunCallIR(name->name);
    for (auto i : args->args) {
      auto opName = i->eval(record);
      funCall->argList.push_back(opName);
    }
    ir.push_back(funCall);
  }


  void IfStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
  }

  void ReturnStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
    if (returnExp != nullptr) {
      returnExp->eval(record);
    }
    auto ret = new RetIR();
  }

  void BreakStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
  }
}// namespace compiler::front::ast

/*
 * genIR
 * */

namespace compiler::front::ast {
  OperatorName NumberExpression::eval(RecordTable *record) {
    auto operatorName = OperatorName(Type::Imm);
    operatorName.name = "";
    operatorName.value = this->value;
    return operatorName;
  }

  OperatorName Identifier::eval(RecordTable *record) {
    auto varInfo = record->searchVar(name);
    auto opName = OperatorName(Type::Var);
    opName.name = varInfo->name;
    opName.value = varInfo->value[0];
    return opName;
  }

  OperatorName ArrayIdentifier::eval(RecordTable *record) {
    auto varInfo = record->searchVar(name);
    auto opName = OperatorName(Type::Var);
    opName.name = varInfo->name;
  }


}// namespace compiler::front::ast