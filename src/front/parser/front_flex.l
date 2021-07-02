%{
//#include<string>
//#include "grammar.hpp"

//using namespace std;
%}

%option noyywrap

number      [0-9]
Ident       [a-zA-Z_]{1}[a-zA-Z0-9_]
%%
"/*"        {comment();}
"//"[^/n]*
"break"         { count(); return(BREAK); }
"case"          { count(); return(CASE); }
"continue"      { count(); return(CONTINUE); }
"do"            { count(); return(DO); }
"else"          { count(); return(ELSE); }
"for"           { count(); return(FOR); }
"goto"          { count(); return(GOTO); }
"if"            { count(); return(IF); }
"return"        { count(); return(RETURN); }
"switch"        { count(); return(SWITCH); }
"void"          { count(); return(VOID); }
"while"         { count(); return(WHILE); }
"true"          { count(); return(TRUE); }
"false"         { count(); return(FALSE); }


"const"         { count(); return(CONST);}   

"+"             { count(); return('+');}
"-"             { count(); return('-');}
"*"             { count(); return('*');}
"/"             { count(); return('/');}
"%"             { count(); return('%');}
"!"             { count(); return('!');}

"<"             { count(); return('<');}
">"             { count(); return('>');}
"=="            { count(); return(EQ_OP);}
"!="            { count(); return(NE_OP);}
"<="            { count(); return(LE_OP);}
">="            { count(); return(GE_OP);}
"&&"            { count(); return(AND_OP);}
"||"            { count(); return(EQ_OP);}

"{"             { count(); return('{');}
"}"             { count(); return('}');}
"("             { count(); return('(');}
")"             { count(); return(')');}
"."             { count(); return('.');}
";"             { count(); return(';'); }
","             { count(); return(','); }
":"             { count(); return(':'); }
"="             { count(); return('='); }
"["             { count(); return('['); }
"]"             { count(); return(']'); }
[\t\v\n\f]      {count();}
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
void count(void)
{
    int i;

    for(int i=0;yytext[i]!='\0';i++)
        if(yytext[i]=='\n')
            column=0;
        else if(yytext[i] == '\t')
            column+=4-(column % 4);
        else
            column++;
        ECHO;
}