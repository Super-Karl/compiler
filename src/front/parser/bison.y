%{
#include "front/ast/AstNode.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "controller/controller.h"
//using parser::ast;

using compiler::controller::generator::root;
using namespace compiler;
extern int yydebug;

extern int yylex();
extern int yyget_lineno();
extern int yylex_destroy();
#define YYDEBUG 1
#define YYERROR_VERBOSE true

void yyerror(const char *s){
    printf("line %d : error: %s\n", yyget_lineno(),s);
    yylex_destroy();
    if (!yydebug)
        std::exit(1);
}

%}

%union {
    int token;
    compiler::front::ast::Identifier* ident;
    compiler::front::ast::Expression* expr;
    compiler::front::ast::Stmt* stmt;
    compiler::front::ast::FunctionCall* functCall;
    compiler::front::ast::AST* root;
    compiler::front::ast::Declare* declare;
    compiler::front::ast::DeclareStatement* declStmt;
    compiler::front::ast::ConstDeclare *conDecl;
    compiler::front::ast::FunctionDefine* funcDef;
    compiler::front::ast::Block* block;
    compiler::front::ast::Expression* expression;
    compiler::front::ast::FunctionCallArgList* funcCallArgList;
    compiler::front::ast::FunctionDefArgList* funcdefParamList;
    compiler::front::ast::FunctionDefArg* funcdefParam;
    compiler::front::ast::ArrayInitVal* arrayInitList;
    compiler::front::ast::ArrayIdentifier* arrayident;
    std::string *string;
}

%token <string> IDENTIFIER NUM
%token <token> IF ELSE BREAK CONTINUE RETURN WHILE 
%token <token> CONST INT VOID
%token <token> ADD SUB MOD MUL DIV NOT_EQUAL LT GT LE GE AND_OP OR_OP EQ NE
%token <token> AND OR
%token <token> LBRACKET RBRACKET LBRACE RBRACE LSQARE RSQARE ASSIGN COLON COMMA SEMI

%type <root> compUnit
%type <arrayident> ArrayIdent
%type <declare>VarDef DefVal DefArray ConstDef ConstDefVal ConstDefArray
%type <token> RelOP UnaryOp MulOp AddOp BType
%type <funcDef> FuncDef
%type <stmt> Stmt IfStmt WhileStmt BreakStmt ContinueStmt VoidStmt Assignment BlockItem ReturnStmt functStmt
%type <arrayInitList> ArrayInitList ListExp
%type <funcCallArgList> FunctionCallArgList 
%type <funcdefParamList> FuncParamList
%type <funcdefParam> FuncParam FuncDefVal FuncDefArr
%type <block> Block BlockItems
%type <declStmt>Decl VarDecl ConstDecl
%type <ident> Ident LVal
%type <expr> Number Exp InitVal LOrExp LAndExp EqExp AddExp MulExp PrimaryExp RelExp UnaryExp FunctCall ConstInitialVal Cond FuncCallArg
%start compUnit 
%%

compUnit: compUnit Decl {$$->codeBlock.push_back($<declare>2); }
    | compUnit FuncDef {$$->codeBlock.push_back($<funcDef>2);}
    | Decl{root = new front::ast::AST();$$=root;$$->codeBlock.push_back($<declare>1);}
    | FuncDef{root = new front::ast::AST();$$=root;$$->codeBlock.push_back($<funcDef>1);}
    ;

Decl: ConstDecl SEMI {$$ = $1;}
    | VarDecl SEMI {$$ = $1;}
    ;

BType: INT;

ConstDecl: CONST BType ConstDef{ $$ = new front::ast::DeclareStatement();$$->declareList.push_back($3);}
    | ConstDecl COMMA ConstDef{$$->declareList.push_back($3);} 
    ;

ConstDef: ConstDefVal
    | ConstDefArray
    ;

ConstDefVal: Ident ASSIGN ConstInitialVal{$$ = new front::ast::ConstDeclare($1,$3);}
    ;

ConstInitialVal:AddExp
    ;

ConstDefArray: CONST ArrayIdent ASSIGN ArrayInitList{$$ = new front::ast::ConstArray($2,$4);}
    ;

VarDecl: BType VarDef {$$ = new front::ast::DeclareStatement();$$->declareList.push_back($2);}
    | VarDecl COMMA VarDef{$$->declareList.push_back($3);} 
    ;

VarDef: DefVal 
    | DefArray
    ;

DefVal: Ident ASSIGN InitVal { $$ = new front::ast::VarDeclareWithInit($1,$3);}
    | Ident {$$ = new front::ast::VarDeclare($1);}

InitVal: Exp
    ;

DefArray: ArrayIdent ASSIGN ArrayInitList{$$ = new front::ast::ArrayDeclareWithInit($1,$3);}
    | ArrayIdent {$$ = new front::ast::ArrayDeclare($1);}
    ;

ArrayIdent: ArrayIdent LSQARE Exp RSQARE {$$->index.push_back($3);}
    | Ident LSQARE Exp RSQARE {$$ = new front::ast::ArrayIdentifier($1->name);$$->index.push_back($3);}
    ;

ArrayInitList:LBRACE ListExp RBRACE {$$ = $2;}
    |LBRACE RBRACE {new front::ast::ArrayInitVal();}
    ;

ListExp: ListExp COMMA ArrayInitList {$$->initValList.push_back($3);}
    | ListExp COMMA InitVal{$$->initValList.push_back($3);}
    | ArrayInitList {$$ = new front::ast::ArrayInitVal();$$->initValList.push_back($1);}
    | InitVal {$$ = new front::ast::ArrayInitVal();$$->initValList.push_back($1);}
    ;

FuncDef: BType Ident LBRACKET FuncParamList RBRACKET Block {$$ = new front::ast::FunctionDefine($1,$2,$4,$6);}
    | BType Ident LBRACKET RBRACKET Block {$$ = new front::ast::FunctionDefine($1,$2,(new front::ast::FunctionDefArgList()),$5);}
    | VOID Ident LBRACKET FuncParamList RBRACKET Block{ $$ = new front::ast::FunctionDefine($1,$2,$4,$6); }
    | VOID Ident LBRACKET RBRACKET Block{ $$ = new front::ast::FunctionDefine($1,$2,(new front::ast::FunctionDefArgList()),$5);}
    ;

FuncParamList: FuncParamList COMMA FuncParam {$$->args.push_back($3);}
    | FuncParam {$$ = new front::ast::FunctionDefArgList();$$->args.push_back($1);}
    ;

FuncParam: FuncDefVal 
    | FuncDefArr
    ;

FuncDefVal: BType Ident {$$ = new front::ast::FunctionDefArg($1,$2);}
    ;

FuncDefArr: BType ArrayIdent {$$ = new front::ast::FunctionDefArg($1,$2);}
    ;

Block: LBRACE RBRACE{ $$ = new front::ast::Block();}
    |   LBRACE BlockItems RBRACE{$$ = $2;}
    ;

BlockItems:BlockItems BlockItem {$$ = $1;$$->blockItem.push_back($2);}
    |BlockItem {$$ = new front::ast::Block();$$->blockItem.push_back($1);}
    ;

BlockItem: Decl
    | Stmt
    ;

Stmt: Assignment SEMI {$$ = $1;}
    | Block
    | IfStmt
    | WhileStmt
    | BreakStmt SEMI { $$ = $1;}
    | ContinueStmt SEMI { $$ = $1;}
    | ReturnStmt SEMI {$$ = $1;}
    | functStmt SEMI {$$ = $1;}
    | VoidStmt 
    ;

functStmt:FunctCall 
    ;

Assignment:LVal ASSIGN Exp {$$ = new front::ast::AssignStmt($1,$3);}
    ;

IfStmt: IF LBRACKET Cond RBRACKET Stmt ELSE Stmt {$$= new front::ast::IfStatement($3,$5,$7);}
    | IF LBRACKET Cond RBRACKET Stmt {$$ = new front::ast::IfStatement($3,$5,(new front::ast::VoidStatement()));}
    ;

WhileStmt: WHILE LBRACKET Cond RBRACKET Stmt {$$ = new front::ast::WhileStatement($3,$5);}
    ;

BreakStmt: BREAK SEMI {$$ =  new front::ast::BreakStatement();}
    ;

ContinueStmt: CONTINUE SEMI {$$ = new front::ast::ContinueStatement();}
    ;

VoidStmt:Exp SEMI {$$ = new front::ast::VoidStatement();}
    |SEMI {$$ = new front::ast::VoidStatement();}
    ;

ReturnStmt:RETURN Exp {$$ = new front::ast::ReturnStatement($2);}
    |RETURN {$$ = new front::ast::ReturnStatement();}
    ;
Exp: AddExp
    ;

Cond: LOrExp
    ;

LOrExp:LOrExp OR_OP LAndExp {$$ = new front::ast::BinaryExpression($1,$2,$3);}
    |LAndExp
    ;

LAndExp:EqExp AND_OP LAndExp {$$ = new front::ast::BinaryExpression($1,$2,$3);}
    |EqExp
    ;

EqExp:EqExp EQ RelExp {$$ = new front::ast::BinaryExpression($1,$2,$3);}
    |EqExp NE RelExp {$$ = new front::ast::BinaryExpression($1,$2,$3);}
    |RelExp
    ;

RelExp:AddExp RelOP RelExp {$$ = new front::ast::BinaryExpression($1,$2,$3);}
    |AddExp
    ;
AddExp: AddExp AddOp MulExp {$$ = new front::ast::BinaryExpression($1,$2,$3);}
    | MulExp
    ;

MulExp: MulExp MulOp UnaryExp{$$ = new front::ast::BinaryExpression($1,$2,$3);}
    | UnaryExp
    ;

UnaryExp: UnaryOp UnaryExp {$$ = new front::ast::UnaryExpression($1,$2);}
    | FunctCall
    | PrimaryExp
    ;

FunctCall: Ident LBRACKET FunctionCallArgList RBRACKET {$$ = new front::ast::FunctionCall($1,$3);}
    | Ident LBRACKET RBRACKET {$$ = new front::ast::FunctionCall($1,(new front::ast::FunctionCallArgList()));}
    ;

FunctionCallArgList: FunctionCallArgList COMMA FuncCallArg {$$->args.push_back($3);}
    | FuncCallArg {$$ = new front::ast::FunctionCallArgList(); $$->args.push_back($1);}
    ;

FuncCallArg:AddExp 
    ;

PrimaryExp: Number 
    | LVal
    | LBRACKET Exp RBRACKET {$$ = $2;}
    ;

LVal: Ident
    | ArrayIdent
    ;

AddOp: ADD
    | SUB
    ;

MulOp: MUL
    | DIV
    | MOD
    ;

UnaryOp: ADD
    | SUB
    | NOT_EQUAL
    ;

RelOP:LT
    |GT
    |LE
    |GE
    ;

Number: NUM {$$ = new front::ast::NumberExpression(std::stoi(*$1,0,0));}
    ;

Ident: IDENTIFIER {$$ = new front::ast::Identifier(*$1);}
    ;