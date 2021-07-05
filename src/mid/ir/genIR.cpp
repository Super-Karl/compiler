//
// Created by wnx on 2021/7/5.
//
#include<iostream>
#include "front/ast/AstNode.h"


using namespace compiler;

void front::ast::Node::genIR(mid::ir::IRList &ir) {
  std::cout << "this node can't genIR" << std::endl;

}

void front::ast::AssignExpression::genIR(mid::ir::IRList &ir) {

  auto operatorName = mid::ir::OperatorName();
}

void front::ast::Expression::genIR(mid::ir::IRList &ir) {

}