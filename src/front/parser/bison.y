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
    front::ast::Identifier* Ident;
    front::ast::Expression* expr;
    front::ast::stmt* stmt;
    front::ast::FuctionCall* functCall;
    front::ast::AST* root;
    front::ast::Declare* declare; 
    front::ast::ConstDeclare *conDecl;
    front::ast::FunctionDefine* funcDecl;
    std::string *string;
}
%%
%token <token> CONST
%token <token> INT
%token <token> COMMA
%token <root> compUnit
%token <declare> Decl varDecl ConstDecl
%token <string> INDENTIFIER
%token <funcDef> FuncDef
%start compUnit ;
%%

CompUnit: compUnit Decl {$$->codeBlock.push_back($<declare>2);}
    |compUnit FuncDef {$$->codeBlock.push_back($<funcDef>2;)}
    |Decl{root = new front::ast::AST();$$=root;$$->codeBlock.push_back($<declare>1);}
    |FuncDef{root = new front::ast::AST();$$=root;$$->codeBlock.push_back($<funcDef>1);};

Decl: ConstDecl SEMI {$$ = $1;}
    | VarDecl SEMI {$$ = $1;}
    ;

ConstDecl: CONST BType ConstDef{ $$ = new front::ast::DeclareStatement();$$->declareList.push_back($1);}
    | ConstDecl COMMA ConstDef{$$->declareList.push_back($3);} 
    ;

BType: INT;

ConstDef: ConstDefVal
    | ConstDefArray
    ;

ConstDefVal: Ident ASSIGN ConstInitialVal{$$ = new front::ast::ConstDeclare($1,$3);}
    ;

constDefArray:
    ;

VarDecl: BType VarDef {$$ = new front::ast::DeclareStatement();$$->declareList.push_back($2);}
    |VarDecl COMMA VarDef{$$->declareList.push_back($3);} 
    ;

VarDef: DefVal //done
    |DefArray //
    ;

DefVal: ValIdent ASSIGN InitVal { $$ = new front::ast::VarDeclareWithInit($1,$3);}
    |ValIdent {$$ = new front::ast::VarDeclare(*($1));}

InitVal: Exp
    ;

DefArray: ArrayIdent ASSIGN ArrayInitList{}
    |ArrayIdent{}
    ;

ArrayIdent: ArrayIdent LSQARE Exp RSQARE{} 
    |Ident LSQARE Exp RSQARE {}
    ;

ArrayInitList:
    ;
//VarDef:IDENT ;

FuncDef: BType Ident LBRACKET FuncParamList RBRACKET Block {$$ = new front::ast::FunctionDefine($1,$2,$4,$6)}
    |BType Ident LBRACKET RBRACKET Block {$$ = new front::ast::FunctionDefine($1,$2,(NULL),$5);}
    |VOID Ident LBRACKET FuncParamList RBRACKET Block{ $$ = new front::ast::FunctionDefine($1,$2,$4,$6); }
    |VOID Ident LBRACKET RBRACKET Block{ $$ = new front::ast::FunctionDefine($1,$2,(NULL),$5);}
    ;

FuncParamList: FuncParamList COMMA FuncParam {$$->args.push_back($3);}
    |FuncParam {$$ = new front::ast::FuncDefArgList();$$->args.push_back($1);}

FuncParam: 
    ;

Exp: addExp;

addExp:addExp AddOp MulExp {}
    |MulExp {}
    ;

MulExp:MulExp UnaryExp{}
    |UnaryExp{}
    ;

UnaryExp:UnaryOp UnaryExp
    | FunctCall
    | PrimaryExp
    ;

Ident : IDENT {$$ = new front::ast::Identifier($1);}