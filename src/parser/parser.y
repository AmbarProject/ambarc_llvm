%code requires {
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
}

%{
#include <cstdio>
#include <memory>

extern "C" {
    int yylex();
    int yyparse();
    void yyerror(const char *s);
    extern FILE *yyin;
    extern int yylineno;
}

namespace ambar {
    class ASTNode;
        std::unique_ptr<ASTNode> astRoot = nullptr;
}

using namespace ambar;

extern std::unique_ptr<ambar::ASTNode> ambar::astRoot;
%}

%union {
    int num;
    float real;
    bool bval;
    char* id;
    ambar::ASTNode* node;
    std::string* str;
    std::vector<ambar::ASTNode*>* stmts;
    std::vector<std::pair<std::string, std::string>>* params;
}

%type <node> program decl_list decl var_decl stmt assign_stmt call_stmt 
             return_stmt if_stmt while_stmt for_stmt break_stmt continue_stmt 
             block expr logic_expr rel_expr arith_expr term factor func_call
             func_decl import_decl

%type <str> type
%type <stmts> stmt_list
%type <params> params opt_params
%type <stmts> opt_args args

%token <id> IDENTIFIER
%token <num> NUM_INT
%token <real> NUM_REAL
%token <str> STRING
%token BOOL_TRUE BOOL_FALSE

%token IMPORT FUNC RETURN IF ELSE WHILE FOR BREAK CONTINUE
%token INT FLOAT BOOL STRING_T VOID
%token AND OR NOT EQ NEQ LT LE GT GE
%token ADD SUB MUL DIV MOD ASSIGN ARROW
%token SEMI COLON COMMA DOT LPAREN RPAREN LBRACE RBRACE

%start program

%%

program:
    decl_list {
        auto prog = std::make_unique<ambar::ProgramNode>();
        ambar::ProgramNode* existingProg = dynamic_cast<ambar::ProgramNode*>($1);
        if (existingProg) {
            astRoot = std::unique_ptr<ambar::ASTNode>($1);
        } else {
            prog->addDeclaration(std::unique_ptr<ambar::ASTNode>($1));
            astRoot = std::move(prog);
        }
        $$ = astRoot.get();
    }
;

decl_list:
      decl {
          auto p = std::make_unique<ambar::ProgramNode>();
          if ($1) {
              p->addDeclaration(std::unique_ptr<ambar::ASTNode>($1));
          }
          $$ = p.release();
      }
    | decl_list decl {
          ambar::ProgramNode* p = dynamic_cast<ambar::ProgramNode*>($1);
          if (p && $2) {
              p->addDeclaration(std::unique_ptr<ambar::ASTNode>($2));
          }
          $$ = p;
      }
;

decl: 
      import_decl         { $$ = nullptr; }
    | var_decl            { $$ = $1; }
    | func_decl           { $$ = $1; }
    | stmt                { $$ = $1; }
;

import_decl: 
    IMPORT IDENTIFIER ARROW IDENTIFIER SEMI { $$ = nullptr; }
;

var_decl:
      IDENTIFIER COLON type ASSIGN expr SEMI { 
          auto var = std::make_unique<ambar::VarNode>(std::string($1), *$3, std::unique_ptr<ambar::ASTNode>($5));
          delete $3;
          free($1);
          $$ = var.release();
      }
    | IDENTIFIER COLON type SEMI { 
          auto var = std::make_unique<ambar::VarNode>(std::string($1), *$3, nullptr);
          delete $3;
          free($1);
          $$ = var.release();
      }
;

func_decl: 
    FUNC IDENTIFIER LPAREN opt_params RPAREN ARROW type block {
        auto params_vec = ($4 != nullptr) ? *$4 : std::vector<std::pair<std::string, std::string>>();
        auto func = std::make_unique<ambar::FunctionNode>(
            std::string($2), 
            *$7,
            std::move(params_vec),
            std::unique_ptr<ambar::BlockNode>(dynamic_cast<ambar::BlockNode*>($8))
        );
        delete $4; 
        delete $7;
        free($2);
        $$ = func.release();
    }
;

opt_params: 
      /* empty */ { $$ = nullptr; }  
    | params { $$ = $1; } 
;

params: 
      IDENTIFIER COLON type {
          $$ = new std::vector<std::pair<std::string, std::string>>();
          $$->push_back({std::string($1), *$3});
          delete $3; 
          free($1);
      }
    | params COMMA IDENTIFIER COLON type {
          $1->push_back({std::string($3), *$5});
          delete $5; 
          free($3);
          $$ = $1;
      }
;

type:
    INT       { $$ = new std::string("int"); }
  | FLOAT     { $$ = new std::string("float"); }
  | BOOL      { $$ = new std::string("bool"); }
  | STRING_T  { $$ = new std::string("string"); }
  | VOID      { $$ = new std::string("void"); }
;

stmt: 
      assign_stmt      { $$ = $1; }
    | call_stmt        { $$ = $1; }
    | return_stmt      { $$ = $1; }
    | if_stmt          { $$ = $1; }
    | while_stmt       { $$ = $1; }
    | for_stmt         { $$ = $1; }
    | break_stmt       { $$ = $1; }
    | continue_stmt    { $$ = $1; }
    | block            { $$ = $1; }
;

assign_stmt: 
    IDENTIFIER ASSIGN expr SEMI { 
        auto assign = std::make_unique<ambar::AssignNode>($1, std::unique_ptr<ambar::ASTNode>($3));
        free($1);
        $$ = assign.release();
    }
;

call_stmt: 
    func_call SEMI { $$ = $1; }
;

return_stmt: 
      RETURN expr SEMI { 
          auto ret = std::make_unique<ambar::ReturnNode>(std::unique_ptr<ambar::ASTNode>($2));
          $$ = ret.release();
      }
    | RETURN SEMI { 
          auto ret = std::make_unique<ambar::ReturnNode>();
          $$ = ret.release();
      }
;

if_stmt: 
      IF LPAREN expr RPAREN stmt {
          auto ifNode = std::make_unique<ambar::IfNode>(
              std::unique_ptr<ambar::ASTNode>($3), 
              std::unique_ptr<ambar::ASTNode>($5)
          );
          $$ = ifNode.release();
      }
    | IF LPAREN expr RPAREN stmt ELSE stmt {
          auto ifNode = std::make_unique<ambar::IfNode>(
              std::unique_ptr<ambar::ASTNode>($3), 
              std::unique_ptr<ambar::ASTNode>($5), 
              std::unique_ptr<ambar::ASTNode>($7)
          );
          $$ = ifNode.release();
      }
;

while_stmt:
    WHILE LPAREN expr RPAREN stmt {
        auto whileNode = std::make_unique<ambar::WhileNode>(
            std::unique_ptr<ambar::ASTNode>($3), 
            std::unique_ptr<ambar::ASTNode>($5)
        );
        $$ = whileNode.release();
    }
;

for_stmt: 
    FOR LPAREN assign_stmt expr SEMI assign_stmt RPAREN stmt {
        auto forNode = std::make_unique<ambar::ForNode>(
            std::unique_ptr<ambar::ASTNode>($3),
            std::unique_ptr<ambar::ASTNode>($4),
            std::unique_ptr<ambar::ASTNode>($6),
            std::unique_ptr<ambar::BlockNode>(dynamic_cast<ambar::BlockNode*>($8))
        );
        $$ = forNode.release();
    }
;

break_stmt: 
    BREAK SEMI { 
        auto breakNode = std::make_unique<ambar::BreakNode>();
        $$ = breakNode.release();
    }
;

continue_stmt: 
    CONTINUE SEMI { 
        auto continueNode = std::make_unique<ambar::ContinueNode>();
        $$ = continueNode.release();
    }
;

block:
    LBRACE RBRACE {
        auto blk = std::make_unique<ambar::BlockNode>();
        $$ = blk.release();
    }
    | LBRACE stmt_list RBRACE {
        auto blk = std::make_unique<ambar::BlockNode>();
        if ($2) {
            for (ambar::ASTNode* stmt : *$2) {
                if (stmt) {
                    blk->add(std::unique_ptr<ambar::ASTNode>(stmt));
                }
            }
            delete $2;
        }
        $$ = blk.release();
    }
;

stmt_list:
      stmt {
        $$ = new std::vector<ambar::ASTNode*>();
        if ($1) $$->push_back($1);
      }
    | stmt_list stmt {
        if ($2) $1->push_back($2);
        $$ = $1;
      }
;

opt_args: 
      /* empty */ { $$ = new std::vector<ambar::ASTNode*>(); }
    | args { $$ = $1; }
;

args: 
      expr { 
          $$ = new std::vector<ambar::ASTNode*>(); 
          if ($1) $$->push_back($1); 
      }
    | args COMMA expr { 
          if ($3) $1->push_back($3); 
          $$ = $1; 
      }
;

expr: 
    logic_expr { $$ = $1; }
;

logic_expr: 
      rel_expr { $$ = $1; }
    | logic_expr AND rel_expr { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "AND", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
    | logic_expr OR rel_expr { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "OR", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
;

rel_expr: 
      arith_expr { $$ = $1; }
    | arith_expr EQ arith_expr { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "==", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
    | arith_expr NEQ arith_expr { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "!=", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
    | arith_expr LT arith_expr { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "<", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
    | arith_expr LE arith_expr { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "<=", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
    | arith_expr GT arith_expr { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              ">", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
    | arith_expr GE arith_expr { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              ">=", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
;

arith_expr: 
      term { $$ = $1; }
    | arith_expr ADD term { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "+", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
    | arith_expr SUB term { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "-", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
;

term: 
      factor { $$ = $1; }
    | term MUL factor { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "*", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
    | term DIV factor { 
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "/", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
;

factor: 
      NUM_INT        { 
          auto numNode = std::make_unique<ambar::NumberNode>($1);
          $$ = numNode.release();
      }
    | NUM_REAL       { 
          auto numNode = std::make_unique<ambar::NumberNode>($1);
          $$ = numNode.release();
      }
    | IDENTIFIER     { 
          auto idNode = std::make_unique<ambar::IdentifierNode>($1);
          free($1);
          $$ = idNode.release();
      }
    | STRING         { 
          auto strNode = std::make_unique<ambar::StringNode>(*$1);
          delete $1;
          $$ = strNode.release();
      }
    | BOOL_TRUE      { 
          auto boolNode = std::make_unique<ambar::BoolNode>(true);
          $$ = boolNode.release();
      }
    | BOOL_FALSE     { 
          auto boolNode = std::make_unique<ambar::BoolNode>(false);
          $$ = boolNode.release();
      }
    | func_call      { $$ = $1; }
    | LPAREN expr RPAREN { $$ = $2; }
;

func_call: 
    IDENTIFIER LPAREN opt_args RPAREN { 
        std::vector<std::unique_ptr<ambar::ASTNode>> args;
        if ($3) {
            for (auto* arg : *$3) {
                if (arg) {
                    args.push_back(std::unique_ptr<ambar::ASTNode>(arg));
                }
            }
            delete $3;
        }
        auto callNode = std::make_unique<ambar::CallNode>(std::string($1), std::move(args));
        free($1);
        $$ = callNode.release();
    }
;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, s);
}

void cleanup() {
    // A limpeza é feita automaticamente pelos unique_ptr
}
