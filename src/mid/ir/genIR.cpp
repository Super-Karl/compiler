//
// Created by wnx on 2021/7/5.
//
#include "front/ast/AstNode.h"
#include "mid/ir/ir.h"
#include "parser.hpp"
#include <iostream>
using namespace compiler::mid::ir;
namespace compiler::front::ast {

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
    string token = record->getFarther() == nullptr ? "@" : "%";
    int val = this->value->eval(record);
    auto varInfo = new VarInfo(token + std::to_string(record->getID()), val);
    record->insertVar(name->name, varInfo);
    //初始化过程在ir中显式表示
    (new AssignStmt(this->name, value))->genIR(ir, record);
  }

  void ConstDeclare::genIR(mid::ir::IRList &ir, RecordTable *record) {
    string token = record->getFarther() == nullptr ? "@" : "%";
    int val = this->value->eval(record);
    auto varInfo = new VarInfo(token + std::to_string(record->getID()), val, true);
    record->insertVar(name->name, varInfo);
    //初始化过程在ir中显式表示
    (new AssignStmt(this->name, value))->genIR(ir, record);
  }

  void ArrayDeclare::genIR(mid::ir::IRList &ir, RecordTable *record) {
    string token = record->getFarther() == nullptr ? "@" : "%";
    vector<int> shape;
    int size = 1;
    for (auto i : arrayName->index) {
      shape.push_back(i->eval(record));
      size *= i->eval(record);
    };
    vector<int> value;
    value.resize(size, INT32_MIN);
    auto varInfo = new VarInfo(arrayName->name, shape, value);
    record->insertVar(arrayName->name, varInfo);
  }

  void ConstArray::genIR(IRList &ir, RecordTable *record) {

  }

  void ArrayDeclareWithInit::genIR(mid::ir::IRList &ir, RecordTable *record) {

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
    //267代表bison生成的符号里的INT
    ElemType retType = this->retType == INT ? ElemType::INT : ElemType::VOID;

    //    创建函数作用域的符号表
    auto funcdef = FunDefIR(retType, name->name);
    auto newTable = new RecordTable(record);

    //保存参数表
    for (auto i : args->args) {
      auto opName = OperatorName(Type::Var);
      opName.name = i->name->name;
      funcdef.argList.push_back(opName);
    }
    //把函数的入参存入函数符号表
    args->genIR(funcdef.funcBody, newTable);

    //对函数体生成ir
    body->genIR(funcdef.funcBody, newTable);
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
    auto funCall = new FunCallIR("@" + name->name);
    for (auto i : args->args) {
      auto opName = i->eval(record);
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

  void WhileStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {

  }

  void ContinueStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {

  }
}// namespace compiler::front::ast

/*
 * eval
 * */

namespace compiler::front::ast {

  int Expression::eval(RecordTable *record) {
    throw std::runtime_error("can't eval this node");
  }

  int NumberExpression::eval(RecordTable *record) {
    return this->value;
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

  int BinaryExpression::eval(RecordTable *record) {
    switch (this->op) {
      case ADD:
        return rightExpr->eval(record) + leftExpr->eval(record);
      case SUB:
        return rightExpr->eval(record) + leftExpr->eval(record);
      case MUL:
        return rightExpr->eval(record) * leftExpr->eval(record);
      case DIV:
        return leftExpr->eval(record) / rightExpr->eval(record);
      case MOD:
        return leftExpr->eval(record) % rightExpr->eval(record);
      case AND_OP:
        return leftExpr->eval(record) && rightExpr->eval(record);
      case OR_OP:
        return leftExpr->eval(record) || rightExpr->eval(record);
      case EQ:
        return leftExpr->eval(record) == rightExpr->eval(record);
      case NE:
        return leftExpr->eval(record) != rightExpr->eval(record);
      case LT:
        return leftExpr->eval(record) < rightExpr->eval(record);
      case LE:
        return leftExpr->eval(record) <= rightExpr->eval(record);
      case GT:
        return leftExpr->eval(record) > rightExpr->eval(record);
      case GE:
        return leftExpr->eval(record) >= rightExpr->eval(record);
      default:
        throw std::runtime_error("undefined op");
    }
  }

  //binExpr分解expr的过程中生成ir
  int BinaryExpression::evalIR(IRList &ir, RecordTable *record) {
  }


  int UnaryExpression::eval(RecordTable *record) {
    //转换成binaryExpr
    auto binForm = new BinaryExpression(nullptr, 0, nullptr);
    switch (this->op) {
      case ADD:
        // binForm = new BinaryExpression();
        return binForm->eval(record);
      case SUB:
        binForm = new BinaryExpression(new NumberExpression(0), SUB, this->right);
        return binForm->eval(record);
      case NOT_EQUAL:
        binForm = new BinaryExpression(new NumberExpression(0), EQ, this->right);
        return binForm->eval(record);
      default:
        throw runtime_error("undefined op");
    }
  }

  int CommaExpression::eval(RecordTable *record) {
    return expr[expr.size() - 1]->eval(record);
  }


}// namespace compiler::front::ast
