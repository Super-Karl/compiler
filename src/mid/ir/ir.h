//
// Created by wnx on 2021/6/13.
//

#ifndef COMPILER_IR_H
#define COMPILER_IR_H

#include <iostream>
#include <mid/recordTable/RecordTable.h>
#include <vector>

namespace compiler::mid::ir {
  class AssignIR;

  class IR;
  class IRList;

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
    Alloca
  };

  class IR {
  public:
    IR(){};
  };

  class IRList {
  public:
    std::vector<IR *> ir;
  };

  class FunCallIR : public IR {
  public:
    OperatorCode operatorCode = OperatorCode::Call;
    std::string funcName;
    std::vector<OperatorName> argList;
    ElemType retType;
    OperatorName dest;
    FunCallIR(std::string name) : IR(), funcName(name){};
  };

  class FunDefIR : public IR {
  public:
    ElemType retType;
    std::string name;
    std::vector<OperatorName> argList;
    IRList funcBody;

    FunDefIR(ElemType retTye, std::string name) : IR(), retType(retTye), name(name){};
  };

  class AssignIR : public IR {
  public:
    OperatorCode operatorCode;
    OperatorName dest;
    OperatorName source1, source2;
    AssignIR() = default;

    AssignIR(OperatorCode opcode, OperatorName dest, OperatorName source1, OperatorName source2) : IR(), operatorCode(opcode), dest(dest), source1(source1), source2(source2){};
  };

  class JmpIR : public IR {
  public:
    int label;
    JmpIR(){};
  };

  class RetIR : public IR {
  public:
    OperatorCode operatorCode = OperatorCode::Ret;
    OperatorName retVal;

    RetIR(){};
    RetIR(OperatorName retVal) : IR(), retVal(retVal){};
  };

  class AllocaIR : public IR {
  public:
    OperatorCode Opcode = OperatorCode::Alloca;
    int size;
    std::string name;//with @,%

    AllocaIR(std::string name, int size) : IR(), name(name), size(size){};
  };


}// namespace compiler::mid::ir

#endif//COMPILER_IR_H
