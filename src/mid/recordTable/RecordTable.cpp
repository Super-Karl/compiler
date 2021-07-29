//
// Created by wnx on 2021/7/5.
//

#include "RecordTable.h"
#include <iostream>
#include <stack>
#include <vector>

namespace compiler::mid::ir {
  unsigned int RecordTable::id = 0;
  VarInfo *RecordTable::searchVar(std::string name) {
    auto tmp = varTable.find(name);
    if (tmp != varTable.end())
      return varTable[name];//
    else if (father != nullptr)
      return father->searchVar(name);
    throw std::out_of_range("no var name " + name);
  }

  void RecordTable::insertVar(std::string name, VarInfo *v) {
    varTable.insert({name, v});
  }

  bool RecordTable::canExprAssign(std::string op1, std::string op2, std::vector<int> index1, std::vector<int> index2) {
    auto var1 = this->searchVar(op1);
    auto var2 = this->searchVar(op2);
    return var1->canAssign(index1) && var2->canAssign(index2);
  }

  /*
   *VarInfo
   */

  VarInfo::VarInfo(std::string name, std::vector<int> &inShape, std::vector<int> &inValue, bool canAssign, bool isConst) {

    this->arrayName = name;
    this->isConst = isConst;
    this->isArray = canAssign;
    this->shape = inShape;
    varUse.resize(inValue.size());
    for (auto i = 0; i < inValue.size(); i++) {
      auto var = VarRedefChain("", inValue[i], true);
      varUse[i].push_front(var);
    }
  }

  VarInfo::VarInfo(std::string name, int value, bool canAssign, bool isConst)
      : isConst(isConst), isArray(false), arrayName("") {
    this->varUse.resize(1);
    this->varUse[0].push_front(VarRedefChain(std::move(name), value, canAssign));
  }

  VarInfo::VarInfo(std::string name, std::vector<int> &inValue, std::initializer_list<int> inShape, bool isConst) {
    this->arrayName = std::move(name);
    this->shape = inShape;
    this->isConst = isConst;
    this->isArray = true;
    varUse.resize(inValue.size());
    for (int i = 0; i < inShape.size(); ++i) {
      auto var = VarRedefChain("", inValue[i], true);
      varUse[i].push_front(var);
    }
  }


  int VarInfo::getArrayVal(std::vector<int> index) {
    int tmp = 1;
    int sum = 0;
    for (int i = index.size() - 1; i >= 0; i--) {
      sum += tmp * index[i];
      tmp *= shape[i];
    }
    if (sum < varUse.size())
      return varUse[sum].front().val;
    throw std::runtime_error("out of index");
  }

  int VarInfo::getArrayIndex(std::vector<int> index) {
    int tmp = 1;
    int sum = 0;
    for (int i = index.size() - 1; i >= 0; i--) {
      sum += tmp * index[i];
      tmp *= shape[i];
    }
    if (sum < varUse.size())
      return sum;
    throw std::runtime_error("out of index");
  }

  int VarInfo::getVal() {
    return this->varUse[0].front().val;
  }

  std::string VarInfo::getUseName() {
    return this->varUse[0].front().defName;
  }

  bool VarInfo::canAssign(std::vector<int> index) {
    if (isArray)
      return varUse[this->getArrayIndex(index)].front().canAssign;
    else
      return varUse[0].front().canAssign;
  }

  void VarInfo::addVarUse(VarRedefChain var, std::vector<int> index) {
    if (isArray) {
      this->varUse[getArrayIndex(std::move(index))].push_front(var);
    } else {
      this->varUse[0].push_front(var);
    }
  }

  ElemType RecordTable::getFunRet(std::string funcName) {
    auto tmp = funDecl.find(funcName);
    if (tmp != funDecl.end())
      return funDecl[funcName];
    else if (this->father != nullptr)
      return father->getFunRet(funcName);
  }

  void RecordTable::setFunRet(std::pair<std::string, ElemType> pair) {
    funDecl.insert(pair);
  }
}// namespace compiler::mid::ir

namespace compiler::mid::ir {
  std::stack<std::pair<LabelIR *, LabelIR *>> RecordTable::labelPairs;
  void RecordTable::pushLabelPair(LabelIR *label1, LabelIR *label2) {
    labelPairs.push(std::make_pair(label1, label2));
  }
  void RecordTable::popLabelPair() {
    labelPairs.pop();
  }
  std::pair<LabelIR *, LabelIR *> &RecordTable::getTopLabel() {
    return labelPairs.top();
  }
}// namespace compiler::mid::ir