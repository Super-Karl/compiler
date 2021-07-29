//
// Created by karl on 2021/7/29.
//

#ifndef COMPILER_GENERATEBACK_H
#define COMPILER_GENERATEBACK_H
#include "back.h"
#include <list>
#include <front/ast/AstNode.h>
#include <unordered_map>

namespace compiler::back {
    void printASM(list<compiler::back::INS *> &list);

    list<compiler::back::INS *> generateBack(compiler::front::ast::AST *root);

    void generateBackFunction(list<INS *> &backlist, compiler::front::ast::FunctionDefine *func);
}
#endif //COMPILER_GENERATEBACK_H
