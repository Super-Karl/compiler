//
// Created by wnx on 2021/6/9.
//

#include "AstNode.h"
#include <iostream>
#include <iterator>

using namespace compiler::front::ast;

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
  cout << this->nodetype << " "
       << "Identifier: " + this->name << endl;
};

void ArrayIdentifier::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "ArrayIdentifier: " + this->name << endl;
  for (auto i : index)
    i->print(depth + 1, i == index.back());
}

void Block::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "Block" << endl;
  for (auto i : blockItem)
    i->print(depth + 1, i == blockItem.back());
};

void VarDeclare::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "VarDeclare" << endl;
  this->name->print(depth + 1, true);
}

void VarDeclareWithInit::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "VarDeclareWithInit" << endl;
  this->name->print(depth + 1, false);
  this->value->print(depth + 1, true);
}

void ConstDeclare::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "ConstDeclare" << endl;
  this->name->print(depth + 1, false);
  this->value->print(depth + 1, true);
}

void ArrayDeclare::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "ArrayDeclare: " << endl;
  this->name->print(depth + 1, false);
}

void ConstArray::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "ConstArray" << endl;
  this->name->print(depth + 1, false);
  initVal->print(depth + 1, true);
}

void ArrayDeclareWithInit::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "ArrayDeclareWithInit" << endl;
  this->name->print(depth + 1, false);
  initVal->print(depth + 1, true);
}

void ArrayInitVal::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "ArrayInitVal: " << endl;
  for (auto i : initValList)
    i->print(depth + 1, i == initValList.back());
}

void FunctionDefArg::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "FunctionDefArg Type: " << retType << endl;
  this->name->print(depth + 1, true);
}

void FunctionDefArgList::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "FunctionDefArgList" << endl;

  for (auto i : args)
    i->print(depth + 1, i == args.back());
}

void FunctionDefine::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "FunctionDefine returnType: " << retType << endl;
  name->print(depth + 1, false);
  args->print(depth + 1, isEnd);
  if (body != NULL)
    body->print(depth + 1, true);
}

void FunctionCallArgList::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "FunctionCallArgList" << endl;
  for (auto i : args)
    i->print(depth + 1, i == args.back());
}

void FunctionCall::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "FunctionCall" << endl;
  name->print(depth + 1, false);
  args->print(depth + 1, true);
}

void CommaExpression::print(int depth, bool isEnd) {
  for (auto i : expr)
    i->print(depth + 1, i == expr.back());
}

void NumberExpression::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "Number: " << value << endl;
}

void BinaryExpression::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "BinaryExpression "
       << "opcode: " << this->op << endl;
  leftExpr->print(depth + 1, false);
  rightExpr->print(depth + 1, true);
}

void UnaryExpression::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "UnaryExpression Operand: " << op << endl;
  right->print(depth + 1, true);
}

void AssignStmt::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "Assign: " << endl;
  name->print(depth + 1, false);
  rightExpr->print(depth + 1, true);
}

void DeclareStatement::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "DeclareStatement" << endl;
  for (auto i : declareList)
    i->print(depth + 1, i == declareList.back());
}

void IfStatement::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "IfStatement" << endl;
  cond->print(depth + 1, false);
  trueBlock->print(depth + 1, false);
  elseBlock->print(depth + 1, true);
}

void WhileStatement::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "WhileStatement" << endl;
  cond->print(depth + 1, false);
  loopBlock->print(depth + 1, true);
}

void BreakStatement::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "BreakStatement";
}

void ContinueStatement::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "ContinueStatement" << endl;
}

void VoidStatement::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "VoidStatement" << endl;
}

void ReturnStatement::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << this->nodetype << " "
       << "ReturnStatement" << endl;
  if (this->returnExp != NULL)
    returnExp->print(depth + 1, true);
}

void AST::print(int depth, bool isEnd) {
  cout << " "
       << "ROOT" << endl;
  for (auto i : codeBlock)
    i->print(depth + 1, i == codeBlock.back());
}
//destructor
Node::~Node() {
}

ArrayInitVal::~ArrayInitVal() {
  for (auto &i : initValList) {
    delete i;
    i = NULL;
  }
}

ArrayIdentifier::~ArrayIdentifier() {
  for (auto &i : index) {
    delete i;
    i = NULL;
  }
}

Block::~Block() {
  for (auto &i : blockItem) {
    delete i;
    i = NULL;
  }
}

Declare::~Declare() {
  delete name;
  name = NULL;
}

VarDeclare::~VarDeclare() {
}

VarDeclareWithInit::~VarDeclareWithInit() {
  delete value;
  value = NULL;
}

ConstDeclare::~ConstDeclare() {
  delete value;
  value = NULL;
}

ArrayDeclare::~ArrayDeclare() {
}

ConstArray::~ConstArray() {
  delete initVal;
}

ArrayDeclareWithInit::~ArrayDeclareWithInit() {
  delete initVal;
}

FunctionDefArg::~FunctionDefArg() {
  delete name;
}

FunctionCallArgList::~FunctionCallArgList() {
  for (auto &i : args) {
    delete i;
  }
}

FunctionDefArgList::~FunctionDefArgList() {
  for (auto &i : args) {
    delete i;
  }
}

FunctionDefine::~FunctionDefine() {
  delete name;
  delete args;
  delete body;
}

FunctionCall::~FunctionCall() {
  delete name;
  delete args;
}

BinaryExpression::~BinaryExpression() {
  delete leftExpr;
  delete rightExpr;
}

UnaryExpression::~UnaryExpression() {
  delete right;
}

AssignStmt::~AssignStmt() {
  delete name;
  delete rightExpr;
}

DeclareStatement::~DeclareStatement() {
  for (auto &i : declareList) {
    delete i;
  }
}

IfStatement::~IfStatement() {
  delete trueBlock;
  delete elseBlock;
  delete cond;
}

WhileStatement::~WhileStatement() {
  delete cond;
  delete loopBlock;
}

ReturnStatement::~ReturnStatement() {
  delete returnExp;
}

AST::~AST() {
  for (auto &i : codeBlock) {
    delete i;
  }
}