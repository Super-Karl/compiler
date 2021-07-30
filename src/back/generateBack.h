//
// Created by karl on 2021/7/29.
//

#ifndef COMPILER_GENERATEBACK_H
#define COMPILER_GENERATEBACK_H

#include <list>
#include <unordered_map>
#include "back.h"
#include "../front/ast/AstNode.h"


namespace compiler::back {
    void printASM(list<compiler::back::INS *> &list);

    list<compiler::back::INS *> generateBack(compiler::front::ast::AST *root);

    void generateBackFunction(list<INS *> &backlist, compiler::front::ast::FunctionDefine *func);
    //pos表示是计算当前是上一级的左表达式（-1），右表达式（1），最高表达式（0）
    void generateBinaryExpression(list<INS *> &backlist, compiler::front::ast::BinaryExpression *expression,int pos);
}
#endif //COMPILER_GENERATEBACK_H
