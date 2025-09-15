/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_PARSER_PARSER_TAB_HH_INCLUDED
# define YY_YY_SRC_PARSER_PARSER_TAB_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "src/parser/parser.y"

#include <cstdio>
#include <memory>
#include <vector>
#include <string>

#include "../ast/nodes/ASTNode.hpp"
#include "../ast/nodes/expressions/BinaryNode.hpp"
#include "../ast/nodes/expressions/BoolNode.hpp"
#include "../ast/nodes/expressions/CallNode.hpp"
#include "../ast/nodes/expressions/IdentifierNode.hpp"
#include "../ast/nodes/expressions/NumberNode.hpp"
#include "../ast/nodes/expressions/StringNode.hpp"
#include "../ast/nodes/expressions/UnaryNode.hpp"

#include "../ast/nodes/statements/AssignNode.hpp"
#include "../ast/nodes/statements/BlockNode.hpp"
#include "../ast/nodes/statements/BreakNode.hpp"
#include "../ast/nodes/statements/ContinueNode.hpp"
#include "../ast/nodes/statements/ForNode.hpp"
#include "../ast/nodes/statements/IfNode.hpp"
#include "../ast/nodes/statements/ReturnNode.hpp"
#include "../ast/nodes/statements/WhileNode.hpp"

#include "../ast/nodes/declarations/FunctionNode.hpp"
#include "../ast/nodes/declarations/ProgramNode.hpp"
#include "../ast/nodes/declarations/VarNode.hpp"

#line 78 "src/parser/parser.tab.hh"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    NUM_INT = 259,                 /* NUM_INT  */
    NUM_REAL = 260,                /* NUM_REAL  */
    STRING = 261,                  /* STRING  */
    BOOL_TRUE = 262,               /* BOOL_TRUE  */
    BOOL_FALSE = 263,              /* BOOL_FALSE  */
    IMPORT = 264,                  /* IMPORT  */
    FUNC = 265,                    /* FUNC  */
    RETURN = 266,                  /* RETURN  */
    IF = 267,                      /* IF  */
    ELSE = 268,                    /* ELSE  */
    WHILE = 269,                   /* WHILE  */
    FOR = 270,                     /* FOR  */
    BREAK = 271,                   /* BREAK  */
    CONTINUE = 272,                /* CONTINUE  */
    INT = 273,                     /* INT  */
    FLOAT = 274,                   /* FLOAT  */
    BOOL = 275,                    /* BOOL  */
    STRING_T = 276,                /* STRING_T  */
    VOID = 277,                    /* VOID  */
    AND = 278,                     /* AND  */
    OR = 279,                      /* OR  */
    NOT = 280,                     /* NOT  */
    EQ = 281,                      /* EQ  */
    NEQ = 282,                     /* NEQ  */
    LT = 283,                      /* LT  */
    LE = 284,                      /* LE  */
    GT = 285,                      /* GT  */
    GE = 286,                      /* GE  */
    ADD = 287,                     /* ADD  */
    SUB = 288,                     /* SUB  */
    MUL = 289,                     /* MUL  */
    DIV = 290,                     /* DIV  */
    MOD = 291,                     /* MOD  */
    ASSIGN = 292,                  /* ASSIGN  */
    ARROW = 293,                   /* ARROW  */
    SEMI = 294,                    /* SEMI  */
    COLON = 295,                   /* COLON  */
    COMMA = 296,                   /* COMMA  */
    DOT = 297,                     /* DOT  */
    LPAREN = 298,                  /* LPAREN  */
    RPAREN = 299,                  /* RPAREN  */
    LBRACE = 300,                  /* LBRACE  */
    RBRACE = 301                   /* RBRACE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 52 "src/parser/parser.y"

    int num;
    float real;
    bool bval;
    char* id;
    ambar::ASTNode* node;
    std::string* str;
    std::vector<ambar::ASTNode*>* stmts;
    std::vector<std::pair<std::string, std::string>>* params;

#line 152 "src/parser/parser.tab.hh"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SRC_PARSER_PARSER_TAB_HH_INCLUDED  */
