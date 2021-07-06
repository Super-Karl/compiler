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
    bool isConst;
    bool isArray;
    std::vector<int> value;
    std::vector<int> shape;
    std::string name;//with identifier @,% 为ssa中rename后的名字

    VarInfo(std::string name, int value, bool isConst = false);
    VarInfo(std::string name,std::vector<int>& inShape,std::vector<int>& inValue,bool isConst = false) ;
    VarInfo::VarInfo(std::string name,std::vector<int>& inValue,std::initializer_list<int> inShape,bool isConst = false);
    //可用{}将形状框起传参
  };

  //全局的记录表,用来记录sy程序中变量的use,
  /* TODO 没实现φ函数,多分支情况下的ssa变量选取*/
  class RecordTable {
  private:
    std::unordered_map<std::string, VarInfo> varTable;//符号表,变量的vec只有一个值,数组的vector会存储所有数组的值
    RecordTable *father;
    unsigned int id = 0;

  public:
    RecordTable(RecordTable *rt = NULL) : father(rt) {}
    VarInfo &searchVar(std::string name);             //输入参数为变量名,返回在hash表中的引用
    RecordTable &insertVar(std::string name, VarInfo);//插入单个varInfo元素
    unsigned int getID() { return this->id++; }
  };
}// namespace compiler::mid::ir

#endif//COMPILER_RECORDTABLE_H
