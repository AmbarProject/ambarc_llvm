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
#line 30 "parser.y"

#include <cstdio>
#include <memory>
#include <cstdlib>

extern "C" {
    int yylex();
    int yyparse();
    void yyerror(const char *s);
    extern FILE *yyin;
    extern int yylineno;
}

namespace ambar {
    class ASTNode;
    extern std::unique_ptr<ASTNode> astRoot;
}

using namespace ambar;

std::unique_ptr<ambar::ASTNode> ambar::astRoot = nullptr;

#line 94 "parser.tab.cc"

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
  YYSYMBOL_UMINUS = 47,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 48,                  /* $accept  */
  YYSYMBOL_program = 49,                   /* program  */
  YYSYMBOL_decl_list = 50,                 /* decl_list  */
  YYSYMBOL_decl = 51,                      /* decl  */
  YYSYMBOL_import_decl = 52,               /* import_decl  */
  YYSYMBOL_var_decl = 53,                  /* var_decl  */
  YYSYMBOL_func_decl = 54,                 /* func_decl  */
  YYSYMBOL_opt_params = 55,                /* opt_params  */
  YYSYMBOL_params = 56,                    /* params  */
  YYSYMBOL_type = 57,                      /* type  */
  YYSYMBOL_stmt = 58,                      /* stmt  */
  YYSYMBOL_assign_stmt = 59,               /* assign_stmt  */
  YYSYMBOL_call_stmt = 60,                 /* call_stmt  */
  YYSYMBOL_return_stmt = 61,               /* return_stmt  */
  YYSYMBOL_if_stmt = 62,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 63,                /* while_stmt  */
  YYSYMBOL_for_var_decl = 64,              /* for_var_decl  */
  YYSYMBOL_for_assign_stmt = 65,           /* for_assign_stmt  */
  YYSYMBOL_for_init = 66,                  /* for_init  */
  YYSYMBOL_opt_expr = 67,                  /* opt_expr  */
  YYSYMBOL_for_stmt = 68,                  /* for_stmt  */
  YYSYMBOL_break_stmt = 69,                /* break_stmt  */
  YYSYMBOL_continue_stmt = 70,             /* continue_stmt  */
  YYSYMBOL_block = 71,                     /* block  */
  YYSYMBOL_stmt_list = 72,                 /* stmt_list  */
  YYSYMBOL_opt_args = 73,                  /* opt_args  */
  YYSYMBOL_args = 74,                      /* args  */
  YYSYMBOL_expr = 75,                      /* expr  */
  YYSYMBOL_logic_expr = 76,                /* logic_expr  */
  YYSYMBOL_rel_expr = 77,                  /* rel_expr  */
  YYSYMBOL_arith_expr = 78,                /* arith_expr  */
  YYSYMBOL_term = 79,                      /* term  */
  YYSYMBOL_factor = 80,                    /* factor  */
  YYSYMBOL_func_call = 81                  /* func_call  */
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
#define YYFINAL  58
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   191

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  86
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  155

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   302


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
      45,    46,    47
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   100,   100,   116,   121,   131,   132,   133,   137,   141,
     147,   156,   178,   179,   183,   189,   197,   198,   199,   200,
     201,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   218,   226,   230,   234,   241,   248,   259,   269,   275,
     284,   296,   297,   298,   299,   304,   305,   309,   321,   328,
     335,   339,   354,   358,   365,   366,   370,   371,   375,   379,
     380,   388,   399,   400,   404,   408,   412,   416,   420,   427,
     428,   432,   439,   440,   444,   448,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   468
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
  "LBRACE", "RBRACE", "UMINUS", "$accept", "program", "decl_list", "decl",
  "import_decl", "var_decl", "func_decl", "opt_params", "params", "type",
  "stmt", "assign_stmt", "call_stmt", "return_stmt", "if_stmt",
  "while_stmt", "for_var_decl", "for_assign_stmt", "for_init", "opt_expr",
  "for_stmt", "break_stmt", "continue_stmt", "block", "stmt_list",
  "opt_args", "args", "expr", "logic_expr", "rel_expr", "arith_expr",
  "term", "factor", "func_call", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-145)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      62,   -26,    10,    13,   125,   -39,   -20,   -13,    -5,     5,
      21,    49,    62,  -145,  -145,  -145,  -145,  -145,  -145,  -145,
    -145,  -145,  -145,  -145,  -145,  -145,  -145,     8,   132,    90,
     132,    22,    25,    37,  -145,  -145,  -145,  -145,  -145,   132,
     132,  -145,   132,    14,    16,  -145,   156,    -8,  -145,  -145,
     132,   132,   148,  -145,  -145,  -145,  -145,    47,  -145,  -145,
    -145,    42,  -145,  -145,  -145,  -145,  -145,   -17,    38,    44,
    -145,    87,    88,  -145,  -145,    56,  -145,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
      58,    59,   -22,  -145,  -145,    66,  -145,  -145,  -145,  -145,
     132,  -145,  -145,   132,    67,    75,    74,    78,  -145,  -145,
    -145,     9,     9,     9,     9,     9,     9,    -8,    -8,  -145,
    -145,  -145,    72,    72,   132,    90,   132,    77,  -145,  -145,
      90,    82,   118,   111,  -145,  -145,    89,   102,  -145,  -145,
    -145,    90,   104,    72,   132,   148,    97,    90,  -145,  -145,
     103,  -145,  -145,    97,  -145
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     2,     3,     5,    30,     6,     7,    21,    22,
      23,    24,    25,    26,    27,    28,    29,     0,     0,     0,
      54,     0,     0,    78,    76,    77,    79,    80,    81,     0,
       0,    34,     0,     0,    58,    59,    62,    69,    72,    82,
       0,     0,    44,    48,    49,    50,    52,     0,     1,     4,
      32,     0,    16,    17,    18,    19,    20,     0,     0,    55,
      56,     0,    12,    85,    84,     0,    33,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    78,    41,    42,     0,    43,    51,    53,    31,
       0,    10,    86,     0,     0,     0,     0,    13,    83,    60,
      61,    63,    64,    65,    66,    67,    68,    70,    71,    73,
      74,    75,     0,     0,     0,     0,    45,     0,    57,     8,
       0,     0,     0,    35,    37,    40,    39,     0,    46,     9,
      14,     0,     0,     0,     0,    44,     0,     0,    36,    38,
       0,    11,    15,     0,    47
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -145,  -145,  -145,   134,  -145,  -145,  -145,  -145,  -145,  -122,
      -9,  -145,  -145,  -145,  -145,  -145,  -145,  -145,     3,  -145,
    -145,  -145,  -145,  -144,  -145,  -145,  -145,     1,  -145,   -32,
      15,   -16,   -33,     0
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    11,    12,    13,    14,    15,    16,   106,   107,    67,
      17,    18,    19,    20,    21,    22,    93,    94,    95,   137,
      23,    24,    25,    26,    57,    68,    69,    96,    44,    45,
      46,    47,    48,    49
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      27,    56,   151,   136,    50,    43,    73,    74,   140,   154,
      27,    28,    27,    31,    29,   124,    32,    30,   125,   146,
     100,    30,   101,    51,     1,   152,    87,    88,    89,    61,
      52,    70,     4,     5,    53,     6,     7,     8,     9,    77,
      78,    85,    86,    75,    54,   109,   110,    60,    98,    58,
       1,    90,    91,    76,   119,   120,   121,    27,     4,     5,
      71,     6,     7,     8,     9,     1,    10,    55,    72,   117,
     118,     2,     3,     4,     5,     1,     6,     7,     8,     9,
      30,    99,   102,     4,     5,   103,     6,     7,     8,     9,
     104,   105,    10,    97,   111,   112,   113,   114,   115,   116,
     108,   127,   122,   123,   128,   126,   129,    10,    62,    63,
      64,    65,    66,   133,   134,   130,   139,    10,   131,   132,
     141,   142,    27,    27,   143,   135,   144,   138,    33,    34,
      35,    36,    37,    38,   148,    33,    34,    35,    36,    37,
      38,   145,    10,    27,   147,   149,    59,   153,   150,     0,
      39,    92,    34,    35,    36,    37,    38,    39,    40,     0,
       0,     0,     0,     0,    41,    40,     0,     0,    42,     0,
       0,     0,     0,    39,     0,    42,     0,     0,     0,     0,
       0,    40,    79,    80,    81,    82,    83,    84,    85,    86,
       0,    42
};

static const yytype_int16 yycheck[] =
{
       0,    10,   146,   125,    43,     4,    39,    40,   130,   153,
      10,    37,    12,     3,    40,    37,     3,    43,    40,   141,
      37,    43,    39,    43,     3,   147,    34,    35,    36,    28,
      43,    30,    11,    12,    39,    14,    15,    16,    17,    23,
      24,    32,    33,    42,    39,    77,    78,    39,    57,     0,
       3,    50,    51,    39,    87,    88,    89,    57,    11,    12,
      38,    14,    15,    16,    17,     3,    45,    46,    43,    85,
      86,     9,    10,    11,    12,     3,    14,    15,    16,    17,
      43,    39,    44,    11,    12,    41,    14,    15,    16,    17,
       3,     3,    45,    46,    79,    80,    81,    82,    83,    84,
      44,   100,    44,    44,   103,    39,    39,    45,    18,    19,
      20,    21,    22,   122,   123,    40,    39,    45,    44,    41,
      38,     3,   122,   123,    13,   124,    37,   126,     3,     4,
       5,     6,     7,     8,   143,     3,     4,     5,     6,     7,
       8,    39,    45,   143,    40,   144,    12,    44,   145,    -1,
      25,     3,     4,     5,     6,     7,     8,    25,    33,    -1,
      -1,    -1,    -1,    -1,    39,    33,    -1,    -1,    43,    -1,
      -1,    -1,    -1,    25,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    33,    26,    27,    28,    29,    30,    31,    32,    33,
      -1,    43
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     9,    10,    11,    12,    14,    15,    16,    17,
      45,    49,    50,    51,    52,    53,    54,    58,    59,    60,
      61,    62,    63,    68,    69,    70,    71,    81,    37,    40,
      43,     3,     3,     3,     4,     5,     6,     7,     8,    25,
      33,    39,    43,    75,    76,    77,    78,    79,    80,    81,
      43,    43,    43,    39,    39,    46,    58,    72,     0,    51,
      39,    75,    18,    19,    20,    21,    22,    57,    73,    74,
      75,    38,    43,    80,    80,    75,    39,    23,    24,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      75,    75,     3,    64,    65,    66,    75,    46,    58,    39,
      37,    39,    44,    41,     3,     3,    55,    56,    44,    77,
      77,    78,    78,    78,    78,    78,    78,    79,    79,    80,
      80,    80,    44,    44,    37,    40,    39,    75,    75,    39,
      40,    44,    41,    58,    58,    75,    57,    67,    75,    39,
      57,    38,     3,    13,    37,    39,    57,    40,    58,    75,
      66,    71,    57,    44,    71
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    48,    49,    50,    50,    51,    51,    51,    52,    53,
      53,    54,    55,    55,    56,    56,    57,    57,    57,    57,
      57,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    59,    60,    61,    61,    62,    62,    63,    64,    64,
      65,    66,    66,    66,    66,    67,    67,    68,    69,    70,
      71,    71,    72,    72,    73,    73,    74,    74,    75,    76,
      76,    76,    77,    77,    77,    77,    77,    77,    77,    78,
      78,    78,    79,    79,    79,    79,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    81
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     5,     6,
       4,     8,     0,     1,     3,     5,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     2,     3,     2,     5,     7,     5,     5,     3,
       3,     1,     1,     1,     0,     0,     1,     9,     2,     2,
       2,     3,     1,     2,     0,     1,     1,     3,     1,     1,
       3,     3,     1,     3,     3,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     3,     2,     2,     4
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
#line 100 "parser.y"
              {
        /* decl_list sempre retorna um ProgramNode* (ou nullptr) */
        if ((yyvsp[0].node) == nullptr) {
            /* grammar vazia -> criar ProgramNode vazio */
            auto prog = std::make_unique<ambar::ProgramNode>();
            ambar::astRoot = std::move(prog);
            (yyval.node) = ambar::astRoot.get();
        } else {
            /* assumir ownership do ProgramNode retornado */
            ambar::astRoot = std::unique_ptr<ambar::ASTNode>((yyvsp[0].node));
            (yyval.node) = ambar::astRoot.get();
        }
    }
#line 1289 "parser.tab.cc"
    break;

  case 3: /* decl_list: decl  */
#line 116 "parser.y"
           {
          auto p = std::make_unique<ambar::ProgramNode>();
          if ((yyvsp[0].node)) p->addDeclaration(std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
          (yyval.node) = p.release();
      }
#line 1299 "parser.tab.cc"
    break;

  case 4: /* decl_list: decl_list decl  */
#line 121 "parser.y"
                     {
          ambar::ProgramNode* p = dynamic_cast<ambar::ProgramNode*>((yyvsp[-1].node));
          if (p && (yyvsp[0].node)) {
              p->addDeclaration(std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
          }
          (yyval.node) = p;
      }
#line 1311 "parser.tab.cc"
    break;

  case 5: /* decl: import_decl  */
#line 131 "parser.y"
                          { (yyval.node) = nullptr; }
#line 1317 "parser.tab.cc"
    break;

  case 6: /* decl: func_decl  */
#line 132 "parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1323 "parser.tab.cc"
    break;

  case 7: /* decl: stmt  */
#line 133 "parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1329 "parser.tab.cc"
    break;

  case 8: /* import_decl: IMPORT IDENTIFIER ARROW IDENTIFIER SEMI  */
#line 137 "parser.y"
                                            { (yyval.node) = nullptr; }
#line 1335 "parser.tab.cc"
    break;

  case 9: /* var_decl: IDENTIFIER COLON type ASSIGN expr SEMI  */
#line 141 "parser.y"
                                             { 
          auto var = std::make_unique<ambar::VarNode>(std::string((yyvsp[-5].id)), *(yyvsp[-3].str), std::unique_ptr<ambar::ASTNode>((yyvsp[-1].node)));
          delete (yyvsp[-3].str);
          free((yyvsp[-5].id));
          (yyval.node) = var.release();
      }
#line 1346 "parser.tab.cc"
    break;

  case 10: /* var_decl: IDENTIFIER COLON type SEMI  */
#line 147 "parser.y"
                                 { 
          auto var = std::make_unique<ambar::VarNode>(std::string((yyvsp[-3].id)), *(yyvsp[-1].str), nullptr);
          delete (yyvsp[-1].str);
          free((yyvsp[-3].id));
          (yyval.node) = var.release();
      }
#line 1357 "parser.tab.cc"
    break;

  case 11: /* func_decl: FUNC IDENTIFIER LPAREN opt_params RPAREN ARROW type block  */
#line 156 "parser.y"
                                                              {
        /* Construir lista de params: se $4 == nullptr -> lista vazia */
        std::vector<std::pair<std::string, std::string>> params_vec;
        if ((yyvsp[-4].params) != nullptr) {
            params_vec = std::move(*(yyvsp[-4].params));
            delete (yyvsp[-4].params);
        }
        std::string retType = *(yyvsp[-1].str);
        delete (yyvsp[-1].str);

        auto func = std::make_unique<ambar::FunctionNode>(
            std::string((yyvsp[-6].id)),
            retType,
            std::move(params_vec),
            std::unique_ptr<ambar::BlockNode>(dynamic_cast<ambar::BlockNode*>((yyvsp[0].node)))
        );
        free((yyvsp[-6].id));
        (yyval.node) = func.release();
    }
#line 1381 "parser.tab.cc"
    break;

  case 12: /* opt_params: %empty  */
#line 178 "parser.y"
                  { (yyval.params) = nullptr; }
#line 1387 "parser.tab.cc"
    break;

  case 13: /* opt_params: params  */
#line 179 "parser.y"
             { (yyval.params) = (yyvsp[0].params); }
#line 1393 "parser.tab.cc"
    break;

  case 14: /* params: IDENTIFIER COLON type  */
#line 183 "parser.y"
                            {
          (yyval.params) = new std::vector<std::pair<std::string, std::string>>();
          (yyval.params)->push_back({*(yyvsp[0].str), std::string((yyvsp[-2].id))}); // CORREÇÃO: tipo, nome
          delete (yyvsp[0].str); 
          free((yyvsp[-2].id));
      }
#line 1404 "parser.tab.cc"
    break;

  case 15: /* params: params COMMA IDENTIFIER COLON type  */
#line 189 "parser.y"
                                         {
          (yyvsp[-4].params)->push_back({*(yyvsp[0].str), std::string((yyvsp[-2].id))}); // CORREÇÃO: tipo, nome
          delete (yyvsp[0].str); 
          free((yyvsp[-2].id));
          (yyval.params) = (yyvsp[-4].params);
      }
#line 1415 "parser.tab.cc"
    break;

  case 16: /* type: INT  */
#line 197 "parser.y"
              { (yyval.str) = new std::string("int"); }
#line 1421 "parser.tab.cc"
    break;

  case 17: /* type: FLOAT  */
#line 198 "parser.y"
              { (yyval.str) = new std::string("float"); }
#line 1427 "parser.tab.cc"
    break;

  case 18: /* type: BOOL  */
#line 199 "parser.y"
              { (yyval.str) = new std::string("bool"); }
#line 1433 "parser.tab.cc"
    break;

  case 19: /* type: STRING_T  */
#line 200 "parser.y"
              { (yyval.str) = new std::string("string"); }
#line 1439 "parser.tab.cc"
    break;

  case 20: /* type: VOID  */
#line 201 "parser.y"
              { (yyval.str) = new std::string("void"); }
#line 1445 "parser.tab.cc"
    break;

  case 21: /* stmt: assign_stmt  */
#line 205 "parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1451 "parser.tab.cc"
    break;

  case 22: /* stmt: call_stmt  */
#line 206 "parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1457 "parser.tab.cc"
    break;

  case 23: /* stmt: return_stmt  */
#line 207 "parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1463 "parser.tab.cc"
    break;

  case 24: /* stmt: if_stmt  */
#line 208 "parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1469 "parser.tab.cc"
    break;

  case 25: /* stmt: while_stmt  */
#line 209 "parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1475 "parser.tab.cc"
    break;

  case 26: /* stmt: for_stmt  */
#line 210 "parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1481 "parser.tab.cc"
    break;

  case 27: /* stmt: break_stmt  */
#line 211 "parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1487 "parser.tab.cc"
    break;

  case 28: /* stmt: continue_stmt  */
#line 212 "parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1493 "parser.tab.cc"
    break;

  case 29: /* stmt: block  */
#line 213 "parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1499 "parser.tab.cc"
    break;

  case 30: /* stmt: var_decl  */
#line 214 "parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1505 "parser.tab.cc"
    break;

  case 31: /* assign_stmt: IDENTIFIER ASSIGN expr SEMI  */
#line 218 "parser.y"
                                {
        auto assign = std::make_unique<ambar::AssignNode>(std::string((yyvsp[-3].id)), std::unique_ptr<ambar::ASTNode>((yyvsp[-1].node)));
        free((yyvsp[-3].id));
        (yyval.node) = assign.release();
    }
#line 1515 "parser.tab.cc"
    break;

  case 32: /* call_stmt: func_call SEMI  */
#line 226 "parser.y"
                   { (yyval.node) = (yyvsp[-1].node); }
#line 1521 "parser.tab.cc"
    break;

  case 33: /* return_stmt: RETURN expr SEMI  */
#line 230 "parser.y"
                       { 
          auto ret = std::make_unique<ambar::ReturnNode>(std::unique_ptr<ambar::ASTNode>((yyvsp[-1].node)));
          (yyval.node) = ret.release();
      }
#line 1530 "parser.tab.cc"
    break;

  case 34: /* return_stmt: RETURN SEMI  */
#line 234 "parser.y"
                  { 
          auto ret = std::make_unique<ambar::ReturnNode>();
          (yyval.node) = ret.release();
      }
#line 1539 "parser.tab.cc"
    break;

  case 35: /* if_stmt: IF LPAREN expr RPAREN stmt  */
#line 241 "parser.y"
                                 {
          auto ifNode = std::make_unique<ambar::IfNode>(
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = ifNode.release();
      }
#line 1551 "parser.tab.cc"
    break;

  case 36: /* if_stmt: IF LPAREN expr RPAREN stmt ELSE stmt  */
#line 248 "parser.y"
                                           {
          auto ifNode = std::make_unique<ambar::IfNode>(
              std::unique_ptr<ambar::ASTNode>((yyvsp[-4].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = ifNode.release();
      }
#line 1564 "parser.tab.cc"
    break;

  case 37: /* while_stmt: WHILE LPAREN expr RPAREN stmt  */
#line 259 "parser.y"
                                  {
        auto whileNode = std::make_unique<ambar::WhileNode>(
            std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
            std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
        );
        (yyval.node) = whileNode.release();
    }
#line 1576 "parser.tab.cc"
    break;

  case 38: /* for_var_decl: IDENTIFIER COLON type ASSIGN expr  */
#line 269 "parser.y"
                                        {
          auto var = std::make_unique<ambar::VarNode>(std::string((yyvsp[-4].id)), *(yyvsp[-2].str), std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
          delete (yyvsp[-2].str);
          free((yyvsp[-4].id));
          (yyval.node) = var.release();
      }
#line 1587 "parser.tab.cc"
    break;

  case 39: /* for_var_decl: IDENTIFIER COLON type  */
#line 275 "parser.y"
                            {
          auto var = std::make_unique<ambar::VarNode>(std::string((yyvsp[-2].id)), *(yyvsp[0].str), nullptr);
          delete (yyvsp[0].str);
          free((yyvsp[-2].id));
          (yyval.node) = var.release();
      }
#line 1598 "parser.tab.cc"
    break;

  case 40: /* for_assign_stmt: IDENTIFIER ASSIGN expr  */
#line 284 "parser.y"
                           {
        auto assign = std::make_unique<ambar::AssignNode>(
            std::string((yyvsp[-2].id)), 
            std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
        );
        free((yyvsp[-2].id));
        (yyval.node) = assign.release();
    }
#line 1611 "parser.tab.cc"
    break;

  case 41: /* for_init: for_var_decl  */
#line 296 "parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1617 "parser.tab.cc"
    break;

  case 42: /* for_init: for_assign_stmt  */
#line 297 "parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1623 "parser.tab.cc"
    break;

  case 43: /* for_init: expr  */
#line 298 "parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1629 "parser.tab.cc"
    break;

  case 44: /* for_init: %empty  */
#line 299 "parser.y"
                   { (yyval.node) = nullptr; }
#line 1635 "parser.tab.cc"
    break;

  case 45: /* opt_expr: %empty  */
#line 304 "parser.y"
                  { (yyval.node) = nullptr; }
#line 1641 "parser.tab.cc"
    break;

  case 46: /* opt_expr: expr  */
#line 305 "parser.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 1647 "parser.tab.cc"
    break;

  case 47: /* for_stmt: FOR LPAREN for_init SEMI opt_expr SEMI for_init RPAREN block  */
#line 309 "parser.y"
                                                                 {
        auto forNode = std::make_unique<ambar::ForNode>(
            std::unique_ptr<ambar::ASTNode>((yyvsp[-6].node)),
            std::unique_ptr<ambar::ASTNode>((yyvsp[-4].node)),
            std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)),
            std::unique_ptr<ambar::BlockNode>(dynamic_cast<ambar::BlockNode*>((yyvsp[0].node)))
        );
        (yyval.node) = forNode.release();
    }
#line 1661 "parser.tab.cc"
    break;

  case 48: /* break_stmt: BREAK SEMI  */
#line 321 "parser.y"
               { 
        auto breakNode = std::make_unique<ambar::BreakNode>();
        (yyval.node) = breakNode.release();
    }
#line 1670 "parser.tab.cc"
    break;

  case 49: /* continue_stmt: CONTINUE SEMI  */
#line 328 "parser.y"
                  { 
        auto continueNode = std::make_unique<ambar::ContinueNode>();
        (yyval.node) = continueNode.release();
    }
#line 1679 "parser.tab.cc"
    break;

  case 50: /* block: LBRACE RBRACE  */
#line 335 "parser.y"
                  {
        auto blk = std::make_unique<ambar::BlockNode>();
        (yyval.node) = blk.release();
    }
#line 1688 "parser.tab.cc"
    break;

  case 51: /* block: LBRACE stmt_list RBRACE  */
#line 339 "parser.y"
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
#line 1705 "parser.tab.cc"
    break;

  case 52: /* stmt_list: stmt  */
#line 354 "parser.y"
           {
        (yyval.stmts) = new std::vector<ambar::ASTNode*>();
        if ((yyvsp[0].node)) (yyval.stmts)->push_back((yyvsp[0].node));
      }
#line 1714 "parser.tab.cc"
    break;

  case 53: /* stmt_list: stmt_list stmt  */
#line 358 "parser.y"
                     {
        if ((yyvsp[0].node)) (yyvsp[-1].stmts)->push_back((yyvsp[0].node));
        (yyval.stmts) = (yyvsp[-1].stmts);
      }
#line 1723 "parser.tab.cc"
    break;

  case 54: /* opt_args: %empty  */
#line 365 "parser.y"
                  { (yyval.stmts) = new std::vector<ambar::ASTNode*>(); }
#line 1729 "parser.tab.cc"
    break;

  case 55: /* opt_args: args  */
#line 366 "parser.y"
           { (yyval.stmts) = (yyvsp[0].stmts); }
#line 1735 "parser.tab.cc"
    break;

  case 56: /* args: expr  */
#line 370 "parser.y"
           { (yyval.stmts) = new std::vector<ambar::ASTNode*>(); if ((yyvsp[0].node)) (yyval.stmts)->push_back((yyvsp[0].node)); }
#line 1741 "parser.tab.cc"
    break;

  case 57: /* args: args COMMA expr  */
#line 371 "parser.y"
                      { if ((yyvsp[0].node)) (yyvsp[-2].stmts)->push_back((yyvsp[0].node)); (yyval.stmts) = (yyvsp[-2].stmts); }
#line 1747 "parser.tab.cc"
    break;

  case 58: /* expr: logic_expr  */
#line 375 "parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1753 "parser.tab.cc"
    break;

  case 59: /* logic_expr: rel_expr  */
#line 379 "parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1759 "parser.tab.cc"
    break;

  case 60: /* logic_expr: logic_expr AND rel_expr  */
#line 380 "parser.y"
                              { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "AND", 
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = binNode.release();
      }
#line 1772 "parser.tab.cc"
    break;

  case 61: /* logic_expr: logic_expr OR rel_expr  */
#line 388 "parser.y"
                             { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "OR", 
              std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), 
              std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))
          );
          (yyval.node) = binNode.release();
      }
#line 1785 "parser.tab.cc"
    break;

  case 62: /* rel_expr: arith_expr  */
#line 399 "parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1791 "parser.tab.cc"
    break;

  case 63: /* rel_expr: arith_expr EQ arith_expr  */
#line 400 "parser.y"
                               { 
          auto binNode = std::make_unique<ambar::BinaryNode>("==", std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
          (yyval.node) = binNode.release();
      }
#line 1800 "parser.tab.cc"
    break;

  case 64: /* rel_expr: arith_expr NEQ arith_expr  */
#line 404 "parser.y"
                                { 
          auto binNode = std::make_unique<ambar::BinaryNode>("!=", std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
          (yyval.node) = binNode.release();
      }
#line 1809 "parser.tab.cc"
    break;

  case 65: /* rel_expr: arith_expr LT arith_expr  */
#line 408 "parser.y"
                               { 
          auto binNode = std::make_unique<ambar::BinaryNode>("<", std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
          (yyval.node) = binNode.release();
      }
#line 1818 "parser.tab.cc"
    break;

  case 66: /* rel_expr: arith_expr LE arith_expr  */
#line 412 "parser.y"
                               { 
          auto binNode = std::make_unique<ambar::BinaryNode>("<=", std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
          (yyval.node) = binNode.release();
      }
#line 1827 "parser.tab.cc"
    break;

  case 67: /* rel_expr: arith_expr GT arith_expr  */
#line 416 "parser.y"
                               { 
          auto binNode = std::make_unique<ambar::BinaryNode>(">", std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
          (yyval.node) = binNode.release();
      }
#line 1836 "parser.tab.cc"
    break;

  case 68: /* rel_expr: arith_expr GE arith_expr  */
#line 420 "parser.y"
                               { 
          auto binNode = std::make_unique<ambar::BinaryNode>(">=", std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
          (yyval.node) = binNode.release();
      }
#line 1845 "parser.tab.cc"
    break;

  case 69: /* arith_expr: term  */
#line 427 "parser.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1851 "parser.tab.cc"
    break;

  case 70: /* arith_expr: arith_expr ADD term  */
#line 428 "parser.y"
                          { 
          auto binNode = std::make_unique<ambar::BinaryNode>("+", std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
          (yyval.node) = binNode.release();
      }
#line 1860 "parser.tab.cc"
    break;

  case 71: /* arith_expr: arith_expr SUB term  */
#line 432 "parser.y"
                          { 
          auto binNode = std::make_unique<ambar::BinaryNode>("-", std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
          (yyval.node) = binNode.release();
      }
#line 1869 "parser.tab.cc"
    break;

  case 72: /* term: factor  */
#line 439 "parser.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 1875 "parser.tab.cc"
    break;

  case 73: /* term: term MUL factor  */
#line 440 "parser.y"
                      { 
          auto binNode = std::make_unique<ambar::BinaryNode>("*", std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
          (yyval.node) = binNode.release();
      }
#line 1884 "parser.tab.cc"
    break;

  case 74: /* term: term DIV factor  */
#line 444 "parser.y"
                      { 
          auto binNode = std::make_unique<ambar::BinaryNode>("/", std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
          (yyval.node) = binNode.release();
      }
#line 1893 "parser.tab.cc"
    break;

  case 75: /* term: term MOD factor  */
#line 448 "parser.y"
                      {
          auto binNode = std::make_unique<ambar::BinaryNode>("%", std::unique_ptr<ambar::ASTNode>((yyvsp[-2].node)), std::unique_ptr<ambar::ASTNode>((yyvsp[0].node)));
          (yyval.node) = binNode.release();
      }
#line 1902 "parser.tab.cc"
    break;

  case 76: /* factor: NUM_INT  */
#line 455 "parser.y"
                     { auto numNode = std::make_unique<ambar::NumberNode>((yyvsp[0].num)); (yyval.node) = numNode.release(); }
#line 1908 "parser.tab.cc"
    break;

  case 77: /* factor: NUM_REAL  */
#line 456 "parser.y"
                     { auto numNode = std::make_unique<ambar::NumberNode>((yyvsp[0].real)); (yyval.node) = numNode.release(); }
#line 1914 "parser.tab.cc"
    break;

  case 78: /* factor: IDENTIFIER  */
#line 457 "parser.y"
                     { auto idNode = std::make_unique<ambar::IdentifierNode>((yyvsp[0].id)); free((yyvsp[0].id)); (yyval.node) = idNode.release(); }
#line 1920 "parser.tab.cc"
    break;

  case 79: /* factor: STRING  */
#line 458 "parser.y"
                     { auto strNode = std::make_unique<ambar::StringNode>(*(yyvsp[0].str)); delete (yyvsp[0].str); (yyval.node) = strNode.release(); }
#line 1926 "parser.tab.cc"
    break;

  case 80: /* factor: BOOL_TRUE  */
#line 459 "parser.y"
                     { auto boolNode = std::make_unique<ambar::BoolNode>(true); (yyval.node) = boolNode.release(); }
#line 1932 "parser.tab.cc"
    break;

  case 81: /* factor: BOOL_FALSE  */
#line 460 "parser.y"
                     { auto boolNode = std::make_unique<ambar::BoolNode>(false); (yyval.node) = boolNode.release(); }
#line 1938 "parser.tab.cc"
    break;

  case 82: /* factor: func_call  */
#line 461 "parser.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1944 "parser.tab.cc"
    break;

  case 83: /* factor: LPAREN expr RPAREN  */
#line 462 "parser.y"
                         { (yyval.node) = (yyvsp[-1].node); }
#line 1950 "parser.tab.cc"
    break;

  case 84: /* factor: SUB factor  */
#line 463 "parser.y"
                 { /* unary minus */ auto u = std::make_unique<ambar::UnaryNode>("-", std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))); (yyval.node) = u.release(); }
#line 1956 "parser.tab.cc"
    break;

  case 85: /* factor: NOT factor  */
#line 464 "parser.y"
                 { auto u = std::make_unique<ambar::UnaryNode>("NOT", std::unique_ptr<ambar::ASTNode>((yyvsp[0].node))); (yyval.node) = u.release(); }
#line 1962 "parser.tab.cc"
    break;

  case 86: /* func_call: IDENTIFIER LPAREN opt_args RPAREN  */
#line 468 "parser.y"
                                      { 
        std::vector<std::unique_ptr<ambar::ASTNode>> args;
        if ((yyvsp[-1].stmts)) {
            for (auto* arg : *(yyvsp[-1].stmts)) {
                if (arg) args.push_back(std::unique_ptr<ambar::ASTNode>(arg));
            }
            delete (yyvsp[-1].stmts);
        }
        auto callNode = std::make_unique<ambar::CallNode>(std::string((yyvsp[-3].id)), std::move(args));
        free((yyvsp[-3].id));
        (yyval.node) = callNode.release();
    }
#line 1979 "parser.tab.cc"
    break;


#line 1983 "parser.tab.cc"

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

#line 482 "parser.y"


void yyerror(const char* s) {
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, s);
}

void cleanup() {
    /* astRoot é unique_ptr — destruição automática */
    ambar::astRoot.reset();
}

