//
// Created by wnx on 2021/7/5.
//

#include "RecordTable.h"
#include <iostream>
#include <vector>

namespace compiler::mid::ir {
  VarInfo::VarInfo(std::string name, std::vector<int> &inShape, std::vector<int> &inValue, bool isConst) {
    this->name = name;
    this->shape = inShape;
    this->value = inValue;
    this->isConst = isConst;
    this->isArray = true;
  }
  VarInfo::VarInfo(std::string name, int value, bool isConst)
      : name(name), isConst(isConst), isArray(false) {
    this->value = {value};
  }
  VarInfo::VarInfo(std::string name, std::vector<int> &inValue, std::initializer_list<int> inShape, bool isConst) {
    this->name = name;
    this->shape = inShape;
    this->value = inValue;
    this->isConst = isConst;
    this->isArray = true;
  }

  void RecordTable::insertVar(std::string name, VarInfo *v) {
    varTable.insert({name, v});
  }

  void RecordTable::setArrayVal(std::string name, std::vector<int> index, int val) {
    int tmp = 1;
    int sum = 0;
    auto varInfo = this->searchVar(name);
    for (int i = index.size() - 1; i >= 0; i--) {
      sum += tmp * index[i];
      tmp *= varInfo->shape[i];
    }
    if (sum < varInfo->value.size())
      varInfo->value[sum] = val;
    throw std::runtime_error("out of index");
  }

  void RecordTable::setVal(std::string name, int val) {
    auto var = this->searchVar(name);
    if (var != nullptr)
      var->value[0] = val;
    throw std::runtime_error("no var name " + name);
  }

  VarInfo *RecordTable::searchVar(std::string name) {
    auto tmp = varTable.find(name);
    if (tmp != varTable.end())
      return varTable[name];//
    else if (father != nullptr)
      return father->searchVar(name);
    throw std::out_of_range("no var name " + name);
  }

  int VarInfo::getArrayVal(std::vector<int> index) {
    int tmp = 1;
    int sum = 0;
    for (int i = index.size() - 1; i >= 0; i--) {
      sum += tmp * index[i];
      tmp *= shape[i];
    }
    if (sum < value.size())
      return this->value[sum];
    throw std::runtime_error("out of index");
  }

  int VarInfo::getArrayIndex(std::vector<int> index) {
    int tmp = 1;
    int sum = 0;
    for (int i = index.size() - 1; i >= 0; i--) {
      sum += tmp * index[i];
      tmp *= shape[i];
    }
    if (sum < value.size())
      return sum;
    throw std::runtime_error("out of index");
  }
}// namespace compiler::mid::ir