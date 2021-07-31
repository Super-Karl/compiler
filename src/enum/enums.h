//
// Created by karl on 2021/7/7.
//

#ifndef YYTOKENTYPE
# define YYTOKENTYPE
enum yytokentype
{
    IDENTIFIER = 258,
    NUM = 259,
    IF = 260,
    ELSE = 261,
    BREAK = 262,
    CONTINUE = 263,
    RETURN = 264,
    WHILE = 265,
    CONST = 266,
    INT = 267,
    VOID = 268,
    ADD = 269,
    SUB = 270,
    MOD = 271,
    MUL = 272,
    DIV = 273,
    NOT_EQUAL = 274,
    LT = 275,
    GT = 276,
    LE = 277,
    GE = 278,
    AND_OP = 279,
    OR_OP = 280,
    EQ = 281,
    NE = 282,
    LBRACKET = 283,
    RBRACKET = 284,
    LBRACE = 285,
    RBRACE = 286,
    LSQARE = 287,
    RSQARE = 288,
    ASSIGN = 289,
    COLON = 290,
    COMMA = 291,
    SEMI = 292
};
#endif

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

enum SourceType{
    VarSourceType,
    NumSourceType
};

enum INSType{
    macroType,
    INSlableType,
    global,
    globalarray,
    funcLable,
    stmdb,
    ldmia,
    mov32,
    mov16,
    str,
    ldr,
    option,
    mla,
    bl
};
#endif //COMPILER_ENUMS_H
