//
// Created by wnx on 2021/7/5.
//

#ifndef COMPILER_RECORDTABLE_H
#define COMPILER_RECORDTABLE_H

#include "mid/ir/ir.h"
#include <unordered_map>

namespace compiler::mid::ir {
  class VarInfo {
  public:
    bool isGlobal;
    bool isConst;
    bool isArray;
    int value;
    std::string name;//with identifier @,% 为ssa中rename后的名字

    VarInfo(std::string name, int value, bool isGlobal = false, bool isConst = false) : name(name), value(value),
                                                                                        isGlobal(isGlobal),
                                                                                        isConst(isConst) {}
  };

  //全局的记录表,用来记录sy程序中变量的use,
  /* TODO 没实现φ函数,多分支情况下的ssa变量选取*/
  class RecordTable {
  private:
    std::unordered_map<std::string, std::vector<VarInfo>> varTable;//符号表,变量的vec只有一个值,数组的vector会存储所有数组的值
    unsigned int id = 0;

  public:
    RecordTable();

    std::vector<VarInfo &> searchVar(std::string name);//输入参数为变量名,返回在hash表中的引用
    void insertVar(std::string name, VarInfo value);
    unsigned int getID() { return this->id++; }
  };
}// namespace compiler::mid::ir

#endif//COMPILER_RECORDTABLE_H
