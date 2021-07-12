//
// Created by wnx on 2021/6/13.
//

#ifndef COMPILER_IR_H
#define COMPILER_IR_H

#include <iostream>
#include <utility>
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
    IR() = default;
    ;
    virtual IR *getThis() {
      return this;
    }
  };

  class FunCallIR : public IR {
  public:
    OperatorCode operatorCode = OperatorCode::Call;
    std::string funcName;
    std::vector<OperatorName> argList;
    FunCallIR(std::string name) : IR(), funcName(std::move(name)){};
    FunCallIR *getThis() override {
      return this;
    }
  };

  class FunDefIR : public IR {
  public:
    ElemType retType;
    std::string name;
    std::vector<OperatorName> argList;
    IRList funcBody;

    FunDefIR(ElemType retTye, std::string name) : IR(), retType(retTye), name(std::move(name)){};

    FunDefIR *getThis() override {
      return this;
    }
  };

  class AssignIR : public IR {
  public:
    OperatorCode operatorCode;
    OperatorName dest;
    OperatorName source1, source2;
    AssignIR() = default;

    AssignIR(OperatorCode opcode, OperatorName dest, OperatorName source1, OperatorName source2) : IR(), operatorCode(opcode), dest(std::move(dest)), source1(source1), source2(std::move(source2)){};
    AssignIR *getThis() override {
      return this;
    }
  };

  class JmpIR : public IR {
  public:
    int label;
    JmpIR(){};
    JmpIR *getThis() override {
      return this;
    }
  };

  class RetIR : public IR {
  public:
    OperatorCode operatorCode = OperatorCode::Ret;
    OperatorName retVal;

    RetIR() { retVal = OperatorName(Type::Void); };
    RetIR(OperatorName retVal) : IR(), retVal(std::move(retVal)){};

    RetIR *getThis() override {
      return this;
    }
  };

  class AllocaIR : public IR {
  public:
    OperatorCode Opcode = OperatorCode::Alloca;
    int size;        //这个size是数组大小,实际分配的内存空间为size * 4
    std::string name;//with @,%

    AllocaIR(std::string name, int size) : IR(), name(std::move(name)), size(size){};
    AllocaIR *getThis() override {
      return this;
    }
  };

  class LoadIR : public IR {//数组元素的值赋值给变量
  public:
    OperatorCode operatorCode = OperatorCode::Load;
    OperatorName dest;
    OperatorName offset;//距离基址的偏移量,数组元素的下标,实际的偏移量为index*4
    OperatorName source;

    LoadIR(OperatorName dest, OperatorName source, int offset) : dest(std::move(dest)), source(std::move(source)), offset(offset){};

    LoadIR *getThis() override {
      return this;
    }
  };

  class StoreIR : public IR {//将值加载进内存(即给数组元素赋值
  public:
    OperatorCode operatorCode = OperatorCode::Store;
    OperatorName dest;
    OperatorName offset;
    OperatorName source;

    StoreIR(OperatorName dest, OperatorName source, OperatorName offset) : dest(std::move(dest)), offset(std::move(offset)), source(std::move(source)){};

    StoreIR *getThis() override {
      return this;
    }
  };
}// namespace compiler::mid::ir

#endif//COMPILER_IR_H
