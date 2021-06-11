%{
#include "y.tab.h"  
#include "AstNode.h"
#include <cstdio>
#include <cstdlib>  
%}

using parser::ast;

extern int yylex();
extern int yyget_lineno();
extern int yylex_destroy();
void yyerror(const char *s){
    printf("line %d : error: %s\n", yylex_lineno(),s);
    yylex_destroy();
    if (!yydebug)
        std::exit(1);
}
%%
%token '+','-','*','/'
%%