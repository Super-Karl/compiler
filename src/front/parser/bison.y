%{

#include "front/ast/AstNode.h"
#include <cstdio>
#include <cstdlib>

//using parser::ast;
using namespace compiler;
extern int yylex();
extern int yydebug;
extern int yyget_lineno();
extern int yylex_destroy();
void yyerror(const char *s){
    printf("line %d : error: %s\n", yyget_lineno(),s);
    yylex_destroy();
    if (!yydebug)
        std::exit(1);
}

front::ast::AST* root;
%}

%union {
    int token;
    front::ast::Identifier* ident;
    front::ast::Expression* expr;
    front::ast::Stmt* stmt;
    front::ast::FunctionCall* functCall;
    front::ast::AST* root;
    front::ast::Declare* declare;
    front::ast::DeclareStatement* declStmt; 
    front::ast::ConstDeclare *conDecl;
    front::ast::FunctionDefine* funcDef;
    front::ast::Block* block;
    front::ast::Expression* expression;
    front::ast::FunctionCallArgList* funcCallArgList;
    front::ast::FunctionDefArgList* funcdefParamList;
    front::ast::FunctionDefArg* funcdefParam;
    std::string *string;
}

%token <token> IF ELSE BREAK CONTINUE RETURN WHILE 
%token <token> INT VOID CONST
%token <token> ADD SUB MOD MUL DIV NOT_EQUAL LT GT LE GE AND_OP OR_OP EQ NE
%token <token> LBRACKET RBRACKET LBRACE RBRACE LSQARE RSQARE ASSIGN COLON COMMA SEMI DOT
%token <string> IDENTIFIER NUM
%token <token> ConstDefArray ArrayInitList FuncDefVal FunDefArr FunctCallVal FunctCallArr//temp

%type <root> compUnit
%type <arrayident> ArrayIdent
%type <declare>VarDef DefVal DefArray ConstDef ConstDefVal //ConstDefArray 

%type <token> RelOP UnaryOp MulOp AddOp BType
%type <funcDef> FuncDef
%type <stmt> Stmt IfStmt WhileStmt BreakStmt ContinueStmt VoidStmt Assignment BlockItem ReturnStmt

%type <funcCallArgList> FunctionCallArgList 
%type <funcdefParamList> FuncParamList
%type <funcdefParam> FuncParam
%type <block> Block BlockItems
%type <declStmt>Decl VarDecl ConstDecl
%type <ident> Ident LVal
%type <expr> Number Exp InitVal LOrExp LAndExp EqExp AddExp MulExp PrimaryExp RelExp UnaryExp FunctCall ConstInitialVal Cond FuncCallArg
%start compUnit 
%%

compUnit: compUnit Decl {$$->codeBlock.push_back($<declare>2);}
    | compUnit FuncDef {$$->codeBlock.push_back($<funcDef>2);}
    | Decl{root = new front::ast::AST();$$=root;$$->codeBlock.push_back($<declare>1);}
    | FuncDef{root = new front::ast::AST();$$=root;$$->codeBlock.push_back($<funcDef>1);}
    ;

Decl: ConstDecl SEMI {$$ = $1;}
    | VarDecl SEMI {$$ = $1;}
    ;

ConstDecl: CONST BType ConstDef{ $$ = new front::ast::DeclareStatement();$$->declareList.push_back($3);}
    | ConstDecl COMMA ConstDef{$$->declareList.push_back($3);} 
    ;

BType: INT;

ConstDef: ConstDefVal
    | ConstDefArray
    ;

ConstDefVal: Ident ASSIGN ConstInitialVal{$$ = new front::ast::ConstDeclare($1,$3);}
    ;

ConstInitialVal:AddExp
    ;

constDefArray:
    ;

VarDecl: BType VarDef {$$ = new front::ast::DeclareStatement();$$->declareList.push_back($2);}
    | VarDecl COMMA VarDef{$$->declareList.push_back($3);} 
    ;

VarDef: DefVal //done
    | DefArray //
    ;

DefVal: Ident ASSIGN InitVal { $$ = new front::ast::VarDeclareWithInit($1,$3);}
    | Ident {$$ = new front::ast::VarDeclare($1);}

InitVal: Exp
    ;

DefArray: ArrayIdent ASSIGN ArrayInitList{$$ = new front::ast::ArrayDeclareWithInit();}
    | ArrayIdent
    ;
/*
ArrayIdent: ArrayIdent LSQARE Exp RSQARE{$$} 
    | Ident LSQARE Exp RSQARE {}
    ;

ArrayInitList:
    ;*/
//VarDef:IDENT ;

FuncDef: BType Ident LBRACKET FuncParamList RBRACKET Block {$$ = new front::ast::FunctionDefine($1,$2,$4,$6);}
    | BType Ident LBRACKET RBRACKET Block {$$ = new front::ast::FunctionDefine($1,$2,(NULL),$5);}
    | VOID Ident LBRACKET FuncParamList RBRACKET Block{ $$ = new front::ast::FunctionDefine($1,$2,$4,$6); }
    | VOID Ident LBRACKET RBRACKET Block{ $$ = new front::ast::FunctionDefine($1,$2,(NULL),$5);}
    ;

FuncParamList: FuncParamList COMMA FuncParam {$$->args.push_back($3);}
    | FuncParam {$$ = new front::ast::FunctionDefArgList();$$->args.push_back($1);}

FuncParam: FuncDefVal 
    | FunDefArr
    ;

Block: LBRACE RBRACE
    |   LBRACE BlockItems RBRACE
    ;

BlockItems:BlockItems BlockItem {$$->blockItem.push_back($1);}
    |BlockItem {$$ = new front::ast::Block();$$->blockItem.push_back($1);}

BlockItem: Decl
    | Stmt
    ;

Stmt: Assignment SEMI {$$ = $1;}
    | Block
    | IfStmt
    | WhileStmt
    | BreakStmt SEMI
    | ContinueStmt SEMI
    | ReturnStmt SEMI
    | VoidStmt SEMI
    ;

Assignment:LVal ASSIGN Exp {$$ = new front::ast::AssignExpression($1,$3);} 
    ;

IfStmt: IF LBRACKET Cond RBRACKET Stmt ELSE Stmt {$$= new front::ast::IfStatement($3,$5,$7);}
    | IF LBRACKET Cond RBRACKET Stmt {$$ = new front::ast::IfStatement($3,$5,NULL);}
    ;

WhileStmt: WHILE LBRACKET Cond RBRACKET Stmt {$$ = new front::ast::WhileStatement($3,$5);}
    ;

BreakStmt: BREAK SEMI {$$ =  new front::ast::BreakStatement();}
    ;

ContinueStmt: CONTINUE SEMI {$$ = new front::ast::ContinueStatement();}
    ;

VoidStmt: SEMI {$$ = new front::ast::VoidStatement();}
    ;

ReturnStmt: {$$ = new front::ast::ReturnStatement();}
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
    | Ident LBRACKET RBRACKET {$$ = new front::ast::FunctionCall($1,NULL);}
    ;

FunctionCallArgList: FunctionCallArgList COMMA FuncCallArg {$$->args.push_back($3);}
    | FuncCallArg {$$ = new front::ast::FunctionCallArgList(); $$->args.push_back($1);}
    ;

FuncCallArg:AddExp // ^&^
    ;

PrimaryExp: Number 
    | LVal
    | LBRACKET Exp RBRACKET {$$ = $2;}
    ;

LVal: Ident
    | ArrayIdent
    ;

ArrayIdent: ArrayIdent LSQARE Exp RSQARE {}
    |Ident LSQARE Exp RSQARE
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

Number: NUM {$$ = new front::ast::NumberExpression(atoi($1->c_str()));}
    ;

Ident: IDENTIFIER {$$ = new front::ast::Identifier(*$1);}
    ;