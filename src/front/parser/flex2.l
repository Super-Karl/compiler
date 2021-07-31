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

\/\/.*\n?                   FIX_LINE;
"/*"                        comment();
[ ]                         ;
\n                          yylineno++;
[\t\r]                      ;

"if"                        return TOKEN(IF);
"else"                      return TOKEN(ELSE);
"while"                     return TOKEN(WHILE);
"break"                     return TOKEN(BREAK);
"continue"                  return TOKEN(CONTINUE);
"return"                    return TOKEN(RETURN);

"const"                     return TOKEN(CONST);
"int"                       return TOKEN(INT);
"void"                      return TOKEN(VOID);

"starttime"[ \t\n]*"("      FIX_LINE; yylval.string = new std::string("_sysy_starttime"); *yy_cp = yy_hold_char; yy_hold_char='(';yy_cp--; yyleng--; yy_c_buf_p--; *yy_c_buf_p='_';yy_c_buf_p--; *yy_c_buf_p='_';yy_c_buf_p--; *yy_c_buf_p='E';yy_c_buf_p--; *yy_c_buf_p='N';yy_c_buf_p--; *yy_c_buf_p='I';yy_c_buf_p--; *yy_c_buf_p='L';yy_c_buf_p--; *yy_c_buf_p='_';yy_c_buf_p--; *yy_c_buf_p='_';yy_c_buf_p--; return IDENTIFIER;
"stoptime"[ \t\n]*"("       FIX_LINE; yylval.string = new std::string("_sysy_stoptime"); *yy_cp = yy_hold_char; yy_hold_char='(';yy_cp--; yyleng--; yy_c_buf_p--; *yy_c_buf_p='_';yy_c_buf_p--; *yy_c_buf_p='_';yy_c_buf_p--; *yy_c_buf_p='E';yy_c_buf_p--; *yy_c_buf_p='N';yy_c_buf_p--; *yy_c_buf_p='I';yy_c_buf_p--; *yy_c_buf_p='L';yy_c_buf_p--; *yy_c_buf_p='_';yy_c_buf_p--; *yy_c_buf_p='_';yy_c_buf_p--; return IDENTIFIER;

[a-zA-Z_][a-zA-Z0-9_]*      SAVE_TOKEN;return IDENTIFIER;
"0x"[0-9a-fA-F]+            SAVE_TOKEN;return NUM;
[0-9]+                      SAVE_TOKEN;return NUM;

"="                         return TOKEN(ASSIGN);
"=="                        return TOKEN(EQ);
"!="                        return TOKEN(NE);
"<"                         return TOKEN(LT);
"<="                        return TOKEN(LE);
">"                         return TOKEN(GT);
">="                        return TOKEN(GE);
"&&"                        return TOKEN(AND_OP);
"||"                        return TOKEN(OR_OP);

"("                         return TOKEN(LBRACKET);
")"                         return TOKEN(RBRACKET);
"["                         return TOKEN(LSQARE);
"]"                         return TOKEN(RSQARE);
"{"                         return TOKEN(LBRACE);
"}"                         return TOKEN(RBRACE);

":"                         return TOKEN(COLON);
","                         return TOKEN(COMMA);
";"                         return TOKEN(SEMI);

"+"                         return TOKEN(ADD);
"-"                         return TOKEN(SUB);
"*"                         return TOKEN(MUL);
"/"                         return TOKEN(DIV);
"%"                         return TOKEN(MOD);
"!"                         return TOKEN(NOT_EQUAL);

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
