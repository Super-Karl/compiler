//
// Created by wnx on 2021/6/13.
//

#ifndef COMPILER_IR_H
#define COMPILER_IR_H

#include <iostream>

namespace compiler::front {
  enum Type {
    Imm,
    Void,
    Var
  };

  class OpType {
  public:
    Type type;
    std::string name;
    int value;

    OpType() {};

    OpType(Type type) : type(type) {};

    Type getType() const { return this->type; };
  };

  enum class Operator {
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
  namespace ir {
    class IR {
    public:

    };

    class FunCallIR : public IR {

    };

    class FunDefIR : public IR {

    };

    class ExprIR : public IR {

    };

    class StmtIR : public IR {
    public:
      Operator op;

    };

    class JmpIR : public IR {

    };
  }
}

#endif //COMPILER_IR_H
