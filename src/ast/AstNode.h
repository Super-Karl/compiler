//
// Created by wnx on 2021/6/9.
//

#ifndef COMPILER_ASTNODE_H
#define COMPILER_ASTNODE_H

#include <iostream>
#include <vector>

using namespace std;

namespace parser::ast {
  class Node {
  public:
    virtual void print(int depth = 0, bool isEnd = false);

    void printPrefix(int depth = 0,bool isEnd = false);
  };

  class Identifier : Node {
  public:
    Identifier(const string name, bool isArray = false);

    void print(int depth = 0, bool isEnd = false) override;

    bool isArray;
    string name;
  };

  class Expression : public Node {
  public:
  };


  class Stmt : public Expression {
  public:
  };

  class Block : public Stmt {//语句块
  public:
    vector<Expression *> blockItem;

    Block() {};

    void print(int depth = 0, bool isEnd = false);
  };

  /**
   * 变量声明语句
   */
  class Declare : public Node {
  public:
    Identifier name;

    Declare(Identifier name);

  };

  class VarDeclare : public Declare {
  public:

    VarDeclare(Identifier name);//无初始化构造

    void print(int depth = 0, bool isEnd = false) override;

  };

  class VarDeclareWithInit : public Declare {
  public:
    Expression *value;

    VarDeclareWithInit(Identifier name, Expression *value);

    void print(int depth = 0, bool isEnd = false) override;
  };

  class ConstDeclare : public Declare {
  public:
    Expression *value;

    ConstDeclare(Identifier name, Expression *value);

    void print(int depth = 0, bool isEnd = false) override;
  };

  class ArrayDeclare : public Declare {
  public:
    Expression *size;

    ArrayDeclare(Expression *size, Identifier name);

    void print(int depth = 0, bool isEnd = false) override;
  };

  class ConstArray : public Declare {
  public:
    Expression *size;
    const vector<Expression *> valueList;

    ConstArray(Expression *size, Identifier &name, const vector<Expression *> value);

    void print(int depth = 0, bool isEnd = false) override;
  };

  class ArrayDeclareWithInit : public Declare {
  public:
    Expression *size;
    vector<Expression *> valueList;

    ArrayDeclareWithInit(Expression *size, Identifier &name, const vector<Expression *> value);

    void print(int depth = 0, bool isEnd = false) override;
  };

  /**
   * 函数声明
   */
  class FunctionDefArg : public Expression {
  public:
    Identifier &name;
    int type;

    FunctionDefArg(Identifier &name, int type) : name(name), type(type) {};

    void print(int depth = 0, bool isEnd = false) override;
  };

  class FunctionDefArgList : public Expression {
  public:
    vector<FunctionDefArg *> args;

    FunctionDefArgList(vector<FunctionDefArg *> args) : args(args) {};

    void print(int depth = 0, bool isEnd =false) override;
  };

  class FunctionDefine : public Expression {
  public:
    int retType;
    Identifier &name;
    FunctionDefArgList *args;
    Block *body;

    FunctionDefine(int retType, Identifier name, FunctionDefArgList *args, Block *body);

    void print(int depth = 0, bool isEnd = false) override;
  };


  /**
   * 函数调用
   */
  class FunctionCallArgList : public Expression {
  public:
    vector<Expression *> args;

    FunctionCallArgList(vector<Expression *> args) : args(args) {};

    void print(int depth = 0, bool isEnd = false) override;
  };

  class FunctionCall : public Expression {
  public:
    Identifier &name;
    FunctionCallArgList &args;

    FunctionCall(Identifier name, FunctionCallArgList &args) : name(name), args(args) {};

    void print(int depth = 0, bool isEnd = false) override;
  };

  /**
   * 表达式
   */
  class CommaExpression : public Expression {//逗号表达式
  public:
    vector<Expression *> expr;

    CommaExpression() = default;

    void print(int depth = 0, bool isEnd = false) override;
  };

  class NumberExpression : public Expression {
  public:
    int value;

    NumberExpression(int value);

    void print(int depth = 0, bool isEnd = false) override;
  };

  class LogicExpression : public Expression {//逻辑表达式
  public:
    int op;
    Expression *leftExpr;
    Expression *rightExpr;

    LogicExpression(Expression *left, int op, Expression *right);

    void print(int depth = 0, bool isEnd = false) override;
  };

  class UnaryExpression : public Expression {
  public:
    int op;
    Expression *right;

    UnaryExpression(int op, Expression *right) : op(op), right(right) {};

    void print(int depth = 0, bool isEnd = false) override;
  };

  class CalcExpression : public Expression {//赋值表达式
  public:
    int op;
    Identifier &name;
    Expression *rightExpr;

    CalcExpression(int op, Identifier name, Expression *right);

    void print(int depth = 0, bool isEnd = false) override;
  };

  /**
   * 语句
   */
  class DeclareStatement : public Stmt {
  public:
    vector<Declare*> declareList;

    DeclareStatement(vector<Declare*> declareList) : declareList(declareList) {};

    void print(int depth = 0, bool isEnd = false) override;
  };

  class IfStatement : public Stmt {
  public:
    LogicExpression *cond;//
    Block *trueBlock;
    Block *elseBlock;

    IfStatement(LogicExpression *cond, Block *trueBlock, Block *elseBlock) : cond(cond), trueBlock(trueBlock),
                                                                          elseBlock(elseBlock) {};

    void print(int depth = 0, bool isEnd = false) override;
  };

  class WhileStatement : public Stmt {
  public:
    LogicExpression *cond;
    Block *loopBlock;

    WhileStatement(LogicExpression *cond, Block *loopBlock) : cond(cond), loopBlock(loopBlock) {};

    void print(int depth = 0, bool isEnd = false) override;
  };

  class BreakStatement : public Stmt {
  public:
    void print(int depth = 0, bool isEnd = false) override;
  };

  class ContinueStatement : public Stmt {
  public:
    void print(int depth = 0, bool isEnd = false) override;
  };

  class VoidStatement : public Stmt {//空语句
  public:
    void print(int depth = 0, bool isEnd = false) override;
  };

  class ReturnStatement : public Stmt {
  public:
    Expression *returnExp;

    ReturnStatement(Expression *exp = NULL);

    void print(int depth = 0, bool isEnd = false) override;
  };


  class AST {
  public:
    vector<Node*> codeBlock;

    AST() = default;

    void print(int depth = 0 , bool isEnd = false);
  };
}

#endif //COMPILER_ASTNODE_H
