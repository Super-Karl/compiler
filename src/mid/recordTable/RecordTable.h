//
// Created by wnx on 2021/7/5.
//

#ifndef COMPILER_RECORDTABLE_H
#define COMPILER_RECORDTABLE_H

#include "mid/ir/ir.h"
#include <list>
#include <stack>
#include <unordered_map>
#include <utility>
#include <vector>

namespace compiler::mid::ir {
  class VarRedefChain {
  public:
    bool canAssign = false;//能否计算出变量值
    std::string defName;   //在ir中该次define的名字
    int val;               //该次define的值
    VarRedefChain() = default;
    VarRedefChain(std::string defName, int val = INT32_MIN, bool canAssign = false) : defName(std::move(defName)), val(val), canAssign(canAssign){};
  };

  //  数组的处理比较麻烦,因为数组在定义的时候标识符是整个数组,而不是单个数组元素,所以给数组单独添加一个属性处理;
  //  int变量,varinfo中的name属性不会调用
  //  在初始化时,数组会有arrayName属性,而varUse中的defName为空;defName在第一次调用数组元素时生成
  class VarInfo {
  public:
    bool isConst;
    bool isArray;
    std::string arrayName;//只有数组会使用这个标识符
    std::vector<int> shape;
    std::vector<std::list<VarRedefChain>> varUse;//TODO试图用varRedefChain 表示变量的def链,use变量的时候实际使用的是def list的顶层

    VarInfo(std::string name, int value, bool canAssign = false, bool isConst = false);
    VarInfo(std::string name, std::vector<int> &inShape, std::vector<int> &inValue, bool isConst = false);
    VarInfo(std::string name, std::vector<int> &inValue, std::initializer_list<int> inShape, bool isConst = false);
    //可用{}将形状框起传参
    int getArrayVal(std::vector<int> index);

    int getArrayIndex(std::vector<int> index);

    int getVal();

    bool canAssign(std::vector<int> = {});

    void updateVarUse(VarRedefChain var, std::vector<int> index);
  };

  //全局的记录表,用来记录sy程序中变量的use
  class RecordTable {
  private:
    std::unordered_map<std::string, VarInfo *> varTable;//符号表,变量的vec只有一个值,数组的vector会存储所有数组的值
    RecordTable *father;
    unsigned int id = 0;
    static std::stack<std::pair<JmpIR*,JmpIR*>> labelPairs;

  public:
    RecordTable(RecordTable *rt = nullptr) : father(rt), id(rt != nullptr ? rt->id : 0){};
    VarInfo *searchVar(std::string name);              //输入参数为变量名,返回在  hash表中的引用
    void insertVar(std::string name, VarInfo *varInfo);//插入单个varInfo元素
    static void pushLabelPair(JmpIR*,JmpIR*);
    static void popLabelPair();
    static std::pair<JmpIR*,JmpIR*>& getTopLabel();
    unsigned int getID() { return this->id++; }

    RecordTable *getFarther() { return father; }

    bool canExprAssign(std::string op1, std::string op2, std::vector<int> index1 = {}, std::vector<int> index2 = {});
  };
}// namespace compiler::mid::ir

#endif//COMPILER_RECORDTABLE_H
