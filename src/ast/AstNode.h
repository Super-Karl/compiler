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

  };

  class Identifier : Node {
  public:
    Identifier(const string name, bool isArray = false);

    bool isArray;
    string name;
  };

  class Expression : public Node {

  };


  class Stmt : public Expression {

  };

  class Block : public Stmt {//语句块
  public:
    vector<Expression *> blockItem;

    Block() {};
  };

  /**
   * 变量声明语句
   */
  class Declare : public Node {
  protected:
    Identifier name;
    bool isInit;//是否有初始值
    Declare(Identifier name, bool isInit);
  };

  class VarDeclare : public Declare {
  public:
    Expression value;

    VarDeclare(Identifier name, bool isInit = false);//无初始化构造
    VarDeclare(Identifier name, bool isInit, Expression value);//带初始化值构造
  };

  class ConstDeclare : public Declare {
  public:
    Expression value;

    ConstDeclare(Identifier name, Expression value, bool isInit = true);
  };

  class ArrayDeclare : public Declare {
  public:
    Expression size;

    ArrayDeclare(Expression size, Identifier name, bool isInit = false);

  };

  class ConstArray : public Declare {
  public:
    Expression size;
    const vector<Expression> valueList;

    ConstArray(Expression size, Identifier name, const vector<Expression> value, bool isInit = true);
  };

  class ArrayDeclareWithInit : public Declare {
  public:
    Expression size;
    vector<Expression> valueList;

    ArrayDeclareWithInit(Expression size, Identifier name, const vector<Expression> value, bool isInit = true);
  };

  /**
   * 函数声明
   */
  class FunctionDefArg : public Expression {
  public:
    Identifier name;

    FunctionDefArg(Identifier name) : name(name) {};
  };

  class FunctionDefArgList : public Expression {
  public:
    vector<FunctionDefArg> args;

    FunctionDefArgList(vector<FunctionDefArg> args) : args(args) {};
  };

  class FunctionDefine : public Expression {
  public:
    int retType;
    Identifier name;
    FunctionDefArgList args;
    Block body;

    FunctionDefine(int retType, Identifier name, FunctionDefArgList args, Block body);
  };


  /**
   * 函数调用
   */
  class FunctionCallArgList : public Expression {
  public:
    vector<Expression> args;

    FunctionCallArgList(vector<Expression> args) : args(args) {};
  };

  class FunctionCall : public Expression {
  public:
    Identifier name;
    FunctionCallArgList args;

    FunctionCall(Identifier name, FunctionCallArgList) : name(name), args(args) {};
  };

  /**
   * 表达式
   */
  class NumberExpression : public Expression {
  public:
    int value;

    NumberExpression(int value);
  };

  class LogicExpression : public Expression {//逻辑表达式
  public:
    int op;
    Expression leftExpr;
    Expression rightExpr;

    LogicExpression(Expression left, int op, Expression right);
  };

  class UnaryExpression : public Expression {
  public:
    int op;
    Expression right;

    UnaryExpression(int op, Expression right) : op(op), right(right) {};
  };

  class CalcExpression : public Expression {//计算节点用此表达式
  public:
    int op;
    Identifier name;
    Expression rightExpr;

    CalcExpression(int op, Identifier name, Expression right);
  };

  /**
   * 语句
   */
  class DeclareStatement : public Stmt {
  public:
    vector<Declare> declareList;

    DeclareStatement(vector<Declare> declareList) : declareList(declareList) {};
  };

  class IfStatement : public Stmt {
  public:
    LogicExpression cond;//
    Block trueBlock;
    Block elseBlock;

    IfStatement(LogicExpression cond, Block trueBlock, Block elseBlock) : cond(cond), trueBlock(trueBlock),
                                                                          elseBlock(elseBlock) {};
  };

  class WhileStatement : public Stmt {
  public:
    LogicExpression cond;
    Block loopBlock;

    WhileStatement(LogicExpression cond, Block loopBlock) : cond(cond), loopBlock(loopBlock) {};

  };

  class BreakStatement : public Stmt {

  };

  class ContinueStatement : public Stmt {

  };

  class ReturnStatement : public Stmt {
  public:
    Expression *returnExp;

    ReturnStatement(Expression *exp = NULL);
  };


  class AST {
    vector<Node> sourceCode;

    AST() {};
  };
}

#endif //COMPILER_ASTNODE_H
