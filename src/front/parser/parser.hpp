/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_MNT_C_USERS_KARL_CLIONPROJECTS_COMPILER_CMAKE_BUILD_DEBUG_FRONT_PARSER_HPP_INCLUDED
# define YY_YY_MNT_C_USERS_KARL_CLIONPROJECTS_COMPILER_CMAKE_BUILD_DEBUG_FRONT_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENTIFIER = 258,
    NUM = 259,
    IF = 260,
    ELSE = 261,
    BREAK = 262,
    CONTINUE = 263,
    RETURN = 264,
    WHILE = 265,
    CONST = 266,
    INT = 267,
    VOID = 268,
    ADD = 269,
    SUB = 270,
    MOD = 271,
    MUL = 272,
    DIV = 273,
    NOT_EQUAL = 274,
    LT = 275,
    GT = 276,
    LE = 277,
    GE = 278,
    AND_OP = 279,
    OR_OP = 280,
    EQ = 281,
    NE = 282,
    LBRACKET = 283,
    RBRACKET = 284,
    LBRACE = 285,
    RBRACE = 286,
    LSQARE = 287,
    RSQARE = 288,
    ASSIGN = 289,
    COLON = 290,
    COMMA = 291,
    SEMI = 292
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
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

#line 116 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_MNT_C_USERS_KARL_CLIONPROJECTS_COMPILER_CMAKE_BUILD_DEBUG_FRONT_PARSER_HPP_INCLUDED  */
