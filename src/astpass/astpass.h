//
// Created by karl on 2021/7/7.
//
#pragma once
#ifndef COMPILER_ASTPASS_H
#define COMPILER_ASTPASS_H

#include <front/ast/AstNode.h>

namespace compiler::astpassir {
    compiler::front::ast::AST *FirstPass(compiler::front::ast::AST *root);

}

#endif //COMPILER_ASTPASS_H
