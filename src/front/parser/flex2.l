%{
#include<iostream>
#include<string>
using namespace std;
#define SAVE_TOKEN yylval.string=new string(yytext,yyleng)
#define TOKEN(t) (yylval=t)
#define FIX_LINE for(auto i:string(yytext_ptr)){if(i=='\n')yylineno++;}

void comment();
%}

%option noyywrap
%%

[0-9]+  SAVE_TOKEN;return NUM;
[a-zA-Z_][a-zA-Z0-9_]* SAVE_TOKEN;return IDENTIFIER; 
\/\/.*\n    FIX_LINE
"/*"        comment();
"//"[^/n]*  ;
[\n]          ;

"break"          return TOKEN(BREAK);  
"case"           return TOKEN(CASE) ;  
"continue"       return TOKEN(CONTINUE);  
"do"             return TOKEN(DO);  
"else"           return TOKEN(ELSE);  
"for"            return TOKEN(FOR);  
"goto"           return TOKEN(GOTO);  
"if"             return TOKEN(IF);  
"return"         return TOKEN(RETURN);  
"switch"         return TOKEN(SWITCH);  
"void"           return TOKEN(VOID);  
"while"          return TOKEN(WHILE);  
"true"           return TOKEN(TRUE);  
"false"          return TOKEN(FALSE);  


"const"          return TOKEN(CONST);     

"+"              return TOKEN(ADD);  
"-"              return TOKEN(SUB);  
"*"              return TOKEN(MUL);  
"/"              return TOKEN(DIV);  
"%"              return TOKEN(SURPLUS);  
"!"              return TOKEN(NOT_EQUAL);  

"<"              return TOKEN(LT);  
">"              return TOKEN(GT);  
"=="             return TOKEN(EQ);  
"!="             return TOKEN(NE);  
"<="             return TOKEN(LE);  
">="             return TOKEN(GE);   
"&&"             return TOKEN(AND_OP);  
"||"             return TOKEN(OR_OP);  

"{"              return TOKEN(LBRACE);  
"}"              return TOKEN(RBARCE);  
"("              return TOKEN(LBRACKET);  
")"              return TOKEN(RBRACKET);  
"."              return TOKEN(POINT);}
";"              return TOKEN(SEMI);  
","              return TOKEN(COMMA);  
":"              return TOKEN(COLON);  
"="              return TOKEN(ASSIGN);  
"["              return TOKEN(LSQARE);  
"]"              return TOKEN(RSQARE);  
[\t\v\n\f]       }
.                /*unmatched codes*/  
%%


void comment(){
    column=0;
    char c,prev=0;
    while(cin>>c){
        if(c=='/' && prev == '*')
            return TOKEN;
        prev=c;
    }
    printf("unterminated comment");
}
