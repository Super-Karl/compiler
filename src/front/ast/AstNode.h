//
// Created by wnx on 2021/6/9.
//

#ifndef COMPILER_ASTNODE_H
#define COMPILER_ASTNODE_H

#include <iostream>
#include <list>
#include <mid/recordTable/RecordTable.h>
#include <vector>

#include "enum/enums.h"

using namespace std;

namespace compiler {
  namespace front::ast {
    class Identifier;
    class ArrayIdentifier;

    using namespace compiler::mid::ir;
    class Node {
    public:
      AstNodeType nodetype;

      Node(AstNodeType type = NodeType) : nodetype(type) {}

      virtual ~Node();

      virtual void print(int depth = 0, bool isEnd = false);

      virtual void genIR(mid::ir::IRList &ir, RecordTable *record);

      void printPrefix(int depth = 0, bool isEnd = false);
    };

    class Expression : public Node {
    public:
      void genIR(IRList &ir, RecordTable *record) override;
      virtual int eval(RecordTable *record);
      virtual OperatorName evalOp(IRList &ir, RecordTable *record);
      virtual void ConditionAnalysis(IRList &ir, RecordTable *record, LabelIR *ifLabel, LabelIR *elLabel, bool trueJmp);
      Expression(AstNodeType type = ExpressionType) : Node(type) {}

      ~Expression() override = default;
    };

    class ArrayInitVal : public Expression {
    public:
      ArrayInitVal(AstNodeType type = ArrayInitValType) : Expression(type) {}

      ~ArrayInitVal();

      vector<Expression *> initValList;

      void print(int depth, bool isEnd = false) override;

      void storeArray(ArrayIdentifier *name, IRList &ir, RecordTable *record);
    };

    class Identifier : public Expression {
    public:
      string name;

      Identifier(string name, AstNodeType type = IdentifierType) : name(name), Expression(type){};

      void print(int depth = 0, bool isEnd = false) override;

      int eval(RecordTable *record) override;

      virtual OperatorName evalOp(IRList &ir, RecordTable *record) override;

      virtual vector<Expression *> getIndex() {
        vector<Expression *> index;
        return index;
      };

      virtual OperatorName evalIndex(IRList &ir, RecordTable *record) {
        throw std::runtime_error("this node cannot evalIndex");
      };
    };

    class ArrayIdentifier : public Identifier {
    public:
      vector<Expression *> index;

      ArrayIdentifier(string name, AstNodeType type = ArrayIdentifierType) : Identifier(name, type){};

      ~ArrayIdentifier() override;

      void print(int depth, bool isEnd) override;

      int eval(RecordTable *record) override;

      OperatorName evalOp(IRList &ir, RecordTable *record) override;

      vector<Expression *> getIndex() override {
        return this->index;
      };

      OperatorName evalIndex(IRList &ir, RecordTable *record) override;
      void storeRuntime(IRList &ir, RecordTable *record, OperatorName source);
    };

    class Stmt : public Expression {
    public:
      Stmt(AstNodeType type = StmtType) : Expression(type){};
    };

    class Block : public Stmt {//语句块
    public:
      list<Expression *> blockItem;

      Block(AstNodeType type = BlockType) : Stmt(type){};

      ~Block();

      void genIR(mid::ir::IRList &ir, RecordTable *record) override;

      void print(int depth = 0, bool isEnd = false);
    };

    /**
         * 变量声明语句
         */
    class Declare : public Node {
    public:
      Identifier *name;

      Declare(AstNodeType type = DeclareType) : Node(type){};

      Declare(Identifier *name, AstNodeType type = DeclareType) : Node(type) {
        this->name = name;
      };

      ~Declare();
    };

    class VarDeclare : public Declare {
    public:
      VarDeclare(Identifier *name, AstNodeType type = VarDeclareType) : Declare(name, type){};

      ~VarDeclare();

      void print(int depth = 0, bool isEnd = false) override;

      void genIR(IRList &ir, RecordTable *record) override;
    };

    class VarDeclareWithInit : public Declare {
    public:
      Expression *value;

      VarDeclareWithInit(Identifier *name, Expression *value, AstNodeType type = VarDeclareWithInitType)
          : Declare(name, type), value(value){};

      ~VarDeclareWithInit();

      void print(int depth = 0, bool isEnd = false) override;

      void genIR(IRList &ir, RecordTable *record) override;
    };

    class ConstDeclare : public Declare {
    public:
      Expression *value;

      ConstDeclare(Identifier *name, Expression *value, AstNodeType type = ConstDeclareType) : Declare(name,
                                                                                                       type),
                                                                                               value(value){};

      ~ConstDeclare();

      void print(int depth = 0, bool isEnd = false) override;
      void genIR(IRList &ir, RecordTable *record) override;
    };

    class ArrayDeclare : public Declare {
    public:
      ArrayIdentifier *arrayName;

      ArrayInitVal *initVal;

      ArrayDeclare(Identifier *name, AstNodeType type = ArrayDeclareType) : Declare(name, type) { initVal = new ArrayInitVal(); };

      ~ArrayDeclare();

      void print(int depth = 0, bool isEnd = false) override;

      void genIR(IRList &ir, RecordTable *record) override;
    };

    class ConstArray : public Declare {
    public:
      ArrayIdentifier *arrayName;
      ArrayInitVal *initVal;

      ConstArray(ArrayIdentifier *name, ArrayInitVal *initVal, AstNodeType type = ConstArrayType) : Declare(name,
                                                                                                            type),
                                                                                                    arrayName(name),
                                                                                                    initVal(initVal){};

      ~ConstArray();

      void print(int depth = 0, bool isEnd = false) override;

      void genIR(IRList &ir, RecordTable *record) override;
    };

    class ArrayDeclareWithInit : public Declare {
    public:
      ArrayIdentifier *arrayName;
      ArrayInitVal *initVal;

      ArrayDeclareWithInit(){};

      ArrayDeclareWithInit(ArrayIdentifier *name, ArrayInitVal *initVal,
                           AstNodeType type = ArrayDeclareWithInitType) : Declare(name, type),
                                                                          arrayName(name),
                                                                          initVal(initVal){};

      ~ArrayDeclareWithInit();

      void print(int depth = 0, bool isEnd = false) override;
      void genIR(IRList &ir, RecordTable *record) override;
    };

    /**
         * 函数声明
         */
    class FunctionDefArg : public Expression {
    public:
      Identifier *name;
      int retType;

      FunctionDefArg(AstNodeType type = FunctionDefArgType) : Expression(type){};

      FunctionDefArg(int retType, Identifier *name, AstNodeType type = FunctionDefArgType) : retType(retType),
                                                                                             name(name),
                                                                                             Expression(type){};

      ~FunctionDefArg();

      void print(int depth = 0, bool isEnd = false) override;
      void genIR(IRList &ir, RecordTable *record);
    };

    class FunctionDefArgList : public Expression {
    public:
      vector<FunctionDefArg *> args;

      FunctionDefArgList(AstNodeType type = FunctionDefArgListType) : Expression(type){};

      ~FunctionDefArgList();

      void print(int depth = 0, bool isEnd = false) override;
      void genIR(IRList &ir, RecordTable *record);
    };

    class FunctionDefine : public Expression {
    public:
      int retType;
      Identifier *name;
      FunctionDefArgList *args;
      Block *body;

      FunctionDefine(AstNodeType type = FunctionDefineType) : Expression(type){};

      FunctionDefine(int &retType, Identifier *name, FunctionDefArgList *args, Block *block,
                     AstNodeType type = FunctionDefineType) : retType(retType),
                                                              name(name),
                                                              args(args),
                                                              body(block), Expression(type){};

      ~FunctionDefine();

      void print(int depth = 0, bool isEnd = false) override;
      void genIR(IRList &ir, RecordTable *record);
    };


    /**
         * 函数调用
         */
    class FunctionCallArgList : public Expression {
    public:
      list<Expression *> args;

      FunctionCallArgList(AstNodeType type = FunctionCallArgListType) : Expression(type){};

      ~FunctionCallArgList();

      void print(int depth = 0, bool isEnd = false) override;
    };

    class FunctionCall : public Expression {
    public:
      Identifier *name;
      FunctionCallArgList *args;

      FunctionCall(AstNodeType type = FunctionCallType) : Expression(type){};

      FunctionCall(Identifier *name, FunctionCallArgList *args, AstNodeType type = FunctionCallType) : name(name),
                                                                                                       args(args),
                                                                                                       Expression(
                                                                                                           type){};

      ~FunctionCall();

      void print(int depth = 0, bool isEnd = false) override;

      OperatorName evalOp(IRList &ir, RecordTable *record);

      void genIR(IRList &ir, RecordTable *record) override;
    };

    /**
         * 表达式
         */
    class CommaExpression : public Expression {//逗号表达式
    public:
      list<Expression *> expr;

      CommaExpression(AstNodeType type = CommaExpressionType) : Expression(type){};

      void print(int depth = 0, bool isEnd = false) override;

      int eval(RecordTable *record);
    };

    class NumberExpression : public Expression {
    public:
      int value;

      NumberExpression(int value = 0, AstNodeType type = NumberExpressionType) : value(value),
                                                                                 Expression(type){};

      void print(int depth = 0, bool isEnd = false) override;

      int eval(RecordTable *record);

      OperatorName evalOp(IRList &ir, RecordTable *record);
    };

    class BinaryExpression : public Expression {//逻辑表达式
    public:
      int op;
      Expression *leftExpr;
      Expression *rightExpr;

      BinaryExpression(Expression *left, int op, Expression *right, AstNodeType type = BinaryExpressionType)
          : leftExpr(left), op(op), rightExpr(right), Expression(type){};

      ~BinaryExpression();

      void print(int depth = 0, bool isEnd = false) override;

      int eval(RecordTable *record) override;

      OperatorName evalOp(IRList &ir, RecordTable *record);

      void ConditionAnalysis(IRList &ir, RecordTable *record, LabelIR *ifLabel, LabelIR *elLabel, bool trueJmp);

      OperatorCode getRelOpCode();
      OperatorCode getAntiRelOpCode();
    };

    class UnaryExpression : public Expression {
    public:
      int op;
      Expression *right;

      UnaryExpression(int op, Expression *right, AstNodeType type = UnaryExpressionType) : op(op), right(right),
                                                                                           Expression(type){};

      ~UnaryExpression();

      void print(int depth = 0, bool isEnd = false) override;

      int eval(RecordTable *record);

      OperatorName evalOp(IRList &ir, RecordTable *record);
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
                                                                                             Stmt(type){};

      ~AssignStmt();

      void genIR(mid::ir::IRList &ir, RecordTable *record) override;

      void print(int depth = 0, bool isEnd = false) override;
    };


    class DeclareStatement : public Stmt {
    public:
      list<Declare *> declareList;

      DeclareStatement(AstNodeType type = DeclareStatementType) : Stmt(type){};

      DeclareStatement(list<Declare *> declareList, AstNodeType type = DeclareStatementType) : declareList(
                                                                                                   declareList),
                                                                                               Stmt(type){};

      ~DeclareStatement();

      void genIR(mid::ir::IRList &ir, RecordTable *record) override;

      void print(int depth = 0, bool isEnd = false) override;
    };

    class IfStatement : public Stmt {
    public:
      Expression *cond;//
      Stmt *trueBlock;
      Stmt *elseBlock;

      IfStatement(Expression *cond, Stmt *trueBlock, Stmt *elseBlock, AstNodeType type = IfStatementType) : cond(
                                                                                                                cond),
                                                                                                            trueBlock(trueBlock),
                                                                                                            elseBlock(
                                                                                                                elseBlock),
                                                                                                            Stmt(type){};

      ~IfStatement();

      void genIR(mid::ir::IRList &ir, RecordTable *record) override;

      void print(int depth = 0, bool isEnd = false) override;
    };

    class WhileStatement : public Stmt {
    public:
      Expression *cond;
      Stmt *loopBlock;

      WhileStatement(Expression *cond, Stmt *loopBlock, AstNodeType type = WhileStatementType) : cond(cond),
                                                                                                 loopBlock(
                                                                                                     loopBlock),
                                                                                                 Stmt(type){};

      ~WhileStatement();

      void genIR(mid::ir::IRList &ir, RecordTable *record) override;

      void print(int depth = 0, bool isEnd = false) override;
    };

    class BreakStatement : public Stmt {
    public:
      BreakStatement(AstNodeType type = BreakStatemetType) : Stmt(type) {}

      void genIR(mid::ir::IRList &ir, RecordTable *record) override;

      void print(int depth = 0, bool isEnd = false) override;
    };

    class ContinueStatement : public Stmt {
    public:
      ContinueStatement(AstNodeType type = ContinueStatementType) : Stmt(type) {}

      void genIR(mid::ir::IRList &ir, RecordTable *record) override;

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
                                                                                        Stmt(type){};

      ~ReturnStatement();

      void genIR(mid::ir::IRList &ir, RecordTable *record) override;

      void print(int depth = 0, bool isEnd = false) override;
    };


    class AST {
    public:
      list<Node *> codeBlock;

      AST(){};

      ~AST();

      void print(int depth = 0, bool isEnd = false);

      void genIR(IRList &ir, RecordTable *record);
    };
  }// namespace front::ast
}// namespace compiler
#endif//COMPILER_ASTNODE_H
