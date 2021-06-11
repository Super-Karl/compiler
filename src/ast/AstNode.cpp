//
// Created by wnx on 2021/6/9.
//

#include "AstNode.h"
#include <cstdint>
#include <iostream>
#include <iterator>

using namespace parser::ast;

/**
*打印ast
**/
void Node::printPrefix(int depth, bool isEnd) {
  for (int i = 0; i < depth; i++)
    cout << (i == depth - 1 ? (isEnd ? "└" : "├") : "│   ");

  cout << "── ";
}

void Node::print(int depth, bool isEnd) {
  this->printPrefix();
}

void Identifier::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "Identifier: " + this->name << endl;
};

void Block::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "Block" << endl;
  for (auto i : blockItem)
    i->print(depth + 1, i == blockItem.back());
};

void VarDeclare::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "VarDeclare" << endl;
  this->name.print(depth + 1, true);
}

void VarDeclareWithInit::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "VarDeclareWithInit" << endl;
  this->name.print(depth + 1, false);
  this->value->print(depth + 1, true);
}

void ConstDeclare::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "ConstDeclare" << endl;
  this->name.print(depth + 1, false);
  this->value->print(depth + 1, true);
}

void ArrayDeclare::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "ArrayDeclare: " << endl;
  this->name.print(depth + 1, false);
  size->print(depth + 1, true);
}

void ConstArray::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "ConstArray" << endl;
  this->name.print(depth + 1, false);
  size->print(depth + 1, false);

  for (auto i : valueList)
    i->print(depth + 1, i == valueList.back());
}

void ArrayDeclareWithInit::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "ArrayDeclareWithInit" << endl;
  this->name.print(depth + 1, false);

  for (auto i : valueList)
    i->print(depth + 1, i == valueList.back());
}

void FunctionDefArg::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "FunctionDefArg Type: " << type << endl;
  this->name.print(depth + 1, true);

}

void FunctionDefArgList::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "FunctionDefArgList" << endl;

  for (auto i : args)
    i->printPrefix(depth + 1, i == args.back());
}

void FunctionDefine::print(int depth, bool isEnd) {
  this->print(depth, isEnd);
  cout << "FunctionDefine" << endl;
  cout << "returnType: " << retType << endl;
  name.print(depth + 1, false);
  args->printPrefix(depth + 1, isEnd);
  body->print(depth + 1, true);
}

void FunctionCallArgList::print(int depth, bool isEnd) {
  this->printPrefix(depth + 1, isEnd);
  cout << "FunctionCallArgList" << endl;
  for (auto i : args)
    i->print(depth + 1, i == args.back());
}

void FunctionCall::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "FunctionCall" << endl;
  name.print(depth + 1, false);
  args.print(depth + 1, true);
}

void CommaExpression::print(int depth, bool isEnd) {
  for (auto i : expr)
    i->print(depth + 1, i == expr.back());
}

void NumberExpression::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "Number: " << value << endl;
}

void LogicExpression::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "LogicExpression" << endl;
  leftExpr->print(depth + 1, false);
  rightExpr->print(depth + 1, true);
}

void UnaryExpression::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "UnaryExpression Oprand: " << op << endl;
  right->print(depth + 1, true);
}

void CalcExpression::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "CalcExpression Oprand: " << op << endl;
  name.print(depth + 1, false);
  rightExpr->print(depth + 1, true);
}

void DeclareStatement::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "DeclareStatement" << endl;
  for (auto i : declareList)
    i->print(depth + 1, i == declareList.back());
}

void IfStatement::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "IfStatement" << endl;
  cond->print(depth + 1, false);
  trueBlock->print(depth + 1, false);
  elseBlock->print(depth + 1, true);
}

void WhileStatement::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "WhileStatement" << endl;
  cond->print(depth + 1, false);
  loopBlock->print(depth + 1, true);
}

void BreakStatement::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "BreakStatement";
}

void ContinueStatement::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "ContinueStatement" << endl;
}

void VoidStatement::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "VoidStatement" << endl;
}

void ReturnStatement::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "ReturnStatement" << endl;
  returnExp->print(depth + 1, true);
}

void AST::print(int depth, bool isEnd) {
  cout << "ROOT" << endl;
  for (auto i : codeBlock)
    i->print(depth + 1, i == codeBlock.back());
}

Identifier::Identifier(const string name, bool isArray) : name(name), isArray(isArray) {};

Declare::Declare(Identifier name) : name(name) {};

VarDeclare::VarDeclare(Identifier name) : Declare(name) {};

VarDeclareWithInit::VarDeclareWithInit(Identifier name, Expression *value) : Declare(name), value(value) {};

ConstDeclare::ConstDeclare(Identifier name, Expression *value) : Declare(name), value(value) {}

ArrayDeclare::ArrayDeclare(Expression *size, Identifier name) : Declare(name), size(size) {};

ConstArray::ConstArray(Expression *size, Identifier &name, const vector<Expression *> value) : Declare(name),
                                                                                               size(size),
                                                                                               valueList(value) {};

ArrayDeclareWithInit::ArrayDeclareWithInit(Expression *size, Identifier &name, const vector<Expression *> value)
    : Declare(
    name), size(size), valueList(value) {};

FunctionDefine::FunctionDefine(int retType, Identifier name, FunctionDefArgList *args, Block *body) : retType(retType),
                                                                                                      name(name),
                                                                                                      args(args),
                                                                                                      body(body) {};

NumberExpression::NumberExpression(int value) : value(value) {};

LogicExpression::LogicExpression(Expression *left, int op, Expression *right) : leftExpr(left), rightExpr(right),
                                                                                op(op) {};

CalcExpression::CalcExpression(int op, Identifier name, Expression *right) : name(name), rightExpr(right), op(op) {};

ReturnStatement::ReturnStatement(Expression *exp) : returnExp(exp) {};