// Created by wnx on 2021/7/5.
//
#include "front/ast/AstNode.h"
#include "parser.hpp"
#include <assert.h>
#include <iostream>
#include <math.h>

using namespace compiler::mid::ir;
namespace compiler::front::ast {
  void AST::genIR(IRList &ir, RecordTable *record) {
    //注册sysy的库函数
    record->setFunRet({"getint", ElemType::INT});
    record->setFunRet({"getch", ElemType::INT});
    record->setFunRet({"getarray", ElemType::INT});
    record->setFunRet({"putint", ElemType::VOID});
    record->setFunRet({"putch", ElemType::VOID});
    record->setFunRet({"putarray", ElemType::VOID});
    record->setFunRet({"_sysy_starttime", ElemType::VOID});
    record->setFunRet({"_sysy_stoptime", ElemType::VOID});

    for (auto i : codeBlock)
      i->genIR(ir, record);
  }

  void Node::genIR(mid::ir::IRList &ir, RecordTable *record) {
    std::cout << "this node can't genIR" << std::endl;
  }

  void VarDeclare::genIR(mid::ir::IRList &ir, RecordTable *record) {
    if (record->getFarther() == nullptr) {
      auto varInfo = new VarInfo("@" + this->name->name, 0, true);
      record->insertVar(name->name, varInfo);
      auto globalData = new GlobalData("@" + this->name->name, 0);
      ir.push_back(globalData);
    } else {
      auto varInfo = new VarInfo("%" + std::to_string(record->getID()), INT32_MIN);
      record->insertVar(name->name, varInfo);
    }
  }

  void VarDeclareWithInit::genIR(mid::ir::IRList &ir, RecordTable *record) {
    if (record->getFarther() == nullptr) {
      string token = "@" + this->name->name;
      try {
        auto global = new GlobalData(token, this->value->eval(record));
        auto varInfo = new VarInfo(token, this->value->eval(record), true);
        record->insertVar(this->name->name, varInfo);
        ir.push_back(global);
      } catch (...) {
      };
    } else {
      string token = "%" + to_string(record->getID());
      auto dest = OperatorName(token);
      auto source = this->value->evalOp(ir, record);
      auto mov = new AssignIR(dest, source);
      ir.push_back(mov);
      if (source.type == Type::Imm) {
        auto varInfo = new VarInfo(token, source.value, true);
        record->insertVar(this->name->name, varInfo);
      } else {
        auto varInfo = new VarInfo(token, 0);
        record->insertVar(this->name->name, varInfo);
      }
    }

    //初始化过程在ir中显式表示
    //    auto assign = new AssignIR(dest, source);
    //    ir.push_back(assign);
  }

  void ConstDeclare::genIR(mid::ir::IRList &ir, RecordTable *record) {
    if (record->getFarther() == nullptr) {
      string token = "@" + this->name->name;
      auto global = new GlobalData(token, this->value->eval(record));
      auto varInfo = new VarInfo(token, this->value->eval(record), true, true);
      ir.push_back(global);
      record->insertVar(this->name->name, varInfo);
    } else {
      auto token = "%" + to_string(record->getID());
      auto source = this->value->evalOp(ir, record);
      if (source.type == Type::Imm) {
        auto varInfo = new VarInfo(token, source.value, true, false);
        record->insertVar(this->name->name, varInfo);
      } else {
        auto varInfo = new VarInfo(token, 0, false, true);
        record->insertVar(this->name->name, varInfo);
      }
    }
  }

  void ArrayDeclare::genIR(mid::ir::IRList &ir, RecordTable *record) {
    if (record->getFarther() == nullptr) {
      string token = "&@" + this->name->name;
      vector<int> val, shape;
      int size = 1;
      for (auto i : this->arrayName->index) {
        shape.push_back(i->eval(record));
        size *= i->eval(record);
      };
      val.resize(size, 0);
      auto global = new GlobalData(token, val);
      ir.push_back(global);
      auto varInfo = new VarInfo(token, shape, val, true);
      record->insertVar(this->arrayName->name, varInfo);
    } else {
      auto array = new ArrayDeclareWithInit(this->arrayName, this->initVal);
      array->genIR(ir, record);
    };
    //    string token = record->getFarther() == nullptr ? "@" : "%";
    //    token = token + std::to_string(record->getID());
    //
    //    vector<int> shape;
    //    int size = 1;
    //    for (auto i : arrayName->index) {
    //      shape.push_back(i->eval(record));
    //      size *= i->eval(record);
    //    };
    //
    //    //数组声明时的内存分配
    //    auto allocaIR = new AllocaIR(token, size);
    //    ir.push_back(allocaIR);
    //
    //    vector<int> value;
    //    value.resize(size, INT32_MIN);
    //    auto varInfo = new VarInfo(token, shape, value);
    //    record->insertVar(arrayName->name, varInfo);
  }

  void ConstArray::genIR(IRList &ir, RecordTable *record) {
    if (record->getFarther() == nullptr) {
      string token = "&@" + this->arrayName->name;
      vector<int> shape, val;
      int size = 1;
      for (auto i : this->arrayName->index) {
        size *= i->eval(record);
        shape.push_back(i->eval(record));
      }
      for (auto i : this->initVal->initValList)
        val.push_back(i->eval(record));
      auto varInfo = new VarInfo(token, shape, val, true, false);
      auto global = new GlobalData(token, val);
      ir.push_back(global);
      record->insertVar(this->arrayName->name, varInfo);
    } else {
      string token = "%" + std::to_string(record->getID());
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

      auto varInfo = new VarInfo(token, shape, value, true, true);
      record->insertVar(arrayName->name, varInfo);

      initVal->storeArray(this->arrayName, ir, record);
    }
  }

  void ArrayDeclareWithInit::genIR(mid::ir::IRList &ir, RecordTable *record) {

    if (record->getFarther() == nullptr) {
      string token = "&@" + this->arrayName->name;
      vector<int> shape, val;
      int size = 1;
      for (auto i : this->arrayName->index) {
        size *= i->eval(record);
        shape.push_back(i->eval(record));
      }
      for (auto i : this->initVal->initValList)
        val.push_back(i->eval(record));
      auto varInfo = new VarInfo(token, shape, val);
      auto global = new GlobalData(token, val);
      ir.push_back(global);
      record->insertVar(this->arrayName->name, varInfo);
    } else {
      string token = "%" + std::to_string(record->getID());
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
  }

  void DeclareStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
    for (auto i : declareList)
      i->genIR(ir, record);
  }

  void AssignStmt::genIR(mid::ir::IRList &ir, RecordTable *record) {
    auto var = record->searchVar(this->name->name);
    if (var->isArray) {
      auto source = rightExpr->evalOp(ir, record);

      dynamic_cast<ArrayIdentifier *>(this->name)->storeRuntime(ir, record, source);

      //尝试计算下标
      std::vector<int> index;
      try {
        for (auto i : name->getIndex())
          index.push_back(i->eval(record));
      } catch (...) {
      }
      VarRedefChain varUse;

      if (!index.empty()) {
        try {
          varUse = VarRedefChain("", this->rightExpr->eval(record), true);
          var->addVarUse(varUse, index);
        } catch (...) {
          varUse = VarRedefChain("", 0, false);
          var->addVarUse(varUse, index);
        }
      }

    } else {
      auto assign = new AssignIR();
      assign->source1 = rightExpr->evalOp(ir, record);
      assign->operatorCode = OperatorCode::Mov;
      assign->dest = OperatorName("%" + to_string(record->getID()));
      assign->dest.defName = this->name->name;
      ir.push_back(assign);

      //更新符号表
      VarRedefChain varUse;
      if (!record->isInLoop()) {
        try {
          varUse = VarRedefChain(assign->dest.name, rightExpr->eval(record), true);
          //          var->addVarUse(varUse);
        } catch (...) {
          varUse = VarRedefChain(assign->dest.name, 0);
          //          var->addVarUse(varUse);
        }
      }
      varUse = VarRedefChain(assign->dest.name, 0);
      var->addVarUse(varUse);
    }
  }

  //  void AssignStmt::genIR(mid::ir::IRList &ir, RecordTable *record) {
  //    auto var = record->searchVar(this->name->name);
  //    if (var->isArray) {
  //      auto source = rightExpr->evalOp(ir, record);
  //
  //      dynamic_cast<ArrayIdentifier *>(this->name)->storeRuntime(ir, record, source);
  //
  //      //尝试计算下标
  //      std::vector<int> index;
  //      try {
  //        for (auto i : name->getIndex())
  //          index.push_back(i->eval(record));
  //      } catch (...) {
  //      }
  //      VarRedefChain varUse;
  //
  //      if (!index.empty()) {
  //        try {
  //          varUse = VarRedefChain("", this->rightExpr->eval(record), true);
  //          var->addVarUse(varUse, index);
  //        } catch (...) {
  //          varUse = VarRedefChain("", 0, false);
  //          var->addVarUse(varUse, index);
  //        }
  //      }
  //
  //    } else {
  //      auto assign = new AssignIR();
  //      assign->source1 = rightExpr->evalOp(ir, record);
  //      assign->operatorCode = OperatorCode::Mov;
  //      assign->dest = OperatorName("%" + to_string(record->getID()));
  //      assign->dest.defName = this->name->name;
  //      ir.push_back(assign);
  //
  //      //更新符号表
  //      VarRedefChain varUse;
  //      if (!record->isInLoop()) {
  //        try {
  //          varUse = VarRedefChain(assign->dest.name, rightExpr->eval(record), true);
  //          var->addVarUse(varUse);
  //        } catch (...) {
  //          varUse = VarRedefChain(assign->dest.name, 0);
  //          var->addVarUse(varUse);
  //        }
  //      }
  //    }
  //  }

  //完成
  void FunctionDefine::genIR(mid::ir::IRList &ir, RecordTable *record) {

    ElemType retType = this->retType == INT ? ElemType::INT : ElemType::VOID;

    record->setFunRet({this->name->name, retType});

    //    创建函数作用域的符号表
    auto funcdef = new FunDefIR(retType, name->name);
    auto newTable = new RecordTable(record);

    //保存参数表到fundefir
    for (auto i : args->args) {
      auto ident = dynamic_cast<ArrayIdentifier *>(i->name);
      if (ident) {
        auto opName = OperatorName("&" + i->name->name, Type::Var);
        funcdef->argList.push_back(opName);
      } else {
        auto opName = OperatorName(i->name->name, Type::Var);
        funcdef->argList.push_back(opName);
      }
    }
    //把函数的入参存入函数符号表
    args->genIR(funcdef->funcBody, newTable);

    //对函数体生成ir
    body->genIR(funcdef->funcBody, newTable);

    RetIR *ret;
    if (this->retType == INT) {
      ret = new RetIR(OperatorName(0, Type::Imm));
    } else
      ret = new RetIR(OperatorName(Type::Void));
    funcdef->funcBody.push_back(ret);

    ir.push_back(funcdef);
  }

  //完成
  void FunctionDefArgList::genIR(mid::ir::IRList &ir, RecordTable *record) {
    for (int i = 0; i < this->args.size(); ++i) {
      auto ident = dynamic_cast<ArrayIdentifier *>(args[i]->name);
      if (ident) {
        vector<int> shape;
        int size = 1;
        for (auto i : ident->index) {
          shape.push_back(i->eval(record));
          size *= i->eval(record);
        }

        auto token = "%" + to_string(record->getID());
        auto dest = OperatorName(token);
        auto source = OperatorName("$" + to_string(i));
        auto assign = new AssignIR(dest, source);

        vector<int> val;
        val.resize(1, 0);

        auto var = new VarInfo(ident->name, shape, val, false);

        record->insertVar(ident->name, var);
        ir.push_back(assign);
      } else {
        auto token = "%" + to_string(record->getID());
        auto dest = OperatorName(token);
        auto source = OperatorName("$" + to_string(i));
        auto assign = new AssignIR(dest, source);
        ir.push_back(assign);
        auto var = new VarInfo(this->args[i]->name->name, INT32_MIN);
        record->insertVar(this->args[i]->name->name, var);
      }
    }
  }

  //fine
  void FunctionDefArg::genIR(mid::ir::IRList &ir, RecordTable *record) {
    auto varInfo = new VarInfo(name->name, INT32_MIN);
    auto ident = dynamic_cast<ArrayIdentifier *>(this);
    if (ident) {

    } else {
      string token = "%" + to_string(record->getID());
      auto dest = VarInfo(token, INT32_MIN);
      auto assign = new AssignIR();
    }
    record->insertVar(name->name, varInfo);
  }

  //完成
  void Block::genIR(mid::ir::IRList &ir, RecordTable *record) {
    for (auto item : blockItem)
      item->genIR(ir, record);
  }

  void IfStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
    auto newTable = new RecordTable(record);
    string id  = std::to_string(newTable->getID());
    auto ifLabel = new LabelIR(".L_THEN_" + id);
    auto elseLabel = new LabelIR(".L_ELSE_" + id);
    auto endLabel = new LabelIR(".L_IF_END_" + id);
    cond->ConditionAnalysis(ir, newTable, ifLabel, elseLabel, true);
    ir.push_back(ifLabel);
    BlockIR *trueIR = new BlockIR();
    BlockIR *falseIR = new BlockIR();
    trueBlock->genIR(trueIR->block, newTable);
    elseBlock->genIR(falseIR->block, newTable);
    std::unordered_map<std::string, std::string> phiRollBack;
    IRList phiRollBackMov;
    for (auto it = trueIR->block.begin(); it != trueIR->block.end(); it++) {
      auto pIR = dynamic_cast<AssignIR *>(*it);
      VarInfo *tmp;
      if (pIR) {
        try {
          tmp = record->searchVar(pIR->dest.defName);
        } catch (...) {
          continue;
        }
        try {
          phiRollBack.at(pIR->dest.defName);
        } catch (...) {
          auto &varDefNameList = tmp->varUse[0];
          std::string name;
          for (auto it = varDefNameList.begin(); it != varDefNameList.end(); it++) {
            if (it->defName == pIR->dest.name) {
              it++;
              if (it == varDefNameList.end()) {
                throw runtime_error("???");
              }
              name = it->defName;
              break;
            }
          }
          //找到在进入块前变量使用的最后一个defName
          phiRollBack[pIR->dest.defName] = name;
          /*if (name != "")
          {
            phiRollBackMov.push_back(new AssignIR(pIR->dest.name,name));
          }*/
        }
      }
    }
    std::unordered_map<std::string, std::string> phiUpdate;
    for (auto it = falseIR->block.rbegin(); it != falseIR->block.rend(); it++) {
      auto temp = *it;
      auto pIR = dynamic_cast<AssignIR *>(*it);
      if (pIR) {
        try {
          auto tmp = record->searchVar(pIR->dest.defName);
        } catch (...) {
          continue;
        }
        try {
          auto tmp = phiUpdate.at(pIR->dest.defName);
        } catch (out_of_range) {
          phiUpdate[pIR->dest.defName] = pIR->dest.name;
        }
      }
    }
    std::list<IR *> phiUpdateMov;
    for (auto it = trueIR->block.rbegin(); it != trueIR->block.rend(); it++) {
      auto temp = *it;
      auto pIR = dynamic_cast<AssignIR *>(*it);
      if (pIR) {
        try {
          auto tmp = phiUpdate.at(pIR->dest.defName);
          phiUpdateMov.emplace_back(new PhiIR(tmp, pIR->dest.name));
          phiUpdate.erase(pIR->dest.defName);
        } catch (out_of_range) {
        }
        try {
          auto tmp = phiRollBack.at(pIR->dest.defName);
          phiRollBackMov.emplace_back(new PhiIR(pIR->dest.name, tmp));
          phiRollBack.erase(pIR->dest.defName);
        } catch (out_of_range) {
        }
      }
    }
    for (auto item : trueIR->block) {
      ir.push_back(item);
    }
    for (auto item : phiUpdateMov) {
      ir.push_back(item);
    }
    ir.push_back(new JmpIR(OperatorCode::Jmp, endLabel));
    ir.push_back(elseLabel);
    for (auto item : phiRollBackMov) {
      ir.push_back(item);
    }
    for (auto item : falseIR->block) {
      ir.push_back(item);
    }
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
    ir.emplace_back(new JmpIR(OperatorCode::Jmp, RecordTable::getTopLabel().second));
  }

  void WhileStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
    auto newTable = new RecordTable(record);
    newTable->setInLoop(true);

    int num = newTable->getID();
    auto loopLabel = new LabelIR(".L_Loop" + std::to_string(num));
    auto endLoopLabel = new LabelIR(".L_EndLoop" + std::to_string(num));
    auto testLabel = new LabelIR(".L_Test" + std::to_string(num));
    auto continueLabel = new LabelIR(".L_Continue"+std::to_string(num));
    RecordTable::pushLabelPair(testLabel, endLoopLabel);
    IRList condIr, loopIR;
    cond->ConditionAnalysis(condIr, newTable, loopLabel, endLoopLabel, true);
    loopBlock->genIR(loopIR, newTable);


    IRList phiLoopMov, AntiPhiLoopMov;
    std::unordered_map<std::string, std::string> phiLoop;
    for (auto it = loopIR.begin(); it != loopIR.end(); it++) {
      auto pIR = dynamic_cast<AssignIR *>(*it);
      VarInfo *tmp;
      if (pIR) {
        try {
          tmp = record->searchVar(pIR->dest.defName);
        } catch (...) {
          continue;
        }
        try {
          phiLoop.at(pIR->dest.defName);
        } catch (...) {
          auto &varDefNameList = tmp->varUse[0];
          std::string name;
          for (auto it = varDefNameList.begin(); it != varDefNameList.end(); it++) {
            if (it->defName == pIR->dest.name) {
              it++;
              if (it == varDefNameList.end()) {
                throw runtime_error("???");
              }
              name = it->defName;
              break;
            }
          }
          phiLoop[pIR->dest.defName] = name;
        }
      }
    }
    for (auto it = loopIR.rbegin(); it != loopIR.rend(); it++) {
      auto pIR = dynamic_cast<AssignIR *>(*it);
      if (pIR) {
        try {
          auto tmp = phiLoop.at(pIR->dest.defName);
          phiLoopMov.emplace_back(new AssignIR(pIR->dest.name, tmp,OperatorCode::PhiMov));
          AntiPhiLoopMov.emplace_back(new AssignIR(tmp, pIR->dest.name,OperatorCode::PhiMov));
          phiLoop.erase(pIR->dest.defName);
        } catch (out_of_range) {
        }
      }
    }
    ir.emplace_back(new JmpIR(mid::ir::OperatorCode::Jmp,testLabel));
    ir.push_back(loopLabel);
    for (auto &item : loopIR) {
      ir.push_back(item);
    }
    ir.push_back(continueLabel);
    for (auto &item : AntiPhiLoopMov) {
      ir.push_back(item);
    }
    ir.push_back(testLabel);
    for (auto &item : condIr) {
      ir.push_back(item);
    }
    //ir.push_back(testLabel);

    //ir.emplace_back(new JmpIR(OperatorCode::Jmp,loopLabel));

    ir.push_back(endLoopLabel);
    for (auto &item : phiLoopMov) {
      ir.push_back(item);
    }
    RecordTable::popLabelPair();

    newTable->setInLoop(false);
  }

  void ContinueStatement::genIR(mid::ir::IRList &ir, RecordTable *record) {
    ir.emplace_back(new JmpIR(OperatorCode::Jmp, RecordTable::getTopLabel().first));
  }

  void FunctionCall::genIR(IRList &ir, RecordTable *record) {
    this->evalOp(ir, record);
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

  int Identifier::eval(RecordTable *record) {
    auto varInfo = record->searchVar(this->name);
    if (varInfo->canAssign())
      return varInfo->getVal();
    throw std::runtime_error("it cannot assign");
  }

  int ArrayIdentifier::eval(RecordTable *record) {
    auto varInfo = record->searchVar(name);
    std::vector<int> index;
    for (auto i : this->index)
      index.push_back(i->eval(record));
    if (varInfo->isConst && varInfo->canAssign(index))
      return varInfo->getArrayVal(index);
    throw std::runtime_error("it cannot assign");
  }

  int BinaryExpression::eval(RecordTable *record) {
    switch (this->op) {
      case ADD:
        return rightExpr->eval(record) + leftExpr->eval(record);
      case SUB:
        return leftExpr->eval(record) - rightExpr->eval(record);
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
    return expr.back()->eval(record);
  }
  /* *
   *
   * condition Analysis
   *
   * */
  void Expression::ConditionAnalysis(IRList &ir, RecordTable *record, LabelIR *ifLabel, LabelIR *elLabel, bool trueJmp) {
    try {
      if (record->isInLoop()){
        throw runtime_error("");
      }
      if (this->eval(record)) {
        ir.emplace_back(new JmpIR(OperatorCode::Jmp, ifLabel));
      } else {
        ir.emplace_back(new JmpIR(OperatorCode::Jmp, elLabel));
      }
    } catch (...) {
      try {
        this->evalOp(ir, record);
        ir.emplace_back(new JmpIR(OperatorCode::Jne, ifLabel));
      } catch (...) {
        if (trueJmp) {
          OperatorName dest = OperatorName((record->getFarther() == nullptr ? "@" : "%") + to_string(record->getID())), left, right;
          auto *assign = new AssignIR(OperatorCode::Cmp, dest, left, right);
          ir.push_back(assign);
          ir.emplace_back((new JmpIR(static_cast<BinaryExpression *>(this)->getRelOpCode(), ifLabel)));
          ir.emplace_back((new JmpIR(static_cast<BinaryExpression *>(this)->getRelOpCode(), elLabel)));
        } else {
          //ir.emplace_back((new JmpIR (static_cast<BinaryExpression*>(this)->getAntiRelOpCode(),)));
        }
      }
    }
  }
  void BinaryExpression::ConditionAnalysis(IRList &ir, RecordTable *record, LabelIR *ifLabel, LabelIR *elLabel, bool trueJmp) {


    /*
     * 关键问题在于:
     * 最终的目的是能够利用好屏蔽效果，即每次调用的目标应该是跳转到本表达式的结束label
     * 优化的策略则是利用屏蔽策略，能够屏蔽更多的运算，跳转到更远的地方
     *
     * 当前的简单策略，将每一个表达式拆分成if else两种结果，根据表达式含义推断跳转的位置
    */
    if (this->op == AND_OP) {
      auto *innerLabel = new LabelIR(".L" + std::to_string(record->getID()));
      leftExpr->ConditionAnalysis(ir, record, innerLabel, elLabel, true);
      ir.push_back(innerLabel);
      rightExpr->ConditionAnalysis(ir, record, ifLabel, elLabel, true);
    } else if (this->op == OR_OP) {
      auto *innerLabel = new LabelIR(".L" + std::to_string(record->getID()));
      leftExpr->ConditionAnalysis(ir, record, ifLabel, innerLabel, true);
      ir.push_back(innerLabel);
      rightExpr->ConditionAnalysis(ir, record, ifLabel, elLabel, true);
    } else {
      try {
        if (record->isInLoop()){
          throw runtime_error("");
        }
        if (this->eval(record)) {
          ir.emplace_back(new JmpIR(OperatorCode::Jmp, ifLabel));
        } else {
          ir.emplace_back(new JmpIR(OperatorCode::Jmp, elLabel));
        }
      } catch (...) {
        try {
          this->evalOp(ir, record);
          ir.emplace_back(new JmpIR(OperatorCode::Jne, ifLabel));
          ir.emplace_back(new JmpIR(OperatorCode::Jeq, elLabel));
        } catch (...) {
          if (trueJmp) {
            OperatorName dest = OperatorName((record->getFarther() == nullptr ? "@" : "%") + to_string(record->getID()));
            auto left = leftExpr->evalOp(ir, record);
            auto right = rightExpr->evalOp(ir, record);
            auto *assign = new AssignIR(OperatorCode::Cmp, dest, left, right);
            ir.push_back(assign);
            ir.emplace_back((new JmpIR(static_cast<BinaryExpression *>(this)->getRelOpCode(), ifLabel)));
            ir.emplace_back((new JmpIR(static_cast<BinaryExpression *>(this)->getAntiRelOpCode(), elLabel)));
          } else {
            //没有else
            //ir.emplace_back((new JmpIR (static_cast<BinaryExpression*>(this)->getAntiRelOpCode(),elLabel)));
          }
        }
      }
    }
  }

}// namespace compiler::front::ast

bool is2Power(int x) {
  double ans = log2(x);
  return (int) ans == ans;
}

int log2(int x) {
  int times = -1;
  while (x > 0) {
    x /= 2;
    times++;
  }
  return times;
}

/*
 *evalOp
 */
namespace compiler::front::ast {

  OperatorName NumberExpression::evalOp(IRList &ir, RecordTable *record) {
    return {this->value, Type::Imm};
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
      if (!record->isInLoop()) {
        int tmp = this->eval(record);
        auto opName = OperatorName("", Type::Imm);
        opName.value = tmp;
        return opName;
      }
    } catch (...) {
    }
    OperatorName dest = OperatorName("%" + to_string(record->getID())), left, right;

    int RelOP[8] = {AND_OP, OR_OP, LT, LE, GE, GT, NE, EQ};
    for (int i = 0; i < 8; i++) {
      if (this->op == RelOP[i])
        throw runtime_error("rel op");
    }
    //排除所有的逻辑运算符
    try {
      if (!record->isInLoop()) { left = OperatorName(leftExpr->eval(record), Type::Imm); }
    } catch (...) {
    }
    left = leftExpr->evalOp(ir, record);
    try {
      if (!record->isInLoop()) { right = OperatorName(rightExpr->eval(record), Type::Imm); }
    } catch (...) {
    }
    right = rightExpr->evalOp(ir, record);
    /*      left = OperatorName(leftExpr->evalOp(ir, record));
      right = OperatorName(rightExpr->evalOp(ir, record));*/

    AssignIR *assign = nullptr;
    switch (this->op) {
      case ADD:
        assign = new AssignIR(OperatorCode::Add, dest, left, right);
        ir.push_back(assign);
        break;
      case SUB:
        assign = new AssignIR(OperatorCode::Sub, dest, left, right);
        ir.push_back(assign);
        break;
      case MUL: {
        auto leftNum = dynamic_cast<NumberExpression *>(leftExpr);
        auto rightNum = dynamic_cast<NumberExpression *>(rightExpr);
        if (leftNum && is2Power(leftNum->value)) {
          assign = new AssignIR(OperatorCode::Sal, dest, right, OperatorName(log2(leftNum->value), Type::Imm));
        } else if (rightNum && is2Power(rightNum->value))
          assign = new AssignIR(OperatorCode::Sal, dest, left, OperatorName(log2(rightNum->value), Type::Imm));
        else
          assign = new AssignIR(OperatorCode::Mul, dest, left, right);
        ir.push_back(assign);
        break;
      }
      case DIV: {
        auto rightNum = dynamic_cast<NumberExpression *>(rightExpr);
        if (rightNum && is2Power(rightNum->value))
          assign = new AssignIR(OperatorCode::Sar, dest, left, OperatorName(log2(rightNum->value), Type::Imm));
        else
          assign = new AssignIR(OperatorCode::Div, dest, left, right);
        ir.push_back(assign);
        break;
      }
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
  void Expression::genIR(IRList &ir, RecordTable *record) {
    Node::genIR(ir, record);
  }

  OperatorName FunctionCall::evalOp(IRList &ir, RecordTable *record) {
    ElemType retType = record->getFunRet(this->name->name);

    auto funCall = new FunCallIR("@" + name->name);
    funCall->retType = retType;
    auto tmpType = retType == ElemType::INT ? Type::Var : Type::Void;
    OperatorName dest = OperatorName("%" + std::to_string(record->getID()), tmpType);
    funCall->retOp = dest;
    for (auto i : args->args) {
      auto ident = dynamic_cast<Identifier *>(i);
      if (ident) {
        auto varInfo = record->searchVar(ident->name);
        if (varInfo->isArray) {
          auto arrayIdent = dynamic_cast<ArrayIdentifier *>(ident);
          if (arrayIdent) {
            //传入的是数组的子数组
            if (arrayIdent->index.size() < varInfo->shape.size()) {
              auto subArray = arrayIdent->getSubArray(ir, record, varInfo->shape.size());
              funCall->argList.push_back(subArray);
            } else {
              auto opName = arrayIdent->evalOp(ir, record);
              funCall->argList.push_back(opName);
            }
          } else {
            auto opName = OperatorName(varInfo->arrayName);
            funCall->argList.push_back(opName);
          }

        } else {
          auto opName = ident->evalOp(ir, record);
          funCall->argList.push_back(opName);
        }
      } else {
        auto val = i->evalOp(ir, record);
        funCall->argList.push_back(val);
      }

      //函数调用时,显式处理函数参数
      //      auto arg = AssignIR(OperatorName("$" + to_string(counter++)), );
    }
    ir.push_back(funCall);
    return dest;
  }

  OperatorName Identifier::evalOp(IRList &ir, RecordTable *record) {
    try {
      if (!record->isInLoop()) {
        auto opName = OperatorName(this->eval(record), Type::Imm);
        return opName;
      }
    } catch (...) {
    }
    auto varInfo = record->searchVar(this->name);
    auto opName = OperatorName(varInfo->getUseName(), Type::Var);
    return opName;
  }


  OperatorName ArrayIdentifier::evalOp(IRList &ir, RecordTable *record) {
    auto varInfo = record->searchVar(this->name);

    auto dest = OperatorName("%" + std::to_string(record->getID()), Type::Var);
    auto source = OperatorName(varInfo->arrayName);
    try {
      std::vector<int> index;
      for (auto i : this->index)
        index.push_back(i->eval(record));

      auto offset = OperatorName(varInfo->getArrayIndex(index), Type::Imm);
      auto load = new LoadIR(dest, source, offset);
      ir.push_back(load);
      return dest;
    } catch (...) {
    }

    if (this->index.size() == 1) {
      auto load = new LoadIR(dest, source, this->index[0]->evalOp(ir, record));
      ir.push_back(load);
    } else {
      //维度大于一的情况下,需要从最低维开始计算偏移量
      auto tmpSize = OperatorName("%" + to_string(record->getID()));
      //最低维的目标变量(ir)
      auto indexContainer = OperatorName("%" + to_string(record->getID()));
      //最低维的大小,是一个立即数
      auto vSize = OperatorName("", Type::Imm);
      vSize.value = varInfo->shape[varInfo->shape.size() - 1];
      //把最后一个维度的值赋值给一个寄存器(mov
      auto assign = new AssignIR(tmpSize, vSize);
      ir.push_back(assign);

      for (int i = this->index.size() - 1; i >= 0; i--) {
        if (i == this->index.size() - 1) {
          auto mov = new AssignIR(indexContainer, this->index[this->index.size() - 1]->evalOp(ir, record));
          ir.push_back(mov);
        } else {
          auto tmp = OperatorName("%" + to_string(record->getID()));
          auto tmp2 = OperatorName("%" + to_string(record->getID()));
          auto t = new AssignIR(OperatorCode::Mul, tmp, tmpSize, this->index[i]->evalOp(ir, record));
          ir.push_back(t);

          auto add = new AssignIR(OperatorCode::Add, tmp2, indexContainer, tmp);
          ir.push_back(add);
          //已经处理的偏移量
          indexContainer = tmp2;

          //更新尺寸
          if (i != 0) {
            auto tmp = OperatorName("%" + to_string(record->getID()));
            auto mul = new AssignIR(OperatorCode::Mul, tmp, tmpSize, this->index[i]->evalOp(ir, record));
            ir.push_back(mul);
            tmpSize = tmp;
          }
        }
      }

      auto load = new LoadIR(dest, source, indexContainer);
      ir.push_back(load);
    }

    return dest;
  }

  OperatorCode BinaryExpression::getRelOpCode() {
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
      auto tmp = initValList[i]->evalOp(ir, record);
      VarRedefChain varUse;
      if (tmp.type == Type::Imm)
        varUse = VarRedefChain("", tmp.value, true);
      else
        varUse = VarRedefChain("", INT32_MIN);
      array->addVarUse(varUse, {i});
    }
  }

  OperatorName ArrayIdentifier::getSubArray(IRList &ir, RecordTable *record, int arraySize) {
    auto varInfo = record->searchVar(this->name);

    auto dest = OperatorName("&%" + to_string(record->getID()));
    try {
      vector<int> subOffset;
      for (auto i : this->index)
        subOffset.push_back(i->eval(record));
      for (int i = this->index.size(); i < arraySize; ++i)
        subOffset.push_back(0);

      auto offset = OperatorName(varInfo->getArrayIndex(subOffset), Type::Imm);
      auto array = OperatorName(varInfo->arrayName);

      //计算subarray首地址相对于数组基址的偏移量
      auto add = new AssignIR(OperatorCode::Add, dest, array, offset);
      ir.push_back(add);
      return dest;
    } catch (...) {
    }
  };

  void ArrayIdentifier::storeRuntime(IRList &ir, RecordTable *record, OperatorName source) {
    auto var = record->searchVar(this->name);
    assert(this->index.size() == var->shape.size());
    try {
      int index = 0, temp = 1;
      for (int i = this->index.size() - 1; i >= 0; --i) {
        index += temp * this->index[i]->eval(record);
        temp *= var->shape[i];
      }
      auto offset = OperatorName(index, Type::Imm);
      auto dest = OperatorName(var->arrayName);
      auto store = new StoreIR(dest, source, offset);
      ir.push_back(store);
    } catch (...) {

      auto dest = OperatorName(var->arrayName);
      if (this->index.size() == 1) {
        auto store = new StoreIR(dest, source, this->index[0]->evalOp(ir, record));
        ir.push_back(store);
      } else {
        //维度大于一的情况下,需要从最低维开始计算偏移量
        auto tmpSize = OperatorName("%" + to_string(record->getID()));
        //最低维的目标变量(ir)
        auto indexContainer = OperatorName("%" + to_string(record->getID()));
        //最低维的大小,是一个立即数
        auto vSize = OperatorName("", Type::Imm);
        vSize.value = var->shape[var->shape.size() - 1];
        //把最后一个维度的值赋值给一个寄存器(mov
        auto assign = new AssignIR(tmpSize, vSize);
        ir.push_back(assign);

        for (int i = this->index.size() - 1; i >= 0; i--) {
          if (i == this->index.size() - 1) {
            auto mov = new AssignIR(indexContainer, this->index[this->index.size() - 1]->evalOp(ir, record));
            ir.push_back(mov);
          } else {
            auto tmp = OperatorName("%" + to_string(record->getID()));
            auto tmp2 = OperatorName("%" + to_string(record->getID()));
            auto t = new AssignIR(OperatorCode::Mul, tmp, tmpSize, this->index[i]->evalOp(ir, record));
            ir.push_back(t);

            auto add = new AssignIR(OperatorCode::Add, tmp2, indexContainer, tmp);
            ir.push_back(add);
            //已经处理的偏移量
            indexContainer = tmp2;

            //更新尺寸
            if (i != 0) {
              auto tmp = OperatorName("%" + to_string(record->getID()));
              auto mul = new AssignIR(OperatorCode::Mul, tmp, tmpSize, this->index[i]->evalOp(ir, record));
              ir.push_back(mul);
              tmpSize = tmp;
            }
          }
        }

        auto store = new StoreIR(dest, source, indexContainer);
        ir.push_back(store);
      }
    }
  }
}// namespace compiler::front::ast