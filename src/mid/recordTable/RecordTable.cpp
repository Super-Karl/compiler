//
// Created by wnx on 2021/7/5.
//

#include "RecordTable.h"
#include <iostream>
#include <vector>

namespace compiler::mid::ir {
  RecordTable& RecordTable::insertVar(std::string name,std::initializer_list<VarInfo> ivl) {
    std::vector<VarInfo> table;
    for (auto &var:ivl){
      table.push_back(var);
    }
    varTable[name] =  table;
  }
  RecordTable& RecordTable::insertVar(std::string name,std::vector<VarInfo> v){
    varTable[name] = v;
  }
  RecordTable& RecordTable::insertVar(std::string name,VarInfo v){
    std::vector<VarInfo>table{v};
    varTable[name]=table;
  }
  std::vector<VarInfo>& RecordTable::searchVar(std::string name) {
    auto tmp = varTable.find(name);
    if (tmp != varTable.end())
      return varTable[name];//
    throw std::out_of_range("no var name " + name);
  }
}// namespace compiler::mid::ir