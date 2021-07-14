
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
    record->insertVar(name->name, varInfo);
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
    auto var = record->searchVar(this->name->name);
    if (var->isArray) {
      auto opName = name->evalOp(ir, record);
      opName.name = record->getFarther() == nullptr ? "%" : "@" + to_string(record->getID());

      std::vector<int> index;
      for (auto i : name->getIndex())
        index.push_back(i->eval(record));

      auto store = new StoreIR(name->evalOp(ir, record), rightExpr->eval(record), var->getArrayIndex(index));
      ir.push_back(store);
      VarRedefChain varUse;

      try {
        varUse = VarRedefChain(opName.name, this->name->eval(record), true);
      } catch (...) {
        varUse = VarRedefChain(opName.name, INT32_MIN, false);
      }
      var->addVarUse(varUse, index);

    } else {
      auto assign = new AssignIR();
      assign->source1 = rightExpr->evalOp(ir, record);
      assign->operatorCode = OperatorCode::Assign;
      assign->dest = OperatorName((record->getFarther() == nullptr ? "@" : "%") + to_string(record->getID()));
      ir.push_back(assign);

      //更新符号表
      VarRedefChain varUse;
      try {
        varUse = VarRedefChain(assign->dest.name, rightExpr->eval(record), true);
      } catch (...) {
        varUse = VarRedefChain(assign->dest.name, INT32_MIN);
      }
      var->addVarUse(varUse);
    }
  }

  //完成
  void FunctionDefine::genIR(mid::ir::IRList &ir, RecordTable *record) {

    ElemType retType = this->retType == INT ? ElemType::INT : ElemType::VOID;

    //    创建函数作用域的符号表
    auto funcdef = new FunDefIR(retType, name->name);
    auto newTable = new RecordTable(record);

    //保存参数表
    for (auto i : args->args) {
      auto opName = OperatorName(i->name->name, Type::Var);
      funcdef->argList.push_back(opName);
    }
    //把函数的入参存入函数符号表
    args->genIR(funcdef->funcBody, newTable);

    //对函数体生成ir
    body->genIR(funcdef->funcBody, newTable);

    ir.push_back(funcdef);

    RetIR *ret;
    if (this->retType == INT) {
      ret = new RetIR(OperatorName(0, Type::Var));
    } else
      ret = new RetIR(OperatorName(Type::Void));
    ir.push_back(ret);
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
  }

  void IfStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
    auto ifLabel = new LabelIR(".L"+std::to_string(record->getID()));
    auto elseLabel = new LabelIR (".L"+std::to_string(record->getID()));
    auto endLabel = new LabelIR(".L"+std::to_string(record->getID()));
    cond->ConditionAnalysis(ir,record,ifLabel,elseLabel, true);
    ir.push_back(ifLabel);
    trueBlock->genIR(ir,record);
    ir.push_back(new JmpIR(OperatorCode::Jmp,endLabel));
    ir.push_back(elseLabel);
    this->elseBlock->genIR(ir,record);
    ir.push_back(endLabel);
  }

  void ReturnStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
    if (returnExp != nullptr) {
      auto ret = new RetIR(returnExp->evalOp(ir, record));
      ir.push_back(ret);
    } else {
      auto ret = new RetIR();
      ir.push_back(ret);
    }
  }

  void BreakStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
    ir.emplace_back(new JmpIR(OperatorCode::Jmp,RecordTable::getTopLabel().second));
  }

  void WhileStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
    auto loopLabel = new LabelIR(".L"+ std::to_string(record->getID()));
    auto endLoopLabel = new LabelIR (".L"+std::to_string(record->getID()));
    cond->ConditionAnalysis(ir,record,loopLabel,endLoopLabel, true);
    ir.push_back(loopLabel);
    RecordTable::pushLabelPair(loopLabel,endLoopLabel);
    this->loopBlock->genIR(ir,record);
    ir.push_back(endLoopLabel);
    RecordTable::popLabelPair();
  }

  void ContinueStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
    ir.emplace_back(new JmpIR(OperatorCode::Jmp,RecordTable::getTopLabel().first));
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

  int Identifier::eval(RecordTable *record) const {
    auto varInfo = record->searchVar(this->name);
    if (varInfo->canAssign())
      return varInfo->getVal();
    throw std::logic_error("it cannot assign");
  }

  int ArrayIdentifier::eval(RecordTable *record) {
    auto varInfo = record->searchVar(name);
    std::vector<int> index;
    for (auto i : this->index)
      index.push_back(i->eval(record));
    if (varInfo->canAssign(index))
      return varInfo->getArrayVal(index);
    throw std::logic_error("it cannot assign");
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

  int UnaryExpression::eval(RecordTable *record) {
    //转换成binaryExpr
    auto binForm = new BinaryExpression(nullptr, 0, nullptr);
    switch (this->op) {
      case ADD:
        binForm = new BinaryExpression(new NumberExpression(0), ADD, this->right);
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
  /* *
   *
   * condition Analysis
   *
   * */
  void Expression::ConditionAnalysis(IRList &ir, RecordTable *record,LabelIR *ifLabel,LabelIR *elLable,bool trueJmp){
    try{
      if(this->eval(record)){
        ir.emplace_back(new JmpIR(OperatorCode::Jmp,ifLabel));
      }
    }catch(runtime_error e) {
      try{
        this->evalOp(ir,record);
        ir.emplace_back(new JmpIR(OperatorCode::Jne,ifLabel));
      }
      catch (runtime_error e){
        if (trueJmp)
        {
          OperatorName dest = OperatorName((record->getFarther() == nullptr ? "@" : "%") + to_string(record->getID())), left, right;
          AssignIR* assign = new AssignIR(OperatorCode::Cmp, dest, left, right);
          ir.push_back(assign);
          ir.emplace_back((new JmpIR (static_cast<BinaryExpression*>(this)->getRelOpCode(),ifLabel)));
        }
        else
        {
          //ir.emplace_back((new JmpIR (static_cast<BinaryExpression*>(this)->getAntiRelOpCode(),)));
        }
      }
    }
  }
  void BinaryExpression::ConditionAnalysis(IRList &ir, RecordTable *record,LabelIR*ifLabel,LabelIR* elLabel,bool trueJmp) {


    /*
     * 关键问题在于:
     * 最终的目的是能够利用好屏蔽效果，即每次调用的目标应该是跳转到本表达式的结束label
     * 优化的策略则是利用屏蔽策略，能够屏蔽更多的运算，跳转到更远的地方
     *
     * 当前的简单策略，将每一个表达式拆分成if else两种结果，根据表达式含义推断跳转的位置
    */
    if (this->op == AND_OP ){
      LabelIR *innerLabel = new LabelIR(".L"+std::to_string(record->getID()));
      leftExpr->ConditionAnalysis(ir,record,innerLabel,elLabel,true);
      ir.push_back(innerLabel);
      rightExpr->ConditionAnalysis(ir,record, ifLabel,elLabel, true)
    }
    else if (this->op == OR_OP){
      LabelIR *innerLabel = new LabelIR(".L"+std::to_string(record->getID()));
      leftExpr->ConditionAnalysis(ir,record, ifLabel,innerLabel, true);
      rightExpr->ConditionAnalysis(ir,record,ifLabel,elLabel,true);
    }
    else
    {
      try{
        if(this->eval(record)){
          ir.emplace_back(new JmpIR(OperatorCode::Jmp,ifLabel));
        }
        else
        {
          //别跳了
        }
      }catch(runtime_error e) {
        try{
          this->evalOp(ir,record);
          ir.emplace_back(new JmpIR(OperatorCode::Jne,ifLabel));
        }
        catch (runtime_error e){
          if (trueJmp)
          {
            OperatorName dest = OperatorName((record->getFarther() == nullptr ? "@" : "%") + to_string(record->getID())), left, right;
            AssignIR* assign = new AssignIR(OperatorCode::Cmp, dest, left, right);
            ir.push_back(assign);
            ir.emplace_back((new JmpIR (static_cast<BinaryExpression*>(this)->getRelOpCode(),ifLabel)));
          }
          else
          {
            //没有else
            //ir.emplace_back((new JmpIR (static_cast<BinaryExpression*>(this)->getAntiRelOpCode(),elLabel)));
          }
        }
      }
    }
  }

}// namespace compiler::front::ast


/*
 *evalOp
 */

namespace compiler::front::ast {
  OperatorName NumberExpression::evalOp(IRList &ir, RecordTable *record) {
    return OperatorName(this->value, Type::Imm);
  }

  OperatorName UnaryExpression::evalOp(IRList &ir, RecordTable *record) {
    auto binForm = new BinaryExpression(nullptr, 0, nullptr);
    switch (this->op) {
      case ADD:
        binForm = new BinaryExpression(new NumberExpression(0), ADD, right);
        return binForm->evalOp(ir, record);
      case SUB:
        binForm = new BinaryExpression(new NumberExpression(0), SUB, right);
        return binForm->evalOp(ir, record);
      case NOT_EQUAL:
        binForm = new BinaryExpression(new NumberExpression(0), EQ, right);
        return binForm->evalOp(ir, record);
      default:
        throw runtime_error("undefined op");
    }
  }

  //binExpr分解expr的过程中生成ir
  OperatorName BinaryExpression::evalOp(IRList &ir, RecordTable *record) {
        try {
      return this->eval(record);
    } catch (...) {
    }
    OperatorName dest = OperatorName((record->getFarther() == nullptr ? "@" : "%") + to_string(record->getID())), left, right;

    if (this->op != AND_OP && this->op != OR_OP) {
      try {
        left = OperatorName(leftExpr->eval(record));
      } catch (...) {
        left = leftExpr->evalOp(ir, record);
      }
      try {
        right = OperatorName(rightExpr->eval(record));
      } catch (...) {
        right = rightExpr->evalOp(ir, record);
      }
/*      left = OperatorName(leftExpr->evalOp(ir, record));
      right = OperatorName(rightExpr->evalOp(ir, record));*/
    }
    AssignIR *assign;
    switch (this->op) {
      case ADD:
        assign = new AssignIR(OperatorCode::Add, dest, left, right);
        ir.push_back(assign);
        break;
      case SUB:
        assign = new AssignIR(OperatorCode::Sub, dest, left, right);
        ir.push_back(assign);
        break;
      case MUL:
        assign = new AssignIR(OperatorCode::Mul, dest, left, right);
        ir.push_back(assign);
        break;
      case DIV:
        assign = new AssignIR(OperatorCode::Div, dest, left, right);
        ir.push_back(assign);
        break;
      case MOD:
        assign = new AssignIR(OperatorCode::Mod, dest, left, right);
        ir.push_back(assign);
        break;
      default:
        throw runtime_error("undefined op");
    }
    return dest;
  }

  OperatorName Expression::evalOp(IRList &ir, RecordTable *record) {
    throw std::runtime_error("this type of node cannot be eval");
  }

  OperatorName FunctionCall::evalOp(IRList &ir, RecordTable *record) {
    auto funCall = new FunCallIR("@" + name->name);
    OperatorName dest = OperatorName("%" + std::to_string(record->getID()), Type::Var);
    for (auto i : args->args) {
      auto val = i->evalOp(ir, record);
      funCall->argList.push_back(val);
    }
    ir.push_back(funCall);
    return dest;
  }

  OperatorName Identifier::evalOp(IRList &ir, RecordTable *record) {
    auto varInfo = record->searchVar(this->name);
    auto opName = OperatorName(varInfo->getUseName(), Type::Var);
    return opName;
  }

  //TODO 下标canAssign为false的情况
  OperatorName ArrayIdentifier::evalOp(IRList &ir, RecordTable *record) {
    auto varInfo = record->searchVar(this->name);
    std::vector<int> index;
    for (auto i : this->index)
      index.push_back(i->eval(record));

    OperatorName offset;
    try {
      offset = OperatorName(varInfo->getArrayVal(index), Type::Imm);
    } catch (...) {
    }

    auto dest = OperatorName((record->getFarther() == nullptr ? "@" : "%") + std::to_string(record->getID()), Type::Var);
    auto source = OperatorName(varInfo->arrayName, Type::Var);
    offset = OperatorName(varInfo->getArrayIndex(index), Type::Imm);
    auto load = new LoadIR(dest, source, offset);
    ir.push_back(load);
    return dest;
  }

  OperatorCode BinaryExpression::getRelOpCode(){
    switch (op) {
      case LT:
        return mid::ir::OperatorCode::Jl;
      case LE:
        return mid::ir::OperatorCode::Jle;
      case EQ:
        return mid::ir::OperatorCode::Jeq;
      case NE:
        return mid::ir::OperatorCode::Jne;
      case GT:
        return mid::ir::OperatorCode::Jg;
      case GE:
        return mid::ir::OperatorCode::Jge;
    }
  }
  OperatorCode BinaryExpression::getAntiRelOpCode() {
    switch (op) {
      case LT:
        return mid::ir::OperatorCode::Jge;
      case LE:
        return mid::ir::OperatorCode::Jg;
      case EQ:
        return mid::ir::OperatorCode::Jne;
      case NE:
        return mid::ir::OperatorCode::Jeq;
      case GT:
        return mid::ir::OperatorCode::Jle;
      case GE:
        return mid::ir::OperatorCode::Jl;
    }
  }
}// namespace compiler::front::ast


/*
 * store array when init;
 */

namespace compiler::front::ast {
  void ArrayInitVal::storeArray(ArrayIdentifier *name, IRList &ir, RecordTable *record) {
    int index = 0;
    std::string arrayIRIdent = record->searchVar(name->name)->arrayName;
    auto array = record->searchVar(name->name);
    for (auto i = 0; i < initValList.size(); ++i) {
      //生成storeIR
      auto dest = OperatorName(arrayIRIdent, Type::Var);
      auto store = new StoreIR(dest, initValList[i]->evalOp(ir, record), OperatorName(index++, Type::Imm));
      ir.push_back(store);
      //更新数组元素的def链
      auto varUse = VarRedefChain("", initValList[i]->eval(record), true);
      array->addVarUse(varUse, {i});
    }
  }

  OperatorName ArrayIdentifier::evalIndex(IRList &ir, RecordTable *record) {

  }
}// namespace compiler::front::ast