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

    void generateBackFunction(list<INS *> &backlist, compiler::front::ast::FunctionDefine *func);

    void generateBackArray(vector<VAR>&vartable,list<INS *> &backlist, compiler::front::ast::Declare *array);
    //获得数组标识符的地址放到r6
    int getArrayIdentAddress(vector<VAR>&vartable,list<INS *> &backlist,compiler::front::ast::ArrayIdentifier* expression);
    //处理函数调用
    void generateFuncCall(vector<VAR>&vartable,list<INS *> &backlist,compiler::front::ast::FunctionCall* functionCall);
    //处理if_while的block
    void generateBlock(vector<VAR>&vartable,list<INS *> &backlist,compiler::front::ast::Block* block,int nowWhileId);
    //处理if_while的单条语句
    void generateStmt(vector<VAR>&vartable,list<INS *> &backlist,compiler::front::ast::Node* stmt,int nowWhileId);

    //计算函数，返回正数或0为结果存放的寄存器值，负数为栈地址相对fp的偏移量
    //新计算函数
    int generateExp(vector<VAR>&vartable,list<INS *> &backlist, compiler::front::ast::Expression *expression);
    //新二叉表达式
    int generateBinaryExp(vector<VAR>&vartable,list<INS *> &backlist, compiler::front::ast::BinaryExpression *expression);
    //新单元表达式
    int generateUnaryExp(vector<VAR>&vartable,list<INS *> &backlist, compiler::front::ast::UnaryExpression *expression);
    //计算子数组的地址
    int getArrayArgAddress(vector<VAR>&vartable,list<INS *> &backlist,compiler::front::ast::ArrayIdentifier* expression);
}
#endif //COMPILER_GENERATEBACK_H
