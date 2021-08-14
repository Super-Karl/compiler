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
    std::string defName;

    OperatorName(int val, Type type = Type::Var) : value(val), type(type), name(""){};

    OperatorName(std::string name, Type type = Type::Var) : type(type), name(std::move(name)) {}

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
    Jeq,
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
    Mov,
    Alloca,
    PhiMov
  };
  void printOpCode(OperatorCode op);
  void printOpName(OperatorName op, char sp = '\n', bool = true);
  class IR {
  public:
    IR(){};
    virtual void print() {
    }
    virtual IR *getThis() {
      return this;
    }
  };

  class GlobalData : public IR {
  public:
    std::string name;
    bool isArray;
    std::vector<int> val;

    GlobalData(std::string name, int val) : name(std::move(name)), isArray(false) { this->val.push_back(val); };

    GlobalData(std::string name, std::vector<int> val) : name(std::move(name)), isArray(true), val(std::move(val)){}
    void print() override {
      std::cout<<".Global";
      if (isArray)
        std::cout<<"Array";
      std::cout<<' '<<name;
      for (auto &item:val){
        std::cout<<' '<<item;
      }
      std::cout<<' '<<std::endl;
    }
  };

  class FunCallIR : public IR {
  public:
    OperatorCode operatorCode = OperatorCode::Call;
    std::string funcName;
    std::vector<OperatorName> argList;
    ElemType retType;
    OperatorName retOp;

    FunCallIR(std::string name) : IR(), funcName(std::move(name)){};
    FunCallIR *getThis() override {
      return this;
    }
    void print() {
      std::cout << '\t';
      printOpCode(OperatorCode::Call);
      std::cout << funcName << ' ';
      for (auto &i : argList) {
        printOpName(i, ' ');
      }
      if (retType == ElemType::VOID)
        std::cout << std::endl;
      else {
        std::cout << "& ";
        printOpName(retOp, '\n', 0);
      }
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
    void print() {
      std::cout << "@" << name << ':' << " ";
      for (auto &i : argList) {
        printOpName(i, ' ');
      }
      std::cout << '\n';
      for (auto &i : funcBody) {
        i->print();
      }
    }
  };

  class AssignIR : public IR {
  public:
    OperatorCode operatorCode;
    OperatorName dest;
    OperatorName source1, source2;

    AssignIR() = default;

    AssignIR(OperatorCode opcode, OperatorName dest, OperatorName source1, OperatorName source2) : IR(), operatorCode(opcode), dest(std::move(dest)), source1(std::move(source1)), source2(std::move(source2)){};

    AssignIR(OperatorName dest, OperatorName source, OperatorCode opcode = OperatorCode::Mov) : dest(std::move(dest)), source1(std::move(source)), operatorCode(opcode){};

    AssignIR *getThis() override {
      return this;
    }
    void print() override {
      std::cout << '\t';
      printOpCode(operatorCode);
      printOpName(dest, ' ');
      printOpName(source1, ' ');
      printOpName(source2, ' ');
      std::cout << '\n';
    }
  };
  class LabelIR : public IR {
  public:
    std::string label;
    LabelIR() = default;
    LabelIR(std::string label) : label(label) {}
    LabelIR *getThis() {
      return this;
    }
    void print() override {
      std::cout << label << ":\n";
    }
  };
  class JmpIR : public IR {
  public:
    OperatorCode action;
    std::string label;
    JmpIR(void);
    JmpIR(OperatorCode opname, std::string name) : label(name), action(opname){};
    JmpIR(OperatorCode opname, LabelIR *name) : label(name->label), action(opname){};
    JmpIR *getThis() {
      return this;
    }
    void print() override {
      std::cout << '\t';
      printOpCode(action);
      std::cout << label << '\n';
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
    void print() override {
      std::cout << '\t';
      printOpCode(operatorCode);
      printOpName(retVal, ' ');
      std::cout << '\n';
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
    void print() override {
      std::cout << '\t';
      printOpCode(Opcode);
      std::cout << name << ", 4byte *" << size << "\n";
    }
  };

  class LoadIR : public IR {//数组元素的值赋值给变量
  public:
    OperatorCode operatorCode = OperatorCode::Load;
    OperatorName dest;
    OperatorName offset;//距离基址的偏移量,数组元素的下标,实际的偏移量为index*4
    OperatorName source;

    LoadIR(OperatorName dest, OperatorName source, OperatorName offset) : dest(std::move(dest)), source(std::move(source)), offset(offset){};

    LoadIR *getThis() override {
      return this;
    }
    void print() override {
      std::cout << '\t';
      printOpCode(operatorCode);
      printOpName(dest, ' ');
      printOpName(offset, ' ');
      printOpName(source, '\n', false);
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
    void print() override {
      std::cout << '\t';
      printOpCode(operatorCode);
      printOpName(dest, ' ');
      printOpName(offset, ' ');
      printOpName(source, '\n', false);
    }
  };
  class BlockIR:public IR{
  public:
    IRList block;
    BlockIR()=default;
    BlockIR* getThis(){
      return this;
    }
    void print() override{
      for (auto i : block){
        i->print();
      }
    }
  };
  class PhiIR:public IR{
  public:
    OperatorName dest;
    OperatorName op1;
    PhiIR(OperatorName dest,OperatorName op1):dest(dest), op1(op1){}
    void print() override{
      std::cout<<"\tPhiMov ";
      printOpName(dest,' ');
      printOpName(op1,'\n', false);
    }
  };
}// namespace compiler::mid::ir

#endif//COMPILER_IR_H
