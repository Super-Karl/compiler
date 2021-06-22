%{
#include "y.tab.h"  
#include "AstNode.h"
#include <cstdio>
#include <cstdlib>  


//using parser::ast;
using namespace compiler;
extern int yylex();
extern int yyget_lineno();
extern int yylex_destroy();
void yyerror(const char *s){
    printf("line %d : error: %s\n", yylex_lineno(),s);
    yylex_destroy();
    if (!yydebug)
        std::exit(1);
%}

%union {
    int token;
    front::ast::Node::Identifier* identifier;
    front::ast::Node::Expression* expr;
    front::ast::Node::stmt* stmt;
    front::ast::Node::FuctionCall* functCall;
    front::ast::AST* root;
    front::ast::Node::Declare* declare; 
    front::ast::Node::ConstDeclare conDecl;
}
%%
%token <token> CONST
%token <root> CompU
%token <declare> Decl varDecl ConstDecl
%start compU;
%%

compU: compU Decl {$$->codeBlock.push_back($<declare>2);}
    |compU funcDef {$$->codeBlock.push_back($<>2;)}
    |Decl{root = new front::ast::AST();$$=root;$$->codeBlock.push_back($<declare>1);}
    |funcDef{};

Decl: ConstDecl
    | VarDecl
    ;

ConstDecl: CONST BType ConstDef {}
    | ConstDecl COMMA ConstDecl{}
    ;

BType: INT;

ConstDef: IDENT ASSIGN ConstInitialVal;

VarDecl: BType VarDef {}
    |VarDecl COMMA VarDef{}
    ;

//VarDef:IDENT ;

FuncDef : FuncType IDENT /*(*/FuncDef/*)*/
    ;