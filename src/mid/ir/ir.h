//
// Created by wnx on 2021/6/13.
//

#ifndef COMPILER_IR_H
#define COMPILER_IR_H

#include <iostream>
#include <vector>

namespace compiler::mid::ir {
  class AssignIR;

  class IR;

  using IRList = std::vector<IR *>;
  enum Type {
    Imm,
    Void,
    Var
  };

  class OperatorName {
  public:
    Type type;
    std::string name;
    int value;

    OperatorName(Type type = Type::Var) : type(type){};

    Type getType() const { return this->type; };

    bool operator==(OperatorName &other);
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

  class FunCallIR : public IR {
  public:
    std::string funcName;
    std::vector<OperatorName> argList;

    FunCallIR(std::string name) : IR(), funcName(name){};
  };

  class FunDefIR : public IR {
  public:
    Type retType;
    std::string name;
    std::vector<OperatorName> argList;
    std::vector<AssignIR> funcBody;

    FunDefIR(Type retTye, std::string name) : IR(), retType(retTye), name(name){};
  };

  class ExprIR : public IR {
  public:
    OperatorCode operatorCode;
    OperatorName dest;
    OperatorName source1, source2;

    ExprIR() : IR(){};

    ExprIR(OperatorCode operatorCode, OperatorName dest, OperatorName op1, OperatorName op2) : IR(), operatorCode(
                                                                                                         operatorCode),
                                                                                               dest(dest),
                                                                                               source1(op1),
                                                                                               source2(op2){};
  };

  class AssignIR : public IR {
  public:
    OperatorName operatorName;
    IR *expr;

    AssignIR() = default;

    AssignIR(OperatorName operatorName, IR *expr) : operatorName(operatorName), expr(expr){};
  };

  class JmpIR : public IR {
  public:
    std::string label;
  };

  class DeclIR : public IR {
  public:
  };
}// namespace compiler::mid::ir

#endif//COMPILER_IR_H
