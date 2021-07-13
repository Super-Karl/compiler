//
// Created by wnx on 2021/7/5.
//
#include "front/ast/AstNode.h"
#include "parser.hpp"
#include <iostream>
using namespace compiler::mid::ir;
namespace compiler::front::ast {
  void AST::genIR(IRList &ir, RecordTable *record) {
    for (auto i : codeBlock)
      i->genIR(ir, record);
  }

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
    string token = record->getFarther() == nullptr ? "@" + to_string(record->getID()) : "%" + to_string(record->getID());
    int val = this->value->eval(record);
    auto varInfo = new VarInfo(token, val);
    record->insertVar(token, varInfo);
    //初始化过程在ir中显式表示
    (new AssignStmt(this->name, value))->genIR(ir, record);
  }

  void ConstDeclare::genIR(mid::ir::IRList &ir, RecordTable *record) {
    string token = record->getFarther() == nullptr ? "@" + this->name->name : "%" + to_string(record->getID());
    int val = this->value->eval(record);
    auto varInfo = new VarInfo(token, val, true);
    record->insertVar(name->name, varInfo);
    //初始化过程在ir中显式表示
    (new AssignStmt(this->name, value))->genIR(ir, record);
  }

  void ArrayDeclare::genIR(mid::ir::IRList &ir, RecordTable *record) {
    string token = record->getFarther() == nullptr ? "@" : "%";
    token = token + std::to_string(record->getID());

    vector<int> shape;
    int size = 1;
    for (auto i : arrayName->index) {
      shape.push_back(i->eval(record));
      size *= i->eval(record);
    };

    //数组声明时的内存分配
    auto allocaIR = new AllocaIR(token, size);
    ir.push_back(allocaIR);

    vector<int> value;
    value.resize(size, INT32_MIN);
    auto varInfo = new VarInfo(token, shape, value);
    record->insertVar(arrayName->name, varInfo);
  }

  void ConstArray::genIR(IRList &ir, RecordTable *record) {
    string token = record->getFarther() == nullptr ? "@" : "%";
    token = token + std::to_string(record->getID());
    vector<int> shape;
    int size = 1;

    //计算数组的维度和每维的大小
    for (auto i : arrayName->index) {
      int tmp = i->eval(record);
      shape.push_back(tmp);
      size *= tmp;
    }

    //resize到数组大小
    vector<int> value;
    value.resize(size);

    //分配内存空间
    auto allocaIR = new AllocaIR(token, size);
    ir.push_back(allocaIR);

    //更新符号表,插入数组的记录
    auto varInfo = new VarInfo(token, shape, value, true);
    record->insertVar(arrayName->name, varInfo);

    //处理数组初始化元素
    initVal->storeArray(this->arrayName, ir, record);
  }

  void ArrayDeclareWithInit::genIR(mid::ir::IRList &ir, RecordTable *record) {
    string token = record->getFarther() == nullptr ? "@" : "%";
    token = token + std::to_string(record->getID());
    vector<int> shape;
    int size = 1;

    //计算数组的维度和每维的大小
    for (auto i : arrayName->index) {
      int tmp = i->eval(record);
      shape.push_back(tmp);
      size *= tmp;
    }
    vector<int> value;
    value.resize(size);

    auto allocaIR = new AllocaIR(token, size);
    ir.push_back(allocaIR);

    auto varInfo = new VarInfo(token, shape, value);
    record->insertVar(arrayName->name, varInfo);

    initVal->storeArray(this->arrayName, ir, record);
  }

  void DeclareStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
    for (auto i : declareList)
      i->genIR(ir, record);
  }

  void AssignStmt::genIR(mid::ir::IRList &ir, RecordTable *record) {
    auto assign = new AssignIR();
    auto var = record->searchVar(this->name->name);
    if (var->isArray == true) {
      std::vector<int> index;



      auto Opname = name->evalOp(ir, record);
      auto store = new StoreIR(name->evalOp(ir,record),rightExpr->eval(record),var->getArrayVal(index));
    }

    assign->source1 = this->rightExpr->evalOp(ir, record);
    assign->dest = this->name->evalOp(ir, record);
    assign->operatorCode = OperatorCode::Assign;
    ir.push_back(assign);
  }

  //完成
  void FunctionDefine::genIR(mid::ir::IRList &ir, RecordTable *record) {

    ElemType retType = this->retType == INT ? ElemType::INT : ElemType::VOID;

    //    创建函数作用域的符号表
    auto funcdef = new FunDefIR(retType, name->name);
    auto newTable = new RecordTable(record);

    //保存参数表
    for (auto i : args->args) {
      auto opName = OperatorName(Type::Var);
      opName.name = i->name->name;
      funcdef->argList.push_back(opName);
    }
    //把函数的入参存入函数符号表
    args->genIR(funcdef->funcBody, newTable);

    //对函数体生成ir
    body->genIR(funcdef->funcBody, newTable);

    ir.push_back(funcdef);
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
      auto val = i->eval(record);
      funCall->argList.push_back(val);
    }

    ir.push_back(funCall);
  }


  void IfStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
    cond->genIR(ir,record);
    auto trueLabel  = new JmpIR(,".L"+std::to_string(record->getID()));
    ir.push_back(trueLabel);
    this->trueBlock->genIR(ir,record);
    auto falseLabel = new LabelIR(".L"+std::to_string(record->getID()));
    ir.push_back(trueLabel);
    this->elseBlock->genIR(ir,record);
  }

  void ReturnStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
    if (returnExp != nullptr) {
      auto ret = returnExp->evalOp(ir, record);
    }
  }

  void BreakStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
  }

  void WhileStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
    cond->genIR(ir,record);
    auto loopLabel = new JmpIR(,".L"+ std::to_string(record->getID()));
    auto endLoopLabel = new JmpIR (,".L"+std::to_string(record->getID()));
    RecordTable::pushLabelPair(loopLabel,endLoopLabel);
    this->loopBlock->genIR(ir,record);
    RecordTable::popLabelPair();
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

  OperatorName NumberExpression::evalOp(IRList &ir, RecordTable *record) {
    return OperatorName(this->value, Type::Imm);
  }

  int Identifier::eval(RecordTable *record) {
    auto varInfo = record->searchVar(this->name);
    return varInfo->value[0];
  }

  int ArrayIdentifier::eval(RecordTable *record) {
    auto varInfo = record->searchVar(name);
    std::vector<int> index;
    for (auto i : this->index)
      index.push_back(i->eval(record));
    return varInfo->getArrayVal(index);
  }

  OperatorName Identifier::evalOp(IRList &ir, RecordTable *record) {
    auto varInfo = record->searchVar(this->name);
    auto opName = OperatorName(varInfo->value[0], Type::Var);
    opName.name = varInfo->name;
    return opName;
  }

  OperatorName ArrayIdentifier::evalOp(IRList &ir, RecordTable *record) {
    auto varInfo = record->searchVar(this->name);
    std::vector<int> index;
    for (auto i : this->index)
      index.push_back(i->eval(record));
    auto opName = OperatorName(varInfo->getArrayVal(index));
    opName.name = varInfo->name;
    return opName;
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
  OperatorName BinaryExpression::evalOp(IRList &ir, RecordTable *record) {
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

  OperatorName Expression::evalOp(IRList &ir, RecordTable *record) {
    throw std::runtime_error("this type of node cannot be eval");
  }


}// namespace compiler::front::ast
/*
 * store array when init;
 */

namespace compiler::front::ast {
  void ArrayInitVal::storeArray(ArrayIdentifier *name, IRList &ir, RecordTable *record) {
    int index = 0;
    std::string arrayIRIdent = record->searchVar(name->name)->name;
    for (auto i : initValList) {
      auto dest = OperatorName(Type::Var);
      dest.name = arrayIRIdent;
      auto store = new StoreIR(dest, i->eval(record), index++);
      ir.push_back(store);
    }
  }
}// namespace compiler::front::ast