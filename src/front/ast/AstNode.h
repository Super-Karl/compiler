//
// Created by wnx on 2021/6/9.
//

#ifndef COMPILER_ASTNODE_H
#define COMPILER_ASTNODE_H

#include "mid/ir/ir.h"
#include <iostream>
#include <mid/recordTable/RecordTable.h>
#include <vector>

using namespace std;

namespace compiler {
  namespace front::ast {
    using namespace compiler::mid::ir;
    class Node {
    public:
      virtual void print(int depth = 0, bool isEnd = false);

      virtual void genIR(mid::ir::IRList &ir, RecordTable *record);

      void printPrefix(int depth = 0, bool isEnd = false);
    };

    class Expression : public Node {
    public:
      virtual int eval(RecordTable *record);
      //      virtual OperatorName evalIR(IRList &ir, RecordTable *record);
    };

    class ArrayInitVal : public Expression {
    public:
      vector<Expression *> initValList;

      ArrayInitVal(){};

      void print(int depth, bool isEnd = false) override;
    };

    class Identifier : public Node {
    public:
      Identifier(string name) : name(name){};

      void print(int depth = 0, bool isEnd = false) override;

      string name;

      OperatorName eval(RecordTable *record);
    };

    class ArrayIdentifier : public Identifier {
    public:
      string name;
      vector<Expression *> index;

      //      vector<Expression *> shape;
      ArrayIdentifier(string name) : Identifier(name){};

      void print(int depth, bool isEnd) override;

      OperatorName eval(RecordTable *record);
    };

    class Stmt : public Expression {
    public:
    };

    class Block : public Stmt {//语句块
    public:
      vector<Expression *> blockItem;

      Block(){};

      void genIR(mid::ir::IRList &ir, RecordTable *record) override;

      void print(int depth = 0, bool isEnd = false);
    };

    /**
     * 变量声明语句
     */
    class Declare : public Node {
    public:
      Identifier *name;

      Declare(){};

      Declare(Identifier *name) {
        this->name = name;
      };
    };

    class VarDeclare : public Declare {
    public:
      VarDeclare(Identifier *name) : Declare(name){};

      void print(int depth = 0, bool isEnd = false) override;

      void genIR(IRList &ir, RecordTable *record) override;
    };

    class VarDeclareWithInit : public Declare {
    public:
      Expression *value;

      VarDeclareWithInit(Identifier *name, Expression *value) : Declare(name), value(value){};

      void print(int depth = 0, bool isEnd = false) override;

      void genIR(IRList &ir, RecordTable *record) override;
    };

    class ConstDeclare : public Declare {
    public:
      Expression *value;

      ConstDeclare(Identifier *name, Expression *value) : Declare(name), value(value){};

      void print(int depth = 0, bool isEnd = false) override;
      void genIR(IRList &ir, RecordTable *record);
    };

    class ArrayDeclare : public Declare {
    public:
      ArrayIdentifier *arrayName;

      ArrayDeclare(ArrayIdentifier *name) : Declare(name), arrayName(name){};

      void print(int depth = 0, bool isEnd = false) override;

      void genIR(IRList &ir, RecordTable *record) override;
    };

    class ConstArray : public Declare {
    public:
      ArrayIdentifier *arrayName;
      ArrayInitVal *initVal;

      ConstArray(ArrayIdentifier *name, ArrayInitVal *initVal) : Declare(name), initVal(initVal), arrayName(name){};

      void print(int depth = 0, bool isEnd = false) override;

      void genIR(IRList &ir, RecordTable *record) override;
    };

    class ArrayDeclareWithInit : public Declare {
    public:
      ArrayIdentifier *arrayName;
      ArrayInitVal *initVal;

      ArrayDeclareWithInit(){};

      ArrayDeclareWithInit(ArrayIdentifier *name, ArrayInitVal *initVal) : Declare(name), arrayName(name), initVal(initVal){};

      void print(int depth = 0, bool isEnd = false) override;
      void genIR(IRList &ir, RecordTable *record);
    };

    /**
     * 函数声明
     */
    class FunctionDefArg : public Expression {
    public:
      Identifier *name;
      int type;

      FunctionDefArg(){};

      FunctionDefArg(int type, Identifier *name) : type(type), name(name){};

      void print(int depth = 0, bool isEnd = false) override;
      void genIR(IRList &ir, RecordTable *record);
    };

    class FunctionDefArgList : public Expression {
    public:
      vector<FunctionDefArg *> args;

      FunctionDefArgList(){};

      void print(int depth = 0, bool isEnd = false) override;
      void genIR(IRList &ir, RecordTable *record);
    };

    class FunctionDefine : public Expression {
    public:
      int retType;
      Identifier *name;
      FunctionDefArgList *args;
      Block *body;

      FunctionDefine(){};

      FunctionDefine(int &retType, Identifier *name, FunctionDefArgList *args, Block *block) : retType(retType),
                                                                                               name(name),
                                                                                               args(args),
                                                                                               body(block){};

      void print(int depth = 0, bool isEnd = false) override;
      void genIR(IRList &ir, RecordTable *record);
    };


    /**
     * 函数调用
     */
    class FunctionCallArgList : public Expression {
    public:
      vector<Expression *> args;

      FunctionCallArgList(){};

      void print(int depth = 0, bool isEnd = false) override;
    };

    class FunctionCall : public Expression {
    public:
      Identifier *name;
      FunctionCallArgList *args;

      FunctionCall(){};

      FunctionCall(Identifier *name, FunctionCallArgList *args) : name(name), args(args){};

      void print(int depth = 0, bool isEnd = false) override;
      void genIR(IRList &ir, RecordTable *record);
    };

    /**
     * 表达式
     */
    class CommaExpression : public Expression {//逗号表达式
    public:
      vector<Expression *> expr;

      CommaExpression(){};

      void print(int depth = 0, bool isEnd = false) override;

      int eval(RecordTable *record);
    };

    class NumberExpression : public Expression {
    public:
      int value;

      NumberExpression(int value = 0) : value(value){};

      void print(int depth = 0, bool isEnd = false) override;

      int eval(RecordTable *record);
    };

    class BinaryExpression : public Expression {//逻辑表达式
    public:
      int op;
      Expression *leftExpr;
      Expression *rightExpr;

      BinaryExpression(Expression *left, int op, Expression *right) : leftExpr(left), op(op), rightExpr(right){};

      void print(int depth = 0, bool isEnd = false) override;

      int eval(RecordTable *record) override;

      int evalIR(IRList &ir, RecordTable *record);
    };

    class UnaryExpression : public Expression {
    public:
      int op;
      Expression *right;

      UnaryExpression(int op, Expression *right) : op(op), right(right){};

      void print(int depth = 0, bool isEnd = false) override;

      int eval(RecordTable *record);
    };

    class AssignStmt : public Stmt {//赋值表达式
    public:
      Identifier *name;
      Expression *rightExpr;

      AssignStmt(Identifier *inName, Expression *right) : name(inName), rightExpr(right){};

      void genIR(mid::ir::IRList &ir, RecordTable *record) override;

      void print(int depth = 0, bool isEnd = false) override;
    };

    /**
     * 语句
     */
    class DeclareStatement : public Stmt {
    public:
      vector<Declare *> declareList;

      DeclareStatement() : Stmt(){};

      DeclareStatement(vector<Declare *> declareList) : declareList(declareList){};

      void genIR(mid::ir::IRList &ir, RecordTable *record) override;

      void print(int depth = 0, bool isEnd = false) override;
    };

    class IfStatement : public Stmt {
    public:
      Expression *cond;//
      Stmt *trueBlock;
      Stmt *elseBlock;

      IfStatement(Expression *cond, Stmt *trueBlock, Stmt *elseBlock) : cond(cond), trueBlock(trueBlock),
                                                                        elseBlock(elseBlock){};

      void genIR(mid::ir::IRList &ir, RecordTable *record) override;

      void print(int depth = 0, bool isEnd = false) override;
    };

    class WhileStatement : public Stmt {
    public:
      Expression *cond;
      Stmt *loopBlock;

      WhileStatement(Expression *cond, Stmt *loopBlock) : cond(cond), loopBlock(loopBlock){};

      void genIR(mid::ir::IRList &ir, RecordTable *record) override;

      void print(int depth = 0, bool isEnd = false) override;
    };

    class BreakStatement : public Stmt {
    public:
      void genIR(mid::ir::IRList &ir, RecordTable *record) override;

      void print(int depth = 0, bool isEnd = false) override;
    };

    class ContinueStatement : public Stmt {
    public:
      void genIR(mid::ir::IRList &ir, RecordTable *record) override;

      void print(int depth = 0, bool isEnd = false) override;
    };

    class VoidStatement : public Stmt {//空语句
    public:
      void print(int depth = 0, bool isEnd = false) override;
    };

    class ReturnStatement : public Stmt {
    public:
      Expression *returnExp;

      ReturnStatement(Expression *exp = NULL) : returnExp(exp){};

      void genIR(mid::ir::IRList &ir, RecordTable *record) override;

      void print(int depth = 0, bool isEnd = false) override;
    };


    class AST {
    public:
      vector<Node *> codeBlock;

      AST(){};

      void print(int depth = 0, bool isEnd = false);

      void genIR(IRList &ir, RecordTable *record);
    };
  }// namespace front::ast
}// namespace compiler
#endif//COMPILER_ASTNODE_H
