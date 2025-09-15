/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 30 "src/parser/parser.y"

#include <cstdio>
#include <memory>

extern "C" {
    int yylex();
    void yyerror(const char *s);
    extern FILE *yyin;
    extern int yylineno;
}

namespace ambar {
    class ASTNode;
    extern std::unique_ptr<ASTNode> astRoot;
}

using namespace ambar;


#line 91 "src/parser/parser.tab.cc"

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

#include "parser.tab.hh"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_NUM_INT = 4,                    /* NUM_INT  */
  YYSYMBOL_NUM_REAL = 5,                   /* NUM_REAL  */
  YYSYMBOL_STRING = 6,                     /* STRING  */
  YYSYMBOL_BOOL_TRUE = 7,                  /* BOOL_TRUE  */
  YYSYMBOL_BOOL_FALSE = 8,                 /* BOOL_FALSE  */
  YYSYMBOL_IMPORT = 9,                     /* IMPORT  */
  YYSYMBOL_FUNC = 10,                      /* FUNC  */
  YYSYMBOL_RETURN = 11,                    /* RETURN  */
  YYSYMBOL_IF = 12,                        /* IF  */
  YYSYMBOL_ELSE = 13,                      /* ELSE  */
  YYSYMBOL_WHILE = 14,                     /* WHILE  */
  YYSYMBOL_FOR = 15,                       /* FOR  */
  YYSYMBOL_BREAK = 16,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 17,                  /* CONTINUE  */
  YYSYMBOL_INT = 18,                       /* INT  */
  YYSYMBOL_FLOAT = 19,                     /* FLOAT  */
  YYSYMBOL_BOOL = 20,                      /* BOOL  */
  YYSYMBOL_STRING_T = 21,                  /* STRING_T  */
  YYSYMBOL_VOID = 22,                      /* VOID  */
  YYSYMBOL_AND = 23,                       /* AND  */
  YYSYMBOL_OR = 24,                        /* OR  */
  YYSYMBOL_NOT = 25,                       /* NOT  */
  YYSYMBOL_EQ = 26,                        /* EQ  */
  YYSYMBOL_NEQ = 27,                       /* NEQ  */
  YYSYMBOL_LT = 28,                        /* LT  */
  YYSYMBOL_LE = 29,                        /* LE  */
  YYSYMBOL_GT = 30,                        /* GT  */
  YYSYMBOL_GE = 31,                        /* GE  */
  YYSYMBOL_ADD = 32,                       /* ADD  */
  YYSYMBOL_SUB = 33,                       /* SUB  */
  YYSYMBOL_MUL = 34,                       /* MUL  */
  YYSYMBOL_DIV = 35,                       /* DIV  */
  YYSYMBOL_MOD = 36,                       /* MOD  */
  YYSYMBOL_ASSIGN = 37,                    /* ASSIGN  */
  YYSYMBOL_ARROW = 38,                     /* ARROW  */
  YYSYMBOL_SEMI = 39,                      /* SEMI  */
  YYSYMBOL_COLON = 40,                     /* COLON  */
  YYSYMBOL_COMMA = 41,                     /* COMMA  */
  YYSYMBOL_DOT = 42,                       /* DOT  */
  YYSYMBOL_LPAREN = 43,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 44,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 45,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 46,                    /* RBRACE  */
  YYSYMBOL_YYACCEPT = 47,                  /* $accept  */
  YYSYMBOL_program = 48,                   /* program  */
  YYSYMBOL_decl_list = 49,                 /* decl_list  */
  YYSYMBOL_decl = 50,                      /* decl  */
  YYSYMBOL_import_decl = 51,               /* import_decl  */
  YYSYMBOL_var_decl = 52,                  /* var_decl  */
  YYSYMBOL_func_decl = 53,                 /* func_decl  */
  YYSYMBOL_opt_params = 54,                /* opt_params  */
  YYSYMBOL_params = 55,                    /* params  */
  YYSYMBOL_type = 56,                      /* type  */
  YYSYMBOL_stmt = 57,                      /* stmt  */
  YYSYMBOL_assign_stmt = 58,               /* assign_stmt  */
  YYSYMBOL_call_stmt = 59,                 /* call_stmt  */
  YYSYMBOL_return_stmt = 60,               /* return_stmt  */
  YYSYMBOL_if_stmt = 61,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 62,                /* while_stmt  */
  YYSYMBOL_for_stmt = 63,                  /* for_stmt  */
  YYSYMBOL_break_stmt = 64,                /* break_stmt  */
  YYSYMBOL_continue_stmt = 65,             /* continue_stmt  */
  YYSYMBOL_block = 66,                     /* block  */
  YYSYMBOL_stmt_list = 67,                 /* stmt_list  */
  YYSYMBOL_opt_args = 68,                  /* opt_args  */
  YYSYMBOL_args = 69,                      /* args  */
  YYSYMBOL_expr = 70,                      /* expr  */
  YYSYMBOL_logic_expr = 71,                /* logic_expr  */
  YYSYMBOL_rel_expr = 72,                  /* rel_expr  */
  YYSYMBOL_arith_expr = 73,                /* arith_expr  */
  YYSYMBOL_term = 74,                      /* term  */
  YYSYMBOL_factor = 75,                    /* factor  */
  YYSYMBOL_func_call = 76                  /* func_call  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYFINAL  57
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   152

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  74
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  139

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   301


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    88,    88,   102,   109,   119,   120,   121,   122,   126,
     130,   136,   145,   161,   162,   166,   172,   181,   182,   183,
     184,   185,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   201,   209,   213,   217,   224,   231,   242,   252,   264,
     271,   278,   282,   297,   301,   308,   309,   313,   317,   324,
     328,   329,   337,   348,   349,   357,   365,   373,   381,   389,
     400,   401,   409,   420,   421,   429,   440,   444,   448,   453,
     458,   462,   466,   467,   471
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER",
  "NUM_INT", "NUM_REAL", "STRING", "BOOL_TRUE", "BOOL_FALSE", "IMPORT",
  "FUNC", "RETURN", "IF", "ELSE", "WHILE", "FOR", "BREAK", "CONTINUE",
  "INT", "FLOAT", "BOOL", "STRING_T", "VOID", "AND", "OR", "NOT", "EQ",
  "NEQ", "LT", "LE", "GT", "GE", "ADD", "SUB", "MUL", "DIV", "MOD",
  "ASSIGN", "ARROW", "SEMI", "COLON", "COMMA", "DOT", "LPAREN", "RPAREN",
  "LBRACE", "RBRACE", "$accept", "program", "decl_list", "decl",
  "import_decl", "var_decl", "func_decl", "opt_params", "params", "type",
  "stmt", "assign_stmt", "call_stmt", "return_stmt", "if_stmt",
  "while_stmt", "for_stmt", "break_stmt", "continue_stmt", "block",
  "stmt_list", "opt_args", "args", "expr", "logic_expr", "rel_expr",
  "arith_expr", "term", "factor", "func_call", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-96)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      25,     6,     8,    11,    79,   -34,    16,    26,     9,    33,
       5,    73,    25,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,   -96,   -96,   -96,   -96,   -96,   -96,    35,    85,   124,
      85,    65,    59,    61,   -96,   -96,   -96,   -96,   -96,   -96,
      85,    69,    21,   -96,    68,    18,   -96,   -96,    85,    85,
     104,   -96,   -96,   -30,   -96,   -96,    15,   -96,   -96,   -96,
      71,   -96,   -96,   -96,   -96,   -96,   -33,    67,    72,   -96,
     109,   113,    75,   -96,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    76,    80,    86,    85,
     -96,   -96,   -96,    85,   -96,   -96,    85,    78,    87,    81,
      88,   -96,   -96,   -96,    22,    22,    22,    22,    22,    22,
      18,    18,   -96,   -96,    64,    64,    92,    93,   -96,   -96,
     124,    95,   131,   134,   -96,   104,   -96,   -96,   124,   108,
      64,   105,   106,   124,   -96,    64,   -96,   -96,   -96
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     2,     3,     5,     6,     7,     8,    22,    23,
      24,    25,    26,    27,    28,    29,    30,     0,     0,     0,
      45,     0,     0,    68,    66,    67,    69,    70,    71,    34,
       0,     0,    49,    50,    53,    60,    63,    72,     0,     0,
       0,    39,    40,     0,    41,    43,     0,     1,     4,    32,
       0,    17,    18,    19,    20,    21,     0,     0,    46,    47,
       0,    13,     0,    33,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      42,    44,    31,     0,    11,    74,     0,     0,     0,     0,
      14,    73,    51,    52,    54,    55,    56,    57,    58,    59,
      61,    62,    64,    65,     0,     0,     0,     0,    48,     9,
       0,     0,     0,    35,    37,     0,    10,    15,     0,     0,
       0,     0,     0,     0,    36,     0,    12,    16,    38
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -96,   -96,   -96,   138,   -96,   -96,   -96,   -96,   -96,   -95,
      -9,   -48,   -96,   -96,   -96,   -96,   -96,   -96,   -96,    20,
     -96,   -96,   -96,   -25,   -96,   -17,    60,   -20,   -19,     0
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    11,    12,    13,    14,    15,    16,    99,   100,    66,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      56,    67,    68,    41,    42,    43,    44,    45,    46,    47
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      27,    55,    89,    60,    93,    69,    94,    28,    53,    48,
      27,    31,    27,    30,    32,    72,     4,     5,    53,     6,
       7,     8,     9,    86,    87,   127,     4,     5,     1,     6,
       7,     8,     9,   132,     2,     3,     4,     5,   137,     6,
       7,     8,     9,    28,    74,    75,    29,    91,    51,    30,
      10,    54,    84,    85,    82,    83,    27,   102,   103,    49,
      10,    90,   110,   111,   116,   112,   113,    53,   117,    50,
      10,   118,    52,    57,    59,     4,     5,   131,     6,     7,
       8,     9,    33,    34,    35,    36,    37,    38,    33,    34,
      35,    36,    37,    38,    76,    77,    78,    79,    80,    81,
      82,    83,    71,    70,    30,   123,   124,    88,    73,    10,
      92,    95,    97,    96,    27,    27,    98,   119,    39,   101,
     114,   134,    40,    28,   115,   121,   138,   120,    40,   122,
      27,   125,   126,   128,   129,    27,   104,   105,   106,   107,
     108,   109,    61,    62,    63,    64,    65,   130,   133,   135,
      58,    10,   136
};

static const yytype_uint8 yycheck[] =
{
       0,    10,    50,    28,    37,    30,    39,    37,     3,    43,
      10,     3,    12,    43,     3,    40,    11,    12,     3,    14,
      15,    16,    17,    48,    49,   120,    11,    12,     3,    14,
      15,    16,    17,   128,     9,    10,    11,    12,   133,    14,
      15,    16,    17,    37,    23,    24,    40,    56,    39,    43,
      45,    46,    34,    35,    32,    33,    56,    74,    75,    43,
      45,    46,    82,    83,    89,    84,    85,     3,    93,    43,
      45,    96,    39,     0,    39,    11,    12,   125,    14,    15,
      16,    17,     3,     4,     5,     6,     7,     8,     3,     4,
       5,     6,     7,     8,    26,    27,    28,    29,    30,    31,
      32,    33,    43,    38,    43,   114,   115,     3,    39,    45,
      39,    44,     3,    41,   114,   115,     3,    39,    39,    44,
      44,   130,    43,    37,    44,    44,   135,    40,    43,    41,
     130,    39,    39,    38,     3,   135,    76,    77,    78,    79,
      80,    81,    18,    19,    20,    21,    22,    13,    40,    44,
      12,    45,   132
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     9,    10,    11,    12,    14,    15,    16,    17,
      45,    48,    49,    50,    51,    52,    53,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    76,    37,    40,
      43,     3,     3,     3,     4,     5,     6,     7,     8,    39,
      43,    70,    71,    72,    73,    74,    75,    76,    43,    43,
      43,    39,    39,     3,    46,    57,    67,     0,    50,    39,
      70,    18,    19,    20,    21,    22,    56,    68,    69,    70,
      38,    43,    70,    39,    23,    24,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    70,    70,     3,    58,
      46,    57,    39,    37,    39,    44,    41,     3,     3,    54,
      55,    44,    72,    72,    73,    73,    73,    73,    73,    73,
      74,    74,    75,    75,    44,    44,    70,    70,    70,    39,
      40,    44,    41,    57,    57,    39,    39,    56,    38,     3,
      13,    58,    56,    40,    57,    44,    66,    56,    57
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    47,    48,    49,    49,    50,    50,    50,    50,    51,
      52,    52,    53,    54,    54,    55,    55,    56,    56,    56,
      56,    56,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    58,    59,    60,    60,    61,    61,    62,    63,    64,
      65,    66,    66,    67,    67,    68,    68,    69,    69,    70,
      71,    71,    71,    72,    72,    72,    72,    72,    72,    72,
      73,    73,    73,    74,    74,    74,    75,    75,    75,    75,
      75,    75,    75,    75,    76
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     1,     5,
       6,     4,     8,     0,     1,     3,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     2,     3,     2,     5,     7,     5,     8,     2,
       2,     2,     3,     1,     2,     0,     1,     1,     3,     1,
       1,     3,     3,     1,     3,     3,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     3,     4
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

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
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  case 2: /* program: decl_list  */
#line 88 "src/parser/parser.y"
              {
        auto prog = std::make_unique<ambar::ProgramNode>();
        ambar::ProgramNode* existingProg = dynamic_cast<ambar::ProgramNode*>((yyvsp[0].node));
        if (existingProg) {
            astRoot = std::unique_ptr<ambar::ASTNode>((yyvsp[0].node));
        } else {
            prog->addDeclaration(std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
            astRoot = std::move(prog);
        }
        (yyval.node) = astRoot.get();
    }
#line 1259 "src/parser/parser.tab.cc"
    break;

  case 3: /* decl_list: decl  */
#line 102 "src/parser/parser.y"
           {
          auto p = std::make_unique<ambar::ProgramNode>();
          if ((yyvsp[0].node)) {
              p->addDeclaration(std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
          }
          (yyval.node) = p.release();
      }
#line 1271 "src/parser/parser.tab.cc"
    break;

  case 4: /* decl_list: decl_list decl  */
#line 109 "src/parser/parser.y"
                     {
          ambar::ProgramNode* p = dynamic_cast<ambar::ProgramNode*>((yyvsp[-1].node));
          if (p && (yyvsp[0].node)) {
              p->addDeclaration(std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
          }
          (yyval.node) = p;
      }
#line 1283 "src/parser/parser.tab.cc"
    break;

  case 5: /* decl: import_decl  */
#line 119 "src/parser/parser.y"
                          { (yyval.node) = nullptr; }
#line 1289 "src/parser/parser.tab.cc"
    break;

  case 6: /* decl: var_decl  */
#line 120 "src/parser/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1295 "src/parser/parser.tab.cc"
    break;

  case 7: /* decl: func_decl  */
#line 121 "src/parser/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1301 "src/parser/parser.tab.cc"
    break;

  case 8: /* decl: stmt  */
#line 122 "src/parser/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1307 "src/parser/parser.tab.cc"
    break;

  case 9: /* import_decl: IMPORT IDENTIFIER ARROW IDENTIFIER SEMI  */
#line 126 "src/parser/parser.y"
                                            { (yyval.node) = nullptr; }
#line 1313 "src/parser/parser.tab.cc"
    break;

  case 10: /* var_decl: IDENTIFIER COLON type ASSIGN expr SEMI  */
#line 130 "src/parser/parser.y"
                                             { 
          auto var = std::make_unique<ambar::VarNode>(std::string((yyvsp[-5].id)), *(yyvsp[-3].str), std::unique_ptr<ambar::ASTNode>((yyvsp[-1].node)));
          delete (yyvsp[-3].str);
          free((yyvsp[-5].id));
          (yyval.node) = var.release();
      }
#line 1324 "src/parser/parser.tab.cc"
    break;

  case 11: /* var_decl: IDENTIFIER COLON type SEMI  */
#line 136 "src/parser/parser.y"
                                 { 
          auto var = std::make_unique<ambar::VarNode>(std::string((yyvsp[-3].id)), *(yyvsp[-1].str), nullptr);
          delete (yyvsp[-1].str);
          free((yyvsp[-3].id));
          (yyval.node) = var.release();
      }
#line 1335 "src/parser/parser.tab.cc"
    break;

  case 12: /* func_decl: FUNC IDENTIFIER LPAREN opt_params RPAREN ARROW type block  */
#line 145 "src/parser/parser.y"
                                                              {
        auto params_vec = ((yyvsp[-4].params) != nullptr) ? *(yyvsp[-4].params) : std::vector<std::pair<std::string, std::string>>();
        auto func = std::make_unique<ambar::FunctionNode>(
            std::string((yyvsp[-6].id)), 
            *(yyvsp[-1].str),
            std::move(params_vec),
            std::unique_ptr<ambar::BlockNode>(dynamic_cast<ambar::BlockNode*>((yyvsp[0].node)))
        );
        delete (yyvsp[-4].params); 
        delete (yyvsp[-1].str);
        free((yyvsp[-6].id));
        (yyval.node) = func.release();
    }
#line 1353 "src/parser/parser.tab.cc"
    break;

  case 13: /* opt_params: %empty  */
#line 161 "src/parser/parser.y"
                  { (yyval.params) = nullptr; }
#line 1359 "src/parser/parser.tab.cc"
    break;

  case 14: /* opt_params: params  */
#line 162 "src/parser/parser.y"
             { (yyval.params) = (yyvsp[0].params); }
#line 1365 "src/parser/parser.tab.cc"
    break;

  case 15: /* params: IDENTIFIER COLON type  */
#line 166 "src/parser/parser.y"
                            {
          (yyval.params) = new std::vector<std::pair<std::string, std::string>>();
          (yyval.params)->push_back({std::string((yyvsp[-2].id)), *(yyvsp[0].str)});
          delete (yyvsp[0].str); 
          free((yyvsp[-2].id));
      }
#line 1376 "src/parser/parser.tab.cc"
    break;

  case 16: /* params: params COMMA IDENTIFIER COLON type  */
#line 172 "src/parser/parser.y"
                                         {
          (yyvsp[-4].params)->push_back({std::string((yyvsp[-2].id)), *(yyvsp[0].str)});
          delete (yyvsp[0].str); 
          free((yyvsp[-2].id));
          (yyval.params) = (yyvsp[-4].params);
      }
#line 1387 "src/parser/parser.tab.cc"
    break;

  case 17: /* type: INT  */
#line 181 "src/parser/parser.y"
              { (yyval.str) = new std::string("int"); }
#line 1393 "src/parser/parser.tab.cc"
    break;

  case 18: /* type: FLOAT  */
#line 182 "src/parser/parser.y"
              { (yyval.str) = new std::string("float"); }
#line 1399 "src/parser/parser.tab.cc"
    break;

  case 19: /* type: BOOL  */
#line 183 "src/parser/parser.y"
              { (yyval.str) = new std::string("bool"); }
#line 1405 "src/parser/parser.tab.cc"
    break;

  case 20: /* type: STRING_T  */
#line 184 "src/parser/parser.y"
              { (yyval.str) = new std::string("string"); }
#line 1411 "src/parser/parser.tab.cc"
    break;

  case 21: /* type: VOID  */
#line 185 "src/parser/parser.y"
              { (yyval.str) = new std::string("void"); }
#line 1417 "src/parser/parser.tab.cc"
    break;

  case 22: /* stmt: assign_stmt  */
#line 189 "src/parser/parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1423 "src/parser/parser.tab.cc"
    break;

  case 23: /* stmt: call_stmt  */
#line 190 "src/parser/parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1429 "src/parser/parser.tab.cc"
    break;

  case 24: /* stmt: return_stmt  */
#line 191 "src/parser/parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1435 "src/parser/parser.tab.cc"
    break;

  case 25: /* stmt: if_stmt  */
#line 192 "src/parser/parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1441 "src/parser/parser.tab.cc"
    break;

  case 26: /* stmt: while_stmt  */
#line 193 "src/parser/parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1447 "src/parser/parser.tab.cc"
    break;

  case 27: /* stmt: for_stmt  */
#line 194 "src/parser/parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1453 "src/parser/parser.tab.cc"
    break;

  case 28: /* stmt: break_stmt  */
#line 195 "src/parser/parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1459 "src/parser/parser.tab.cc"
    break;

  case 29: /* stmt: continue_stmt  */
#line 196 "src/parser/parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1465 "src/parser/parser.tab.cc"
    break;

  case 30: /* stmt: block  */
#line 197 "src/parser/parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1471 "src/parser/parser.tab.cc"
    break;

  case 31: /* assign_stmt: IDENTIFIER ASSIGN expr SEMI  */
#line 201 "src/parser/parser.y"
                                { 
        auto assign = std::make_unique<ambar::AssignNode>((yyvsp[-3].id), std::unique_ptr<ambar::ASTNode>((yyvsp[-1].node)));
        free((yyvsp[-3].id));
        (yyval.node) = assign.release();
    }
#line 1481 "src/parser/parser.tab.cc"
    break;

  case 32: /* call_stmt: func_call SEMI  */
#line 209 "src/parser/parser.y"
                   { (yyval.node) = (yyvsp[-1].node); }
#line 1487 "src/parser/parser.tab.cc"
    break;

  case 33: /* return_stmt: RETURN expr SEMI  */
#line 213 "src/parser/parser.y"
                       { 
          auto ret = std::make_unique<ambar::ReturnNode>(std::unique_ptr<ambar::ASTNode>((yyvsp[-1].node)));
          (yyval.node) = ret.release();
      }
#line 1496 "src/parser/parser.tab.cc"
    break;

  case 34: /* return_stmt: RETURN SEMI  */
#line 217 "src/parser/parser.y"
                  { 
          auto ret = std::make_unique<ambar::ReturnNode>();
          (yyval.node) = ret.release();
      }
#line 1505 "src/parser/parser.tab.cc"
    break;

  case 35: /* if_stmt: IF LPAREN expr RPAREN stmt  */
#line 224 "src/parser/parser.y"
                                 {
          auto ifNode = std::make_unique<ambar::IfNode>(
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = ifNode.release();
      }
#line 1517 "src/parser/parser.tab.cc"
    break;

  case 36: /* if_stmt: IF LPAREN expr RPAREN stmt ELSE stmt  */
#line 231 "src/parser/parser.y"
                                           {
          auto ifNode = std::make_unique<ambar::IfNode>(
              std::unique_ptr<ambar::ASTNode>((yyvsp[-4].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = ifNode.release();
      }
#line 1530 "src/parser/parser.tab.cc"
    break;

  case 37: /* while_stmt: WHILE LPAREN expr RPAREN stmt  */
#line 242 "src/parser/parser.y"
                                  {
        auto whileNode = std::make_unique<ambar::WhileNode>(
            std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
            std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
        );
        (yyval.node) = whileNode.release();
    }
#line 1542 "src/parser/parser.tab.cc"
    break;

  case 38: /* for_stmt: FOR LPAREN assign_stmt expr SEMI assign_stmt RPAREN stmt  */
#line 252 "src/parser/parser.y"
                                                             {
        auto forNode = std::make_unique<ambar::ForNode>(
            std::unique_ptr<ambar::ASTNode>((yyvsp[-5].node)),
            std::unique_ptr<ambar::ASTNode>((yyvsp[-4].node)),
            std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)),
            std::unique_ptr<ambar::BlockNode>(dynamic_cast<ambar::BlockNode*>((yyvsp[0].node)))
        );
        (yyval.node) = forNode.release();
    }
#line 1556 "src/parser/parser.tab.cc"
    break;

  case 39: /* break_stmt: BREAK SEMI  */
#line 264 "src/parser/parser.y"
               { 
        auto breakNode = std::make_unique<ambar::BreakNode>();
        (yyval.node) = breakNode.release();
    }
#line 1565 "src/parser/parser.tab.cc"
    break;

  case 40: /* continue_stmt: CONTINUE SEMI  */
#line 271 "src/parser/parser.y"
                  { 
        auto continueNode = std::make_unique<ambar::ContinueNode>();
        (yyval.node) = continueNode.release();
    }
#line 1574 "src/parser/parser.tab.cc"
    break;

  case 41: /* block: LBRACE RBRACE  */
#line 278 "src/parser/parser.y"
                  {
        auto blk = std::make_unique<ambar::BlockNode>();
        (yyval.node) = blk.release();
    }
#line 1583 "src/parser/parser.tab.cc"
    break;

  case 42: /* block: LBRACE stmt_list RBRACE  */
#line 282 "src/parser/parser.y"
                              {
        auto blk = std::make_unique<ambar::BlockNode>();
        if ((yyvsp[-1].stmts)) {
            for (ambar::ASTNode* stmt : *(yyvsp[-1].stmts)) {
                if (stmt) {
                    blk->add(std::unique_ptr<ambar::ASTNode>(stmt));
                }
            }
            delete (yyvsp[-1].stmts);
        }
        (yyval.node) = blk.release();
    }
#line 1600 "src/parser/parser.tab.cc"
    break;

  case 43: /* stmt_list: stmt  */
#line 297 "src/parser/parser.y"
           {
        (yyval.stmts) = new std::vector<ambar::ASTNode*>();
        if ((yyvsp[0].node)) (yyval.stmts)->push_back((yyvsp[0].node));
      }
#line 1609 "src/parser/parser.tab.cc"
    break;

  case 44: /* stmt_list: stmt_list stmt  */
#line 301 "src/parser/parser.y"
                     {
        if ((yyvsp[0].node)) (yyvsp[-1].stmts)->push_back((yyvsp[0].node));
        (yyval.stmts) = (yyvsp[-1].stmts);
      }
#line 1618 "src/parser/parser.tab.cc"
    break;

  case 45: /* opt_args: %empty  */
#line 308 "src/parser/parser.y"
                  { (yyval.stmts) = new std::vector<ambar::ASTNode*>(); }
#line 1624 "src/parser/parser.tab.cc"
    break;

  case 46: /* opt_args: args  */
#line 309 "src/parser/parser.y"
           { (yyval.stmts) = (yyvsp[0].stmts); }
#line 1630 "src/parser/parser.tab.cc"
    break;

  case 47: /* args: expr  */
#line 313 "src/parser/parser.y"
           { 
          (yyval.stmts) = new std::vector<ambar::ASTNode*>(); 
          if ((yyvsp[0].node)) (yyval.stmts)->push_back((yyvsp[0].node)); 
      }
#line 1639 "src/parser/parser.tab.cc"
    break;

  case 48: /* args: args COMMA expr  */
#line 317 "src/parser/parser.y"
                      { 
          if ((yyvsp[0].node)) (yyvsp[-2].stmts)->push_back((yyvsp[0].node)); 
          (yyval.stmts) = (yyvsp[-2].stmts); 
      }
#line 1648 "src/parser/parser.tab.cc"
    break;

  case 49: /* expr: logic_expr  */
#line 324 "src/parser/parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1654 "src/parser/parser.tab.cc"
    break;

  case 50: /* logic_expr: rel_expr  */
#line 328 "src/parser/parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1660 "src/parser/parser.tab.cc"
    break;

  case 51: /* logic_expr: logic_expr AND rel_expr  */
#line 329 "src/parser/parser.y"
                              { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "AND", 
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = binNode.release();
      }
#line 1673 "src/parser/parser.tab.cc"
    break;

  case 52: /* logic_expr: logic_expr OR rel_expr  */
#line 337 "src/parser/parser.y"
                             { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "OR", 
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = binNode.release();
      }
#line 1686 "src/parser/parser.tab.cc"
    break;

  case 53: /* rel_expr: arith_expr  */
#line 348 "src/parser/parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1692 "src/parser/parser.tab.cc"
    break;

  case 54: /* rel_expr: arith_expr EQ arith_expr  */
#line 349 "src/parser/parser.y"
                               { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "==", 
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = binNode.release();
      }
#line 1705 "src/parser/parser.tab.cc"
    break;

  case 55: /* rel_expr: arith_expr NEQ arith_expr  */
#line 357 "src/parser/parser.y"
                                { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "!=", 
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = binNode.release();
      }
#line 1718 "src/parser/parser.tab.cc"
    break;

  case 56: /* rel_expr: arith_expr LT arith_expr  */
#line 365 "src/parser/parser.y"
                               { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "<", 
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = binNode.release();
      }
#line 1731 "src/parser/parser.tab.cc"
    break;

  case 57: /* rel_expr: arith_expr LE arith_expr  */
#line 373 "src/parser/parser.y"
                               { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "<=", 
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = binNode.release();
      }
#line 1744 "src/parser/parser.tab.cc"
    break;

  case 58: /* rel_expr: arith_expr GT arith_expr  */
#line 381 "src/parser/parser.y"
                               { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              ">", 
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = binNode.release();
      }
#line 1757 "src/parser/parser.tab.cc"
    break;

  case 59: /* rel_expr: arith_expr GE arith_expr  */
#line 389 "src/parser/parser.y"
                               { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              ">=", 
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = binNode.release();
      }
#line 1770 "src/parser/parser.tab.cc"
    break;

  case 60: /* arith_expr: term  */
#line 400 "src/parser/parser.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1776 "src/parser/parser.tab.cc"
    break;

  case 61: /* arith_expr: arith_expr ADD term  */
#line 401 "src/parser/parser.y"
                          { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "+", 
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = binNode.release();
      }
#line 1789 "src/parser/parser.tab.cc"
    break;

  case 62: /* arith_expr: arith_expr SUB term  */
#line 409 "src/parser/parser.y"
                          { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "-", 
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = binNode.release();
      }
#line 1802 "src/parser/parser.tab.cc"
    break;

  case 63: /* term: factor  */
#line 420 "src/parser/parser.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 1808 "src/parser/parser.tab.cc"
    break;

  case 64: /* term: term MUL factor  */
#line 421 "src/parser/parser.y"
                      { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "*", 
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = binNode.release();
      }
#line 1821 "src/parser/parser.tab.cc"
    break;

  case 65: /* term: term DIV factor  */
#line 429 "src/parser/parser.y"
                      { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "/", 
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = binNode.release();
      }
#line 1834 "src/parser/parser.tab.cc"
    break;

  case 66: /* factor: NUM_INT  */
#line 440 "src/parser/parser.y"
                     { 
          auto numNode = std::make_unique<ambar::NumberNode>((yyvsp[0].num));
          (yyval.node) = numNode.release();
      }
#line 1843 "src/parser/parser.tab.cc"
    break;

  case 67: /* factor: NUM_REAL  */
#line 444 "src/parser/parser.y"
                     { 
          auto numNode = std::make_unique<ambar::NumberNode>((yyvsp[0].real));
          (yyval.node) = numNode.release();
      }
#line 1852 "src/parser/parser.tab.cc"
    break;

  case 68: /* factor: IDENTIFIER  */
#line 448 "src/parser/parser.y"
                     { 
          auto idNode = std::make_unique<ambar::IdentifierNode>((yyvsp[0].id));
          free((yyvsp[0].id));
          (yyval.node) = idNode.release();
      }
#line 1862 "src/parser/parser.tab.cc"
    break;

  case 69: /* factor: STRING  */
#line 453 "src/parser/parser.y"
                     { 
          auto strNode = std::make_unique<ambar::StringNode>(*(yyvsp[0].str));
          delete (yyvsp[0].str);
          (yyval.node) = strNode.release();
      }
#line 1872 "src/parser/parser.tab.cc"
    break;

  case 70: /* factor: BOOL_TRUE  */
#line 458 "src/parser/parser.y"
                     { 
          auto boolNode = std::make_unique<ambar::BoolNode>(true);
          (yyval.node) = boolNode.release();
      }
#line 1881 "src/parser/parser.tab.cc"
    break;

  case 71: /* factor: BOOL_FALSE  */
#line 462 "src/parser/parser.y"
                     { 
          auto boolNode = std::make_unique<ambar::BoolNode>(false);
          (yyval.node) = boolNode.release();
      }
#line 1890 "src/parser/parser.tab.cc"
    break;

  case 72: /* factor: func_call  */
#line 466 "src/parser/parser.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1896 "src/parser/parser.tab.cc"
    break;

  case 73: /* factor: LPAREN expr RPAREN  */
#line 467 "src/parser/parser.y"
                         { (yyval.node) = (yyvsp[-1].node); }
#line 1902 "src/parser/parser.tab.cc"
    break;

  case 74: /* func_call: IDENTIFIER LPAREN opt_args RPAREN  */
#line 471 "src/parser/parser.y"
                                      { 
        std::vector<std::unique_ptr<ambar::ASTNode>> args;
        if ((yyvsp[-1].stmts)) {
            for (auto* arg : *(yyvsp[-1].stmts)) {
                if (arg) {
                    args.push_back(std::unique_ptr<ambar::ASTNode>(arg));
                }
            }
            delete (yyvsp[-1].stmts);
        }
        auto callNode = std::make_unique<ambar::CallNode>(std::string((yyvsp[-3].id)), std::move(args));
        free((yyvsp[-3].id));
        (yyval.node) = callNode.release();
    }
#line 1921 "src/parser/parser.tab.cc"
    break;


#line 1925 "src/parser/parser.tab.cc"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 487 "src/parser/parser.y"


void yyerror(const char* s) {
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, s);
}

void cleanup() {
    // A limpeza é feita automaticamente pelos unique_ptr
}
