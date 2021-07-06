//
// Created by wnx on 2021/7/5.
//

#include "RecordTable.h"
#include <iostream>

namespace compiler::mid::ir {
  void RecordTable::insertVar(std::string name, VarInfo value) {
    varTable.insert(name, value);
  }

  VarInfo &RecordTable::searchVar(std::string name) {
    auto tmp = varTable.find(name);
    if (tmp != varTable.end())
      return tmp->second;
    throw std::out_of_range("no var name " + name);
  }
}