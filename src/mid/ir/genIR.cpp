//
// Created by wnx on 2021/7/5.
//
#include<iostream>
#include "front/ast/AstNode.h"
#include "mid/ir/ir.h"

using namespace compiler;

void front::ast::Node::genIR(mid::ir::IRList &ir) {
  std::cout << "this node can't genIR" << std::endl;

}

void front::ast::AssignExpression::genIR(mid::ir::IRList &ir) {

  auto operatorName = mid::ir::OperatorName();
}

void front::ast::FunctionCall::genIR(mid::ir::IRList &ir) {
  auto funCallIR = new mid::ir::FunCallIR(this->name->name);
  for(auto i : args->args){

  }
}