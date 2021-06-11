//
// Created by wnx on 2021/6/11.
//
#include "ast/AstNode.h"
#include <iostream>
using namespace parser::ast;
using namespace std;


int main(){
  AST *root = new AST();
  vector<Declare*> v;
  auto *a = new VarDeclareWithInit(Identifier("a"),new NumberExpression(5));
  v.push_back(a);
  auto *b = new VarDeclare(Identifier("b"));
  v.push_back(b);
  auto *stmt = new DeclareStatement(v);
  root->codeBlock.push_back(stmt);
  root->codeBlock.push_back(a);
  root->codeBlock.push_back(b);
  root->print();
  return 0;
}