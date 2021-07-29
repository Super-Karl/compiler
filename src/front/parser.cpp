/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"

#include "front/ast/AstNode.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "controller/controller.h"
//using parser::ast;

using compiler::controller::generator::root;
using namespace compiler;
extern int yydebug;

extern int yylex();
extern int yyget_lineno();
extern int yylex_destroy();
#define YYDEBUG 1
#define YYERROR_VERBOSE true

void yyerror(const char *s){
    printf("line %d : error: %s\n", yyget_lineno(),s);
    yylex_destroy();
    if (!yydebug)
        std::exit(1);
}


#line 97 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
#line 28 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"

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

#line 208 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_MNT_C_USERS_KARL_CLIONPROJECTS_COMPILER_CMAKE_BUILD_DEBUG_FRONT_PARSER_HPP_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   241

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  54
/* YYNRULES -- Number of rules.  */
#define YYNRULES  112
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  182

#define YYUNDEFTOK  2
#define YYMAXUTOK   292


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    72,    72,    73,    74,    75,    78,    79,    82,    84,
      85,    88,    89,    92,    95,    98,   101,   102,   105,   106,
     109,   110,   112,   115,   116,   119,   120,   121,   122,   125,
     126,   129,   130,   131,   132,   135,   136,   137,   138,   141,
     142,   145,   146,   149,   152,   155,   156,   159,   160,   163,
     164,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     178,   181,   184,   185,   188,   191,   194,   197,   198,   201,
     202,   204,   207,   210,   211,   214,   215,   218,   219,   220,
     223,   224,   226,   227,   230,   231,   234,   235,   236,   239,
     240,   243,   244,   247,   250,   251,   252,   255,   256,   259,
     260,   263,   264,   265,   268,   269,   270,   273,   274,   275,
     276,   279,   282
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "NUM", "IF", "ELSE",
  "BREAK", "CONTINUE", "RETURN", "WHILE", "CONST", "INT", "VOID", "ADD",
  "SUB", "MOD", "MUL", "DIV", "NOT_EQUAL", "LT", "GT", "LE", "GE",
  "AND_OP", "OR_OP", "EQ", "NE", "LBRACKET", "RBRACKET", "LBRACE",
  "RBRACE", "LSQARE", "RSQARE", "ASSIGN", "COLON", "COMMA", "SEMI",
  "$accept", "compUnit", "Decl", "BType", "ConstDecl", "ConstDef",
  "ConstDefVal", "ConstInitialVal", "ConstDefArray", "VarDecl", "VarDef",
  "DefVal", "InitVal", "DefArray", "ArrayIdent", "ArrayInitList",
  "ListExp", "FuncDef", "FuncParamList", "FuncParam", "FuncDefVal",
  "FuncDefArr", "Block", "BlockItems", "BlockItem", "Stmt", "functStmt",
  "Assignment", "IfStmt", "WhileStmt", "BreakStmt", "ContinueStmt",
  "VoidStmt", "ReturnStmt", "Exp", "Cond", "LOrExp", "LAndExp", "EqExp",
  "RelExp", "AddExp", "MulExp", "UnaryExp", "FunctCall",
  "FunctionCallArgList", "FuncCallArg", "PrimaryExp", "LVal", "AddOp",
  "MulOp", "UnaryOp", "RelOP", "Number", "Ident", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292
};
# endif

#define YYPACT_NINF (-151)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-61)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     207,    34,  -151,    52,    75,  -151,    52,   -10,    64,  -151,
      52,  -151,    21,  -151,  -151,  -151,  -151,  -151,  -151,    46,
      42,    52,  -151,    52,  -151,  -151,  -151,  -151,    49,    73,
       7,    94,    30,    13,   154,   213,  -151,  -151,    85,    30,
     213,    37,    52,     3,  -151,  -151,  -151,  -151,  -151,  -151,
    -151,   213,  -151,    57,    83,   162,   217,  -151,  -151,  -151,
    -151,   213,  -151,   -15,   175,  -151,    37,     9,  -151,    87,
    -151,  -151,  -151,  -151,   162,    54,  -151,    57,   106,    37,
      34,   114,  -151,  -151,  -151,   213,  -151,  -151,  -151,   213,
    -151,   196,  -151,  -151,  -151,    -8,  -151,    37,  -151,   124,
     117,   130,   213,   144,  -151,  -151,  -151,    52,  -151,   125,
    -151,  -151,   137,   146,  -151,  -151,   148,   151,  -151,   155,
     158,   161,   157,  -151,  -151,  -151,   217,  -151,  -151,   162,
      18,  -151,  -151,   193,  -151,   213,  -151,  -151,  -151,   213,
    -151,  -151,  -151,  -151,  -151,  -151,  -151,  -151,   213,  -151,
     213,  -151,  -151,   172,   177,  -151,    66,  -151,   127,   180,
    -151,  -151,   156,   213,   213,   213,   213,  -151,  -151,  -151,
    -151,   213,   156,   198,  -151,  -151,  -151,  -151,  -151,  -151,
     156,  -151
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     8,     0,     0,     4,     0,     0,     0,     5,
       0,   112,     0,     1,     2,     3,    16,    18,    19,    24,
      21,     0,     6,     0,     7,    10,    12,    11,     0,     0,
       0,     0,     0,     0,     0,     0,     9,    17,    21,     0,
       0,     0,     0,     0,    40,    41,    42,   111,   104,   105,
     106,     0,    26,    98,     0,    71,    83,    85,    87,    88,
      95,     0,    94,    97,     0,    23,     0,     0,    28,     0,
      20,    22,    15,    13,    14,     0,    38,    44,    43,     0,
       0,     0,    25,    99,   100,     0,   103,   101,   102,     0,
      86,     0,    30,    34,    33,     0,    36,     0,    27,     0,
       0,     0,    70,     0,    45,    68,    49,     0,    52,     0,
      48,    50,     0,     0,    53,    54,     0,     0,    59,     0,
       0,    87,    95,    37,    39,    96,    82,    84,    90,    93,
       0,    92,    29,     0,    35,     0,    65,    66,    69,     0,
      46,    47,    58,    51,    55,    56,    57,    67,     0,    89,
       0,    32,    31,     0,    72,    74,    76,    79,    81,     0,
      61,    91,     0,     0,     0,     0,     0,   107,   108,   109,
     110,     0,     0,    63,    73,    75,    77,    78,    80,    64,
       0,    62
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -151,  -151,    48,     2,  -151,   192,  -151,  -151,  -151,  -151,
     191,  -151,   -54,  -151,    14,   -31,  -151,   218,   197,   149,
    -151,  -151,   -26,  -151,   122,  -150,  -151,  -151,  -151,  -151,
    -151,  -151,  -151,  -151,   -30,    97,  -151,    17,  -151,   -20,
     -40,   141,   -45,   -68,  -151,    88,  -151,   -66,  -151,  -151,
    -151,  -151,  -151,     8
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,   106,    42,     7,    25,    26,    73,    27,     8,
      16,    17,    70,    18,    53,    65,    95,     9,    43,    44,
      45,    46,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   153,   154,   155,   156,   157,
      55,    56,    57,    58,   130,   131,    59,    60,    85,    89,
      61,   171,    62,    63
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      74,    54,     6,    10,    69,    71,     6,   121,    72,   122,
      93,    12,   173,    91,    20,    76,    90,    34,    29,     2,
      19,    81,   179,   132,    28,     2,    21,    22,   133,    29,
     181,    38,    79,    94,    71,    28,    41,    19,    97,    80,
      96,   121,    66,   122,   127,    80,     2,   149,     5,    30,
      78,   129,    14,   123,   150,    11,    77,    11,    47,    99,
      64,   100,   101,   102,   103,     1,     2,    75,    48,    49,
      33,   134,   138,    50,    34,    13,    35,   107,    31,   151,
      32,    31,    51,    39,    75,   104,     1,     2,     3,    31,
     164,   105,   165,   166,   121,   158,   122,    11,    47,   158,
      23,    24,   152,    71,   121,    34,   122,    40,    48,    49,
     129,   107,   121,    50,   122,    38,    82,    34,   160,    35,
      98,    19,    51,   158,   158,   158,   158,    52,    11,    47,
      99,   158,   100,   101,   102,   103,     1,     2,    34,    48,
      49,    83,    84,   125,    50,   176,   177,   167,   168,   169,
     170,   178,   135,    51,   136,    75,   140,    11,    47,    11,
      47,    99,   105,   100,   101,   102,   103,   137,    48,    49,
      48,    49,   139,    50,   142,    50,    83,    84,    11,    47,
     174,   175,    51,   143,    51,   144,    75,    68,   145,    48,
      49,   148,   146,   105,    50,   147,    11,    47,   -60,    11,
      47,   162,   163,    51,   180,    64,    92,    48,    49,   172,
      48,    49,    50,    36,    37,    50,    11,    47,     1,     2,
       3,    51,    15,    64,    51,   128,   126,    48,    49,   124,
      67,   141,    50,    86,    87,    88,   159,     0,   161,     0,
       0,    51
};

static const yytype_int16 yycheck[] =
{
      40,    31,     0,     1,    34,    35,     4,    75,    39,    75,
      64,     3,   162,    28,     6,    41,    61,    32,    10,    12,
       6,    51,   172,    31,    10,    12,    36,    37,    36,    21,
     180,    23,    29,    64,    64,    21,    29,    23,    29,    36,
      66,   109,    29,   109,    89,    36,    12,    29,     0,    28,
      42,    91,     4,    79,    36,     3,    42,     3,     4,     5,
      30,     7,     8,     9,    10,    11,    12,    30,    14,    15,
      28,    97,   102,    19,    32,     0,    34,    75,    32,   133,
      34,    32,    28,    34,    30,    31,    11,    12,    13,    32,
      24,    37,    26,    27,   162,   135,   162,     3,     4,   139,
      36,    37,   133,   133,   172,    32,   172,    34,    14,    15,
     150,   109,   180,    19,   180,   107,    33,    32,   148,    34,
      33,   107,    28,   163,   164,   165,   166,    33,     3,     4,
       5,   171,     7,     8,     9,    10,    11,    12,    32,    14,
      15,    14,    15,    29,    19,   165,   166,    20,    21,    22,
      23,   171,    28,    28,    37,    30,    31,     3,     4,     3,
       4,     5,    37,     7,     8,     9,    10,    37,    14,    15,
      14,    15,    28,    19,    37,    19,    14,    15,     3,     4,
     163,   164,    28,    37,    28,    37,    30,    33,    37,    14,
      15,    34,    37,    37,    19,    37,     3,     4,    37,     3,
       4,    29,    25,    28,     6,    30,    31,    14,    15,    29,
      14,    15,    19,    21,    23,    19,     3,     4,    11,    12,
      13,    28,     4,    30,    28,    29,    85,    14,    15,    80,
      33,   109,    19,    16,    17,    18,   139,    -1,   150,    -1,
      -1,    28
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    11,    12,    13,    39,    40,    41,    42,    47,    55,
      41,     3,    91,     0,    40,    55,    48,    49,    51,    52,
      91,    36,    37,    36,    37,    43,    44,    46,    52,    91,
      28,    32,    34,    28,    32,    34,    43,    48,    91,    34,
      34,    29,    41,    56,    57,    58,    59,     4,    14,    15,
      19,    28,    33,    52,    72,    78,    79,    80,    81,    84,
      85,    88,    90,    91,    30,    53,    29,    56,    33,    72,
      50,    72,    53,    45,    78,    30,    60,    52,    91,    29,
      36,    72,    33,    14,    15,    86,    16,    17,    18,    87,
      80,    28,    31,    50,    53,    54,    60,    29,    33,     5,
       7,     8,     9,    10,    31,    37,    40,    41,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    81,    85,    60,    57,    29,    79,    80,    29,    78,
      82,    83,    31,    36,    60,    28,    37,    37,    72,    28,
      31,    62,    37,    37,    37,    37,    37,    37,    34,    29,
      36,    50,    53,    73,    74,    75,    76,    77,    78,    73,
      72,    83,    29,    25,    24,    26,    27,    20,    21,    22,
      23,    89,    29,    63,    75,    75,    77,    77,    77,    63,
       6,    63
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    38,    39,    39,    39,    39,    40,    40,    41,    42,
      42,    43,    43,    44,    45,    46,    47,    47,    48,    48,
      49,    49,    50,    51,    51,    52,    52,    52,    52,    53,
      53,    54,    54,    54,    54,    55,    55,    55,    55,    56,
      56,    57,    57,    58,    59,    60,    60,    61,    61,    62,
      62,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      64,    65,    66,    66,    67,    68,    69,    70,    70,    71,
      71,    72,    73,    74,    74,    75,    75,    76,    76,    76,
      77,    77,    78,    78,    79,    79,    80,    80,    80,    81,
      81,    82,    82,    83,    84,    84,    84,    85,    85,    86,
      86,    87,    87,    87,    88,    88,    88,    89,    89,    89,
      89,    90,    91
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     2,     1,     1,     2,     2,     1,     3,
       3,     1,     1,     3,     1,     3,     2,     3,     1,     1,
       3,     1,     1,     3,     1,     4,     3,     4,     3,     3,
       2,     3,     3,     1,     1,     6,     5,     6,     5,     3,
       1,     1,     1,     2,     2,     2,     3,     2,     1,     1,
       1,     2,     1,     1,     1,     2,     2,     2,     2,     1,
       1,     3,     7,     5,     5,     2,     2,     2,     1,     2,
       1,     1,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     1,     3,     1,     3,     1,     2,     1,     1,     4,
       3,     3,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 72 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                        {(yyval.root)->codeBlock.push_back((yyvsp[0].declare)); }
#line 1535 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 3:
#line 73 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                       {(yyval.root)->codeBlock.push_back((yyvsp[0].funcDef));}
#line 1541 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 4:
#line 74 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
          {root = new front::ast::AST();(yyval.root)=root;(yyval.root)->codeBlock.push_back((yyvsp[0].declare));}
#line 1547 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 5:
#line 75 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
             {root = new front::ast::AST();(yyval.root)=root;(yyval.root)->codeBlock.push_back((yyvsp[0].funcDef));}
#line 1553 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 6:
#line 78 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                     {(yyval.declStmt) = (yyvsp[-1].declStmt);}
#line 1559 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 7:
#line 79 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                   {(yyval.declStmt) = (yyvsp[-1].declStmt);}
#line 1565 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 9:
#line 84 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                  {(yyval.declStmt)->declareList.push_back((yyvsp[0].declare));}
#line 1571 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 10:
#line 85 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                          { (yyval.declStmt) = new front::ast::DeclareStatement();(yyval.declStmt)->declareList.push_back((yyvsp[0].declare));}
#line 1577 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 13:
#line 92 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                         {(yyval.declare) = new front::ast::ConstDeclare((yyvsp[-2].ident),(yyvsp[0].expr));}
#line 1583 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 15:
#line 98 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                              {(yyval.declare) = new front::ast::ConstArray((yyvsp[-2].arrayident),(yyvsp[0].arrayInitList));}
#line 1589 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 16:
#line 101 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                      {(yyval.declStmt) = new front::ast::DeclareStatement();(yyval.declStmt)->declareList.push_back((yyvsp[0].declare));}
#line 1595 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 17:
#line 102 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                          {(yyval.declStmt)->declareList.push_back((yyvsp[0].declare));}
#line 1601 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 20:
#line 109 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                             { (yyval.declare) = new front::ast::VarDeclareWithInit((yyvsp[-2].ident),(yyvsp[0].expr));}
#line 1607 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 21:
#line 110 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
            {(yyval.declare) = new front::ast::VarDeclare((yyvsp[0].ident));}
#line 1613 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 23:
#line 115 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                         {(yyval.declare) = new front::ast::ArrayDeclareWithInit((yyvsp[-2].arrayident),(yyvsp[0].arrayInitList));}
#line 1619 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 24:
#line 116 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                 {(yyval.declare) = new front::ast::ArrayDeclare((yyvsp[0].arrayident));}
#line 1625 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 25:
#line 119 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                         {(yyval.arrayident)->index.push_back((yyvsp[-1].expr));}
#line 1631 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 26:
#line 120 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                              {(yyval.arrayident)->index.push_back(new front::ast::NumberExpression());}
#line 1637 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 27:
#line 121 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                              {(yyval.arrayident) = new front::ast::ArrayIdentifier((yyvsp[-3].ident)->name);(yyval.arrayident)->index.push_back((yyvsp[-1].expr));}
#line 1643 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 28:
#line 122 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                          {(yyval.arrayident) = new front::ast::ArrayIdentifier((yyvsp[-2].ident)->name);(yyval.arrayident)->index.push_back(new front::ast::NumberExpression());}
#line 1649 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 29:
#line 125 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                    {(yyval.arrayInitList) = (yyvsp[-1].arrayInitList);}
#line 1655 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 30:
#line 126 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                   {(yyval.arrayInitList) = new front::ast::ArrayInitVal();}
#line 1661 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 31:
#line 129 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                     {(yyval.arrayInitList)->initValList.push_back((yyvsp[0].arrayInitList));}
#line 1667 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 32:
#line 130 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                           {(yyval.arrayInitList)->initValList.push_back((yyvsp[0].expr));}
#line 1673 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 33:
#line 131 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                    {(yyval.arrayInitList) = new front::ast::ArrayInitVal();(yyval.arrayInitList)->initValList.push_back((yyvsp[0].arrayInitList));}
#line 1679 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 34:
#line 132 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
              {(yyval.arrayInitList) = new front::ast::ArrayInitVal();(yyval.arrayInitList)->initValList.push_back((yyvsp[0].expr));}
#line 1685 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 35:
#line 135 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                                           {(yyval.funcDef) = new front::ast::FunctionDefine((yyvsp[-5].token),(yyvsp[-4].ident),(yyvsp[-2].funcdefParamList),(yyvsp[0].block));}
#line 1691 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 36:
#line 136 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                          {(yyval.funcDef) = new front::ast::FunctionDefine((yyvsp[-4].token),(yyvsp[-3].ident),(new front::ast::FunctionDefArgList()),(yyvsp[0].block));}
#line 1697 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 37:
#line 137 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                                      { (yyval.funcDef) = new front::ast::FunctionDefine((yyvsp[-5].token),(yyvsp[-4].ident),(yyvsp[-2].funcdefParamList),(yyvsp[0].block)); }
#line 1703 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 38:
#line 138 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                        { (yyval.funcDef) = new front::ast::FunctionDefine((yyvsp[-4].token),(yyvsp[-3].ident),(new front::ast::FunctionDefArgList()),(yyvsp[0].block));}
#line 1709 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 39:
#line 141 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                             {(yyval.funcdefParamList)->args.push_back((yyvsp[0].funcdefParam));}
#line 1715 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 40:
#line 142 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                {(yyval.funcdefParamList) = new front::ast::FunctionDefArgList();(yyval.funcdefParamList)->args.push_back((yyvsp[0].funcdefParam));}
#line 1721 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 43:
#line 149 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                        {(yyval.funcdefParam) = new front::ast::FunctionDefArg((yyvsp[-1].token),(yyvsp[0].ident));}
#line 1727 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 44:
#line 152 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                             {(yyval.funcdefParam) = new front::ast::FunctionDefArg((yyvsp[-1].token),(yyvsp[0].arrayident));}
#line 1733 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 45:
#line 155 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                    { (yyval.block) = new front::ast::Block();}
#line 1739 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 46:
#line 156 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                {(yyval.block) = (yyvsp[-1].block);}
#line 1745 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 47:
#line 159 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                {(yyval.block) = (yyvsp[-1].block);(yyval.block)->blockItem.push_back((yyvsp[0].stmt));}
#line 1751 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 48:
#line 160 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
               {(yyval.block) = new front::ast::Block();(yyval.block)->blockItem.push_back((yyvsp[0].stmt));}
#line 1757 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 51:
#line 167 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                      {(yyval.stmt) = (yyvsp[-1].stmt);}
#line 1763 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 55:
#line 171 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                     { (yyval.stmt) = (yyvsp[-1].stmt);}
#line 1769 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 56:
#line 172 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                        { (yyval.stmt) = (yyvsp[-1].stmt);}
#line 1775 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 57:
#line 173 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                      {(yyval.stmt) = (yyvsp[-1].stmt);}
#line 1781 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 58:
#line 174 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                     {(yyval.stmt) = (yyvsp[-1].stmt);}
#line 1787 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 61:
#line 181 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                           {(yyval.stmt) = new front::ast::AssignStmt((yyvsp[-2].ident),(yyvsp[0].expr));}
#line 1793 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 62:
#line 184 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                                 {(yyval.stmt)= new front::ast::IfStatement((yyvsp[-4].expr),(yyvsp[-2].stmt),(yyvsp[0].stmt));}
#line 1799 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 63:
#line 185 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                     {(yyval.stmt) = new front::ast::IfStatement((yyvsp[-2].expr),(yyvsp[0].stmt),(new front::ast::VoidStatement()));}
#line 1805 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 64:
#line 188 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                             {(yyval.stmt) = new front::ast::WhileStatement((yyvsp[-2].expr),(yyvsp[0].stmt));}
#line 1811 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 65:
#line 191 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                      {(yyval.stmt) =  new front::ast::BreakStatement();}
#line 1817 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 66:
#line 194 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                            {(yyval.stmt) = new front::ast::ContinueStatement();}
#line 1823 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 67:
#line 197 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                  {(yyval.stmt) = new front::ast::VoidStatement();}
#line 1829 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 68:
#line 198 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
          {(yyval.stmt) = new front::ast::VoidStatement();}
#line 1835 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 69:
#line 201 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                      {(yyval.stmt) = new front::ast::ReturnStatement((yyvsp[0].expr));}
#line 1841 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 70:
#line 202 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
            {(yyval.stmt) = new front::ast::ReturnStatement();}
#line 1847 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 73:
#line 210 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                            {(yyval.expr) = new front::ast::BinaryExpression((yyvsp[-2].expr),(yyvsp[-1].token),(yyvsp[0].expr));}
#line 1853 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 75:
#line 214 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                             {(yyval.expr) = new front::ast::BinaryExpression((yyvsp[-2].expr),(yyvsp[-1].token),(yyvsp[0].expr));}
#line 1859 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 77:
#line 218 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                      {(yyval.expr) = new front::ast::BinaryExpression((yyvsp[-2].expr),(yyvsp[-1].token),(yyvsp[0].expr));}
#line 1865 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 78:
#line 219 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                     {(yyval.expr) = new front::ast::BinaryExpression((yyvsp[-2].expr),(yyvsp[-1].token),(yyvsp[0].expr));}
#line 1871 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 80:
#line 223 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                           {(yyval.expr) = new front::ast::BinaryExpression((yyvsp[-2].expr),(yyvsp[-1].token),(yyvsp[0].expr));}
#line 1877 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 82:
#line 226 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                            {(yyval.expr) = new front::ast::BinaryExpression((yyvsp[-2].expr),(yyvsp[-1].token),(yyvsp[0].expr));}
#line 1883 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 84:
#line 230 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                             {(yyval.expr) = new front::ast::BinaryExpression((yyvsp[-2].expr),(yyvsp[-1].token),(yyvsp[0].expr));}
#line 1889 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 86:
#line 234 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                           {(yyval.expr) = new front::ast::UnaryExpression((yyvsp[-1].token),(yyvsp[0].expr));}
#line 1895 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 89:
#line 239 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                                       {(yyval.expr) = new front::ast::FunctionCall((yyvsp[-3].ident),(yyvsp[-1].funcCallArgList));}
#line 1901 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 90:
#line 240 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                              {(yyval.expr) = new front::ast::FunctionCall((yyvsp[-2].ident),(new front::ast::FunctionCallArgList()));}
#line 1907 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 91:
#line 243 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                                                           {(yyval.funcCallArgList)->args.push_back((yyvsp[0].expr));}
#line 1913 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 92:
#line 244 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                  {(yyval.funcCallArgList) = new front::ast::FunctionCallArgList(); (yyval.funcCallArgList)->args.push_back((yyvsp[0].expr));}
#line 1919 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 96:
#line 252 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                            {(yyval.expr) = (yyvsp[-1].expr);}
#line 1925 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 111:
#line 279 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
            {(yyval.expr) = new front::ast::NumberExpression(std::stoi(*(yyvsp[0].string),0,0));}
#line 1931 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;

  case 112:
#line 282 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
                  {(yyval.ident) = new front::ast::Identifier(*(yyvsp[0].string));}
#line 1937 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"
    break;


#line 1941 "/mnt/c/Users/karl/CLionProjects/compiler/cmake-build-debug/front/parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 284 "/mnt/c/Users/karl/CLionProjects/compiler/src/front/parser/bison.y"
