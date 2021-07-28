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
  cout << "Identifier: " + this->name << endl;
};

void ArrayIdentifier::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "ArrayIdentifier: " + this->name << endl;
  for (auto i : index)
    i->print(depth + 1, i == index.back());
}

void Block::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "Block" << endl;
  for (auto i : blockItem)
    i->print(depth + 1, i == blockItem.back());
}

void VarDeclare::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "VarDeclare" << endl;
  this->name->print(depth + 1, true);
}

void VarDeclareWithInit::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "VarDeclareWithInit" << endl;
  this->name->print(depth + 1, false);
  this->value->print(depth + 1, true);
}

void ConstDeclare::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "ConstDeclare" << endl;
  this->name->print(depth + 1, false);
  this->value->print(depth + 1, true);
}

void ArrayDeclare::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "ArrayDeclare: " << endl;
  this->name->print(depth + 1, false);
}

void ConstArray::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "ConstArray" << endl;
  this->name->print(depth + 1, false);
  cout << "shape" << endl;
  initVal->print(depth + 1, true);
}

void ArrayDeclareWithInit::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "ArrayDeclareWithInit" << endl;
  this->name->print(depth + 1, false);
  initVal->print(depth + 1, true);
}

void ArrayInitVal::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "ArrayInitVal: " << endl;
  for (auto i : initValList)
    i->print(depth + 1, i == initValList.back());
}

void FunctionDefArg::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "FunctionDefArg Type: " << type << endl;
  this->name->print(depth + 1, true);

}

void FunctionDefArgList::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "FunctionDefArgList" << endl;

  for (auto i : args)
    i->print(depth + 1, i == args.back());
}

void FunctionDefine::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "FunctionDefine";
  cout << " returnType: " << retType << endl;
  name->print(depth + 1, false);
  args->print(depth + 1, isEnd);
  if (body != NULL)
    body->print(depth + 1, true);
}

void FunctionCallArgList::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "FunctionCallArgList" << endl;
  for (auto i : args)
    i->print(depth + 1, i == args.back());
}

void FunctionCall::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "FunctionCall" << endl;
  name->print(depth + 1, false);
  args->print(depth + 1, true);
}

void CommaExpression::print(int depth, bool isEnd) {
  for (auto i : expr)
    i->print(depth + 1, i == expr.back());
}

void NumberExpression::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "Number: " << value << endl;
}

void BinaryExpression::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "BinaryExpression " << "opcode: " << this->op << endl;
  leftExpr->print(depth + 1, false);
  rightExpr->print(depth + 1, true);
}

void UnaryExpression::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "UnaryExpression Operand: " << op << endl;
  right->print(depth + 1, true);
}

void AssignStmt::print(int depth, bool isEnd) {
  this->printPrefix(depth, isEnd);
  cout << "Mov: " << endl;
  name->print(depth + 1, false);
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
  if (this->returnExp != NULL)
    returnExp->print(depth + 1, true);
}

void AST::print(int depth, bool isEnd) {
  cout << "ROOT" << endl;
  for (auto i : codeBlock)
    i->print(depth + 1, i == codeBlock.back());
}
