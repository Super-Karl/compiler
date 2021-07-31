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
    class VAR;

    int tableFind(vector<VAR>&vartable,string name);

    void printASM(list<compiler::back::INS *> &list);

    list<compiler::back::INS *> generateBack(compiler::front::ast::AST *root);

    void generateBackFunction(vector<VAR>vartable, list<INS *> &backlist, compiler::front::ast::FunctionDefine *func);
    //计算表达式
    void generateExpression(vector<VAR>&vartable,list<INS *> &backlist, compiler::front::ast::Expression *expression);
    //pos表示是计算当前是上一级的左表达式（-1），右表达式（1），最高表达式（0）
    void generateBinaryExpression(vector<VAR>&vartable,list<INS *> &backlist, compiler::front::ast::BinaryExpression *expression,int pos);

    void generateBackArray(vector<VAR>&vartable,list<INS *> &backlist, compiler::front::ast::Declare *array);
    //获得数组标识符的地址放到r6
    void getArrayIdentAddress(vector<VAR>&vartable,list<INS *> &backlist,compiler::front::ast::ArrayIdentifier* arrayIdentifier);
}
#endif //COMPILER_GENERATEBACK_H
