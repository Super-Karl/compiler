//
// Created by karl on 2021/7/7.
//

#ifndef COMPILER_ENUMS_H
#define COMPILER_ENUMS_H

enum AstNodeType {
    NodeType = 0,
    ExpressionType,
    ArrayInitValType,
    BinaryExpressionType,
    FunctionDefArgType,
    FunctionDefArgListType,
    FunctionDefineType,
    FunctionCallArgListType,
    FunctionCallType,
    CommaExpressionType,
    NumberExpressionType,
    LogicExpressionType,
    UnaryExpressionType,
    CalcExpressionType,
    StmtType,
    AssignStmtType,
    BlockType,
    DeclareStatementType,
    IfStatementType,
    WhileStatementType,
    BreakStatemetType,
    ContinueStatementType,
    VoidStatementType,
    ReturnStatementType,
    IdentifierType,
    ArrayIdentifierType,
    DeclareType,
    VarDeclareType,
    VarDeclareWithInitType,
    ConstDeclareType,
    ArrayDeclareType,
    ConstArrayType,
    ArrayDeclareWithInitType
};
#endif //COMPILER_ENUMS_H
