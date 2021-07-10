//
// Created by wnx on 2021/7/5.
//

#ifndef COMPILER_RECORDTABLE_H
#define COMPILER_RECORDTABLE_H

#include "mid/ir/ir.h"
#include <unordered_map>
#include <vector>

namespace compiler::mid::ir {
  class VarInfo {
  public:
    bool isConst;
    bool isArray;
    std::vector<int> value;//存储数据,如果是int,只有一个元素,如果是数组,数据个数为数组转化为一维之后的长度
    std::vector<int> shape;
    std::string name;//with identifier @,% 为ssa中rename后的名字

    VarInfo(std::string name, int value, bool isConst = false);
    VarInfo(std::string name, std::vector<int> &inShape, std::vector<int> &inValue, bool isConst = false);
    VarInfo(std::string name, std::vector<int> &inValue, std::initializer_list<int> inShape, bool isConst = false);
    //可用{}将形状框起传参
  };

  //全局的记录表,用来记录sy程序中变量的use
  class RecordTable {
  private:
    std::unordered_map<std::string, VarInfo *> varTable;//符号表,变量的vec只有一个值,数组的vector会存储所有数组的值
    RecordTable *father;
    unsigned int id = 0;

  public:
    RecordTable(RecordTable *rt = NULL) : father(rt), id(rt != nullptr ? rt->id : 0){};
    VarInfo *searchVar(std::string name);              //输入参数为变量名,返回在hash表中的引用
    void insertVar(std::string name, VarInfo *varInfo);//插入单个varInfo元素
    unsigned int getID() { return this->id++; }
    RecordTable *getFarther() { return father; }
  };
}// namespace compiler::mid::ir

#endif//COMPILER_RECORDTABLE_H
