%{
#include<iostream>
#include<string>
#include "front/ast/AstNode.h"
#include "parser.hpp"


#define SAVE_TOKEN yylval.string=new std::string(yytext,yyleng)
#define TOKEN(t) (yylval.token=t)
#define FIX_LINE for(auto i:std::string(yytext_ptr)){if(i=='\n')yylineno++;}

void comment();
%}

%option noyywrap

%%

[0-9]+                      SAVE_TOKEN;return NUM;
[a-zA-Z_][a-zA-Z0-9_]*      SAVE_TOKEN;return IDENTIFIER;
\/\/.*\n                    FIX_LINE
"/*"                        comment();
"//"[^/n]*                  ;
\n                          yylineno++;
[ ]                         ;

"break"                     return TOKEN(BREAK);
"continue"                  return TOKEN(CONTINUE);
"else"                      return TOKEN(ELSE);
"if"                        return TOKEN(IF);
"return"                    return TOKEN(RETURN);
"void"                      return TOKEN(VOID);
"while"                     return TOKEN(WHILE);

"int"                       return TOKEN(INT);

"const"                     return TOKEN(CONST);

"+"                         return TOKEN(ADD);
"-"                         return TOKEN(SUB);
"*"                         return TOKEN(MUL);
"/"                         return TOKEN(DIV);
"%"                         return TOKEN(MOD);
"!"                         return TOKEN(NOT_EQUAL);

"<"                         return TOKEN(LT);
">"                         return TOKEN(GT);
"=="                        return TOKEN(EQ);
"!="                        return TOKEN(NE);
"<="                        return TOKEN(LE);
">="                        return TOKEN(GE);
"&&"                        return TOKEN(AND_OP);
"||"                        return TOKEN(OR_OP);

"{"                         return TOKEN(LBRACE);
"}"                         return TOKEN(RBRACE);
"("                         return TOKEN(LBRACKET);
")"                         return TOKEN(RBRACKET);
";"                         return TOKEN(SEMI);
","                         return TOKEN(COMMA);
":"                         return TOKEN(COLON);
"="                         return TOKEN(ASSIGN);
"["                         return TOKEN(LSQARE);
"]"                         return TOKEN(RSQARE);

[\t\v\n\f]                  ;

.                           /*unmatched codes*/

%%

void comment(){
    int c;
    while((c = yyinput()) != 0) {
        if(c == '\n')
            yylineno++;
        else if(c == '*') {
            if((c = yyinput()) == '/')
                break;
            else
                unput(c);
        }
    }
};
