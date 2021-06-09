//
// Created by wnx on 2021/6/9.
//

#include "AstNode.h"

using namespace parser::ast;

Identifier::Identifier(const string name,bool isArray) :name(name),isArray(isArray){};

Declare::Declare(Identifier name, bool isInit) : name(name),isInit(isInit){};

VarDeclare::VarDeclare(Identifier name, bool isInit) : Declare(name,isInit){}

VarDeclare::VarDeclare(Identifier name, bool isInit, Expression value): Declare(name,isInit),value(value) {}

ConstDeclare::ConstDeclare(Identifier name, Expression value, bool isInit) : Declare(name,isInit),value(value){}

ArrayDeclare::ArrayDeclare(Expression size, Identifier name,bool isInit) : Declare(name,isInit),size(size){};

ConstArray::ConstArray(Expression size, Identifier name, const vector<Expression> value, bool isInit) : Declare(name,isInit),size(size),valueList(value){};

ArrayDeclareWithInit::ArrayDeclareWithInit(Expression size, Identifier name, const vector<Expression> value,
                                           bool isInit) : Declare(name,isInit),size(size),valueList(value){};

FunctionDefine::FunctionDefine(int retType, Identifier name, FunctionDefArgList args, Block body) :retType(retType), name(name),
                                                                                                 args(args),body(body){};

NumberExpression::NumberExpression(int value) :value(value){};

LogicExpression::LogicExpression(Expression left, int op, Expression right) : leftExpr(left),rightExpr(right),op(op){};

CalcExpression::CalcExpression(int op, Identifier name, Expression right) : name(name),rightExpr(right),op(op){};

ReturnStatement::ReturnStatement(Expression *exp ) :returnExp(exp){};
