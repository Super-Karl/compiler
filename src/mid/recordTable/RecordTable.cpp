//
// Created by wnx on 2021/7/5.
//

#include "RecordTable.h"
#include <iostream>
#include <vector>

namespace compiler::mid::ir {
  VarInfo::VarInfo(std::string name,std::vector<int>& inShape,std::vector<int>& inValue,bool isConst = false){
    this->name = name;
    this->shape = inShape;
    this->value = inValue;
    this->isConst = isConst;
    this->isArray = true;
  }
  VarInfo::VarInfo(std::string name, int value, bool isConst = false) 
    : name(name),isConst(isConst),isArray(false) {
      this->value={value};
  }
  VarInfo::VarInfo(std::string name,std::vector<int>& inValue,std::initializer_list<int> inShape,bool isConst = false){
    this->name = name;
    this->shape = inShape;
    this->value = inValue;
    this->isConst = isConst;
    this->isArray = true;
  }

  RecordTable &RecordTable::insertVar(std::string name, VarInfo v) {
    varTable[name] = v;
  }
  VarInfo &RecordTable::searchVar(std::string name) {
    auto tmp = varTable.find(name);
    if (tmp != varTable.end())
      return varTable[name];//
    throw std::out_of_range("no var name " + name);
  }
}// namespace compiler::mid::ir