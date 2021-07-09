//
// Created by karl on 2021/7/7.
//
#pragma once
#ifndef COMPILER_ASTPASS_H
#define COMPILER_ASTPASS_H

#include <front/ast/AstNode.h>
#include <unordered_map>

using Hash = std::unordered_map<string, int>;
namespace compiler::astpassir {
    bool caluExpersion(compiler::front::ast::Expression *exp, int &result);

    void FirstPassRoot(compiler::front::ast::AST *root, Hash constTbale);

    void FirstPassNode(compiler::front::ast::Block *node, Hash constTbale);

    void FirstPassStmt(compiler::front::ast::Stmt *stmt, Hash constTbale);

    compiler::front::ast::Expression *FirstPassExpr(compiler::front::ast::Expression *expr, Hash constTbale);
}

#endif //COMPILER_ASTPASS_H
