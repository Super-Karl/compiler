//
// Created by wnx on 2021/6/13.
//

#include "ir.h"
#include <assert.h>

namespace compiler::mid::ir {

  bool OperatorName::operator==(OperatorName &other) {
    switch (other.getType()) {
      case Type::Var:
        return this->name == other.name;
      case Type::Imm:
        return this->value == other.value;
      case Type::Void:
        assert(this->getType() == Type::Void);
        return true;
    };
  }

  OperatorName OperatorName::operator=(std::string &name) {
    auto opName = OperatorName();
    opName.name = name;
    return opName;
  }

  void printOpCode(OperatorCode op){
    switch (op) {
      case OperatorCode::Add:std::cout<<"Add ";break;
      case OperatorCode::Sub:std::cout<<"Sub ";break;
      case OperatorCode::Mul:std::cout<<"Mul ";break;
      case OperatorCode::Div:std::cout<<"Div ";break;
      case OperatorCode::Mod:std::cout<<"Mod ";break;
      case OperatorCode::Call:std::cout<<"Call ";break;
      case OperatorCode::Jmp:std::cout<<"Jmp ";break;
      case OperatorCode::Jg:std::cout<<"Jg ";break;
      case OperatorCode::Jge:std::cout<<"Jge ";break;
      case OperatorCode::Jl:std::cout<<"Jl ";break;
      case OperatorCode::Jle:std::cout<<"Jle ";break;
      case OperatorCode::Cmp:std::cout<<"Cmp ";break;
      case OperatorCode::Jne:std::cout<<"Jne ";break;
      case OperatorCode::Jeq:std::cout<<"Jeq ";break;
      case OperatorCode::And:std::cout<<"And ";break;
      case OperatorCode::Or:std::cout<<"Or ";break;
      case OperatorCode::Xor:std::cout<<"Xor ";break;
      case OperatorCode::Not:std::cout<<"Not ";break;
      case OperatorCode::Sal:std::cout<<"Sal ";break;
      case OperatorCode::Sar:std::cout<<"Sar ";break;
      case OperatorCode::Load:std::cout<<"Load ";break;
      case OperatorCode::Store:std::cout<<"Store ";break;
      case OperatorCode::Label:std::cout<<"Label ";break;
      case OperatorCode::Ret:std::cout<<"Ret ";break;
      case OperatorCode::Nop:std::cout<<"Nop ";break;
      case OperatorCode::Assign:std::cout<<"Assign ";break;
      case OperatorCode::Alloca:std::cout<<"Alloca ";break;
    }
  }

  void printOpName(OperatorName op,char sp ){
    switch (op.type) {
      case Type::Imm:
      case Type::Var:
      case Type::Void:
        break;
    }
    if(sp == '\n')
      std::cout<<'\t';
    std::cout<<op.name<<':'<<op.value<<sp;
  }
}// namespace compiler::mid::ir
