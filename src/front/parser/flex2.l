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

NUM         [0-9]
IDENT       [a-zA-Z_]{1}[a-zA-Z0-9_]
\/\/.*\n    FIX_LINE
"/*"        {comment();}
"//"[^/n]*  ;
[\n]          ;

"break"         {return BREAK;}
"case"          {return CASE ; }
"continue"      {return CONTINUE; }
"do"            {return(DO); }
"else"          {return(ELSE); }
"for"           {return(FOR); }
"goto"          {return(GOTO); }
"if"            {return(IF); }
"return"        {return(RETURN); }
"switch"        {return(SWITCH); }
"void"          {return(VOID); }
"while"         {return(WHILE); }
"true"          {return(TRUE); }
"false"         {return(FALSE); }


"const"         {return(CONST);}   

"+"             {return('ADD');}
"-"             {return('SUB');}
"*"             {return('MUL');}
"/"             {return('DIV');}
"%"             {return('SURPLUS');}
"!"             {return('NOT_EQUAL');}

"<"             {return('LESS');}
">"             {return('BIGGER');}
"=="            {return(EQ_OP);}
"!="            {return(NE_OP);}
"<="            {return(LE_OP);}
">="            {return(GE_OP);}
"&&"            {return(AND_OP);}
"||"            {return(EQ_OP);}

"{"             {return('LBRACE');}
"}"             {return('RBARCE');}
"("             {return('LBRACKET');}
")"             {return('RBRACKET');}
"."             {return('POINT');}
";"             {return('SELEICOLON'); }
","             {return('COMMA'); }
":"             {return('COLON'); }
"="             {return('EQUAL'); }
"["             {return('LSQARE'); }
"]"             {return('RSQARE'); }
[\t\v\n\f]      {}
.               {/*unmatched codes*/}
%%


void comment(){
    column=0;
    char c,prev=0;
    while(cin>>c){
        if(c=='/' && prev == '*')
            return;
        prev=c;
    }
    printf("unterminated comment");
}
