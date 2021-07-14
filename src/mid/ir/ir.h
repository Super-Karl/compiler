//
// Created by wnx on 2021/6/13.
//

#ifndef COMPILER_IR_H
#define COMPILER_IR_H

#include <iostream>
#include <vector>

namespace compiler::mid::ir {
  class IR;

  using IRList = std::vector<IR *>;

  enum class ElemType {
    INT,
    VOID,
  };

  enum class Type {
    Imm,
    Void,
    Var
  };

  class OperatorName {//ir中的操作数
  public:
    Type type;
    std::string name;//ir中的name
    int value;

    OperatorName(Type type = Type::Var) : type(type){};

    OperatorName(int val, Type type = Type::Var) : value(val), type(type){};

    Type getType() const { return this->type; };

    bool operator==(OperatorName &other);

    OperatorName operator=(std::string &name);
  };

  enum class OperatorCode {
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Call,
    Jmp,
    Jg,
    Jge,
    Jl,
    Jle,
    Cmp,
    Jne,
    And,
    Or,
    Xor,
    Not,
    Sal,
    Sar,
    Load,
    Store,
    Label,
    Ret,
    Nop,
    Assign,
    Alloca
  };

  class IR {
  public:
    IR(){};
    virtual IR *getThis() {
      return this;
    }
  };

  class FunCallIR : public IR {
  public:
    OperatorCode operatorCode = OperatorCode::Call;
    std::string funcName;
    std::vector<int> argList;
    ElemType retType;
    OperatorName dest;
    FunCallIR(std::string name) : IR(), funcName(name){};
    FunCallIR *getThis() {
      return this;
    }
  };

  class FunDefIR : public IR {
  public:
    ElemType retType;
    std::string name;
    std::vector<OperatorName> argList;
    IRList funcBody;

    FunDefIR(ElemType retTye, std::string name) : IR(), retType(retTye), name(name){};

    FunDefIR *getThis() {
      return this;
    }
  };

  class AssignIR : public IR {
  public:
    OperatorCode operatorCode;
    OperatorName dest;
    OperatorName source1, source2;
    AssignIR() = default;

    AssignIR(OperatorCode opcode, OperatorName dest, OperatorName source1, OperatorName source2) : IR(), operatorCode(opcode), dest(dest), source1(source1), source2(source2){};
    AssignIR *getThis() {
      return this;
    }
  };
  class LabelIR : public IR{
  public:
    std::string label;
    LabelIR(std::string label):label(label){}
    LabelIR* getThis(){
      return this;
    }
  };
  class JmpIR : public IR {
  public:
    OperatorCode action;
    std::string label;
    JmpIR(void);
    JmpIR(OperatorCode opname ,std::string name):label(name),action(opname){};
    JmpIR *getThis() {
      return this;
    }
  };

  class RetIR : public IR {
  public:
    OperatorCode operatorCode = OperatorCode::Ret;
    OperatorName retVal;

    RetIR(){};
    RetIR(OperatorName retVal) : IR(), retVal(retVal){};

    RetIR *getThis() {
      return this;
    }
  };

  class AllocaIR : public IR {
  public:
    OperatorCode Opcode = OperatorCode::Alloca;
    int size;        //这个size是数组大小,实际分配的内存空间为size * 4
    std::string name;//with @,%

    AllocaIR(std::string name, int size) : IR(), name(name), size(size){};
    AllocaIR *getThis() {
      return this;
    }
  };

  class LoadIR : public IR {//数组元素的值赋值给变量
  public:
    OperatorCode operatorCode = OperatorCode::Load;
    OperatorName dest;
    int offset;//距离基址的偏移量,数组元素的下标,实际的偏移量为index*4
    OperatorName source;

    LoadIR(OperatorName dest, OperatorName source, int offset) : dest(std::move(dest)), source(source), offset(offset){};

    LoadIR *getThis() {
      return this;
    }
  };

  class StoreIR : public IR {//将值加载进内存(即给数组元素赋值
  public:
    OperatorCode operatorCode = OperatorCode::Store;
    OperatorName dest;
    int offset;
    int source;

    StoreIR(OperatorName dest, int source, int offset) : dest(std::move(dest)), offset(offset), source(source){};

    StoreIR *getThis() override {
      return this;
    }
  };
}// namespace compiler::mid::ir

#endif//COMPILER_IR_H
