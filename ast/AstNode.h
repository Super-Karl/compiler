//
// Created by wnx on 2021/6/9.
//

#ifndef COMPILER_ASTNODE_H
#define COMPILER_ASTNODE_H

#include <iostream>
#include <vector>

using namespace std;

constexpr INT = 1;
constexpr VOID = 0;

namespace parser::ast {
  class Node {

  };

  class Expression : public Node {

  };

  /**
   * 变量声明语句
   */
  class Declare : public Expression {
  protected:
    bool isConst;//是否为常量
    vector<std::string> name;

  };

  class VarDeclare : public Declare {
  };

  class ArrayDeclare : public Declare {

  };

  /**
   * 函数声明
   */
  class FunctionDefine : public Expression {
  public:
    int retType;
    std::string name;
    FunctionDefArgList args;
    Block body;
  };

  class FunctionDefArg : public Expression {
  public:
    std::string name;
  };

  class FunctionDefArgList : public Expression {
  public:
    vector <FunctionDefArg> args;
  };

  /**
   * 函数调用
   */
  class FunctionCall : public Expression {
  public:
    std::string name;
    FunctionCallArgList args;
  };

  class FunctionCallArgList : public Expression {
  public:
    vector <Expression> args;
  };

  /**
   * 表达式
   */
  class NumberExpression : public Expression {
  public:
    int value;
  };

  class LOrExpression : public Expression {

  };

  class CondExpression : public Expression {

  };

  class LValExpression : public Expression {

  };

  class PrimaryExpression : public Expression {

  };

  class UnaryExpression : public Expression {

  };

  class UnaryOp : public Expression {

  };

  class MulExpression : public Expression {

  };

  class AddExpression : public Expression {

  };

  class RelExpression : public Expression {

  };

  class EqExpression : public Expression {

  };

  class LAndExpression : public Expression {

  };

  class ConstExpression : public Expression {

  };

  /**
   * 语句
   */
  class Stmt : public Expression {
  public:

  };

  class IfStatement : public Stmt {
  public:

  };

  class WhileStatement : public Stmt {

  };

  class BreakStatement : public Stmt {

  };

  class ContinueStatement : public Stmt {

  };

  class ReturnStatement : public Stmt {
  public:
    Expression returnExp;
  };

  class Block : public Stmt {//语句块
  public:
    vector <Expression> blockItem;
  };
}

#endif //COMPILER_ASTNODE_H
