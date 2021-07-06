//
// Created by wnx on 2021/7/5.
//

#include "RecordTable.h"
#include <iostream>
#include <vector>

namespace compiler::mid::ir {
  void RecordTable::insertVar(std::string name, VarInfo value) {
    std::vector<compiler::mid::ir::VarInfo> table;
    table.back().insert(VarInfo);
    varTable.insert(name, table);
  }

  std::vector<VarInfo &> RecordTable::searchVar(std::string name) {
    auto tmp = varTable.find(name);
    if (tmp != varTable.end())
      return tmp->second;//
    throw std::out_of_range("no var name " + name);
  }
}// namespace compiler::mid::ir