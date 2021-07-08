//
// Created by wnx on 2021/6/9.
//

#ifndef COMPILER_ASTNODE_H
#define COMPILER_ASTNODE_H

#include <iostream>
#include <vector>

#include"enum/enums.h"

using namespace std;

namespace compiler {
    namespace front::ast {
        class Node {
        public:
            AstNodeType nodetype;

            Node(AstNodeType type = NodeType) : nodetype(type) {}

            virtual void print(int depth = 0, bool isEnd = false);

            void printPrefix(int depth = 0, bool isEnd = false);
        };

        class Expression : public Node {
        public:
            Expression(AstNodeType type = ExpressionType) : Node(type) {}
        };

        class ArrayInitVal : public Expression {
        public:
            ArrayInitVal(AstNodeType type = ArrayInitValType) : Expression(type) {}

            vector<Expression *> initValList;

            void print(int depth, bool isEnd = false) override;
        };

        class Identifier : public Expression {
        public:
            string name;

            Identifier(string name, AstNodeType type = IdentifierType) : name(name), Expression(type) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

        class ArrayIdentifier : public Identifier {
        public:
            vector<Expression *> index;

            ArrayIdentifier(string name, AstNodeType type = ArrayIdentifierType) : Identifier(name, type) {};

            void print(int depth, bool isEnd) override;
        };

        class Stmt : public Expression {
        public:
            Stmt(AstNodeType type = StmtType) : Expression(type) {};
        };

        class Block : public Stmt {//语句块
        public:
            vector<Expression *> blockItem;

            Block(AstNodeType type = BlockType) : Stmt(type) {};

            void print(int depth = 0, bool isEnd = false);
        };

        /**
         * 变量声明语句
         */
        class Declare : public Node {
        public:
            Identifier *name;

            Declare(AstNodeType type = DeclareType) : Node(type) {};

            Declare(Identifier *name, AstNodeType type = DeclareType) : Node(type) {
                this->name = name;
            };

        };

        class VarDeclare : public Declare {
        public:

            VarDeclare(Identifier *name, AstNodeType type = VarDeclareType) : Declare(name, type) {};

            void print(int depth = 0, bool isEnd = false) override;

        };

        class VarDeclareWithInit : public Declare {
        public:
            Expression *value;

            VarDeclareWithInit(Identifier *name, Expression *value, AstNodeType type = VarDeclareWithInitType)
                    : Declare(name, type), value(value) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

        class ConstDeclare : public Declare {
        public:
            Expression *value;

            ConstDeclare(Identifier *name, Expression *value, AstNodeType type = ConstDeclareType) : Declare(name,
                                                                                                             type),
                                                                                                     value(value) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

        class ArrayDeclare : public Declare {
        public:

            ArrayDeclare(Identifier *name, AstNodeType type = ArrayDeclareType) : Declare(name, type) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

        class ConstArray : public Declare {
        public:
            ArrayInitVal *initVal;

            ConstArray(ArrayIdentifier *name, ArrayInitVal *initVal, AstNodeType type = ConstArrayType) : Declare(name,
                                                                                                              type),
                                                                                                      initVal(initVal) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

        class ArrayDeclareWithInit : public Declare {
        public:
            ArrayInitVal *initVal;

            //ArrayDeclareWithInit(AstNodeType type = ArrayDeclareWithInitType) : Declare(type) {};

            ArrayDeclareWithInit(ArrayIdentifier *name, ArrayInitVal *initVal,
                                 AstNodeType type = ArrayDeclareWithInitType) : Declare(name, type),
                                                                                initVal(initVal) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

        /**
         * 函数声明
         */
        class FunctionDefArg : public Expression {
        public:
            Identifier *name;
            int retType;

            FunctionDefArg(AstNodeType type = FunctionDefArgType) : Expression(type) {};

            FunctionDefArg(int retType, Identifier *name, AstNodeType type = FunctionDefArgType) : retType(retType),
                                                                                                   name(name),
                                                                                                   Expression(type) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

        class FunctionDefArgList : public Expression {
        public:
            vector<FunctionDefArg *> args;

            FunctionDefArgList(AstNodeType type = FunctionDefArgListType) : Expression(type) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

        class FunctionDefine : public Expression {
        public:
            int retType;
            Identifier *name;
            FunctionDefArgList *args;
            Block *body;

            FunctionDefine(AstNodeType type = FunctionDefineType) : Expression(type) {};

            FunctionDefine(int &retType, Identifier *name, FunctionDefArgList *args, Block *block,
                           AstNodeType type = FunctionDefineType) : retType(retType),
                                                                    name(name),
                                                                    args(args),
                                                                    body(block), Expression(type) {};

            void print(int depth = 0, bool isEnd = false) override;
        };


        /**
         * 函数调用
         */
        class FunctionCallArgList : public Expression {
        public:
            vector<Expression *> args;

            FunctionCallArgList(AstNodeType type = FunctionCallArgListType) : Expression(type) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

        class FunctionCall : public Expression {
        public:
            Identifier *name;
            FunctionCallArgList *args;

            FunctionCall(AstNodeType type = FunctionCallType) : Expression(type) {};

            FunctionCall(Identifier *name, FunctionCallArgList *args, AstNodeType type = FunctionCallType) : name(name),
                                                                                                             args(args),
                                                                                                             Expression(
                                                                                                                     type) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

        /**
         * 表达式
         */
        class CommaExpression : public Expression {//逗号表达式
        public:
            vector<Expression *> expr;

            CommaExpression(AstNodeType type = CommaExpressionType) : Expression(type) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

        class NumberExpression : public Expression {
        public:
            int value;

            NumberExpression(int value = 0, AstNodeType type = NumberExpressionType) : value(value),
                                                                                       Expression(type) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

        class BinaryExpression : public Expression {//逻辑表达式
        public:
            int op;
            Expression *leftExpr;
            Expression *rightExpr;

            BinaryExpression(Expression *left, int op, Expression *right, AstNodeType type = BinaryExpressionType)
                    : leftExpr(left), op(op), rightExpr(right), Expression(type) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

        class UnaryExpression : public Expression {
        public:
            int op;
            Expression *right;

            UnaryExpression(int op, Expression *right, AstNodeType type = UnaryExpressionType) : op(op), right(right),
                                                                                                 Expression(type) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

/*
 * 语句
 */
        class AssignStmt : public Stmt {//赋值表达式
        public:
            Identifier *name;
            Expression *rightExpr;

            AssignStmt(Identifier *inName, Expression *right, AstNodeType type = AssignStmtType) : name(inName),
                                                                                                   rightExpr(right),
                                                                                                   Stmt(type) {};

            void print(int depth = 0, bool isEnd = false) override;
        };


        class DeclareStatement : public Stmt {
        public:
            vector<Declare *> declareList;

            DeclareStatement(AstNodeType type = DeclareStatementType) : Stmt(type) {};

            DeclareStatement(vector<Declare *> declareList, AstNodeType type = DeclareStatementType) : declareList(
                    declareList),
                                                                                                       Stmt(type) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

        class IfStatement : public Stmt {
        public:
            Expression *cond;//
            Stmt *trueBlock;
            Stmt *elseBlock;

            IfStatement(Expression *cond, Stmt *trueBlock, Stmt *elseBlock, AstNodeType type = IfStatementType) : cond(
                    cond), trueBlock(trueBlock),
                                                                                                                  elseBlock(
                                                                                                                          elseBlock),
                                                                                                                  Stmt(type) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

        class WhileStatement : public Stmt {
        public:
            Expression *cond;
            Stmt *loopBlock;

            WhileStatement(Expression *cond, Stmt *loopBlock, AstNodeType type = WhileStatementType) : cond(cond),
                                                                                                       loopBlock(
                                                                                                               loopBlock),
                                                                                                       Stmt(type) {};

            void print(int depth = 0, bool isEnd = false) override;
        };

        class BreakStatement : public Stmt {
        public:
            BreakStatement(AstNodeType type = BreakStatemetType) : Stmt(type) {}

            void print(int depth = 0, bool isEnd = false) override;
        };

        class ContinueStatement : public Stmt {
        public:
            ContinueStatement(AstNodeType type = ContinueStatementType) : Stmt(type) {}

            void print(int depth = 0, bool isEnd = false) override;
        };

        class VoidStatement : public Stmt {//空语句
        public:
            VoidStatement(AstNodeType type = VoidStatementType) : Stmt(type) {}

            void print(int depth = 0, bool isEnd = false) override;
        };

        class ReturnStatement : public Stmt {
        public:
            Expression *returnExp;

            ReturnStatement(Expression *exp = NULL, AstNodeType type = ReturnStatementType) : returnExp(exp),
                                                                                              Stmt(type) {};

            void print(int depth = 0, bool isEnd = false) override;
        };


        class AST {
        public:
            vector<Node *> codeBlock;

            AST() {};

            void print(int depth = 0, bool isEnd = false);
        };
    }
}
#endif //COMPILER_ASTNODE_H
