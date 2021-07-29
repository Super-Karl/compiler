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
    UnaryExpressionType,
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

enum LableType{
    FunctionType,
    LoopType,
    IfType,
    ElseType,
    WhileType
};

enum ThreeSourceType{
    ADD3,
    SUB3,
    STR3,
    LDR
};

enum TwoSourceTyoe{
    MOV,
};
#endif //COMPILER_ENUMS_H
