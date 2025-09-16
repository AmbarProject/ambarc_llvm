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

%right ASSIGN
%left OR
%left AND
%left EQ NEQ
%left LT LE GT GE
%left ADD SUB
%left MUL DIV
%right UMINUS


%start program

%%

program:
    decl_list {
        printf("DEBUG PARSER: Reducing program\n");
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
          printf("DEBUG PARSER: Reducing decl_list (single decl)\n");
          auto p = std::make_unique<ambar::ProgramNode>();
          if ($1) {
              p->addDeclaration(std::unique_ptr<ambar::ASTNode>($1));
          }
          $$ = p.release();
      }
    | decl_list decl {
          printf("DEBUG PARSER: Reducing decl_list (multiple decl)\n");
          ambar::ProgramNode* p = dynamic_cast<ambar::ProgramNode*>($1);
          if (p && $2) {
              p->addDeclaration(std::unique_ptr<ambar::ASTNode>($2));
          }
          $$ = p;
      }
;

decl: 
      import_decl         { printf("DEBUG PARSER: Reducing decl (import_decl)\n"); $$ = nullptr; }
    | var_decl            { printf("DEBUG PARSER: Reducing decl (var_decl)\n"); $$ = $1; }
    | func_decl           { printf("DEBUG PARSER: Reducing decl (func_decl)\n"); $$ = $1; }
    | stmt                { printf("DEBUG PARSER: Reducing decl (stmt)\n"); $$ = $1; }
;

import_decl: 
    IMPORT IDENTIFIER ARROW IDENTIFIER SEMI { printf("DEBUG PARSER: Reducing import_decl\n"); $$ = nullptr; }
;

var_decl:
      IDENTIFIER COLON type ASSIGN expr SEMI { 
          printf("DEBUG PARSER: Reducing var_decl (with init)\n");
          auto var = std::make_unique<ambar::VarNode>(std::string($1), *$3, std::unique_ptr<ambar::ASTNode>($5));
          delete $3;
          free($1);
          $$ = var.release();
      }
    | IDENTIFIER COLON type SEMI { 
          printf("DEBUG PARSER: Reducing var_decl (without init)\n");
          auto var = std::make_unique<ambar::VarNode>(std::string($1), *$3, nullptr);
          delete $3;
          free($1);
          $$ = var.release();
      }
;

func_decl: 
    FUNC IDENTIFIER LPAREN opt_params RPAREN ARROW type block {
        printf("DEBUG PARSER: Reducing func_decl\n");
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
      /* empty */ { printf("DEBUG PARSER: Reducing opt_params (empty)\n"); $$ = nullptr; }  
    | params { printf("DEBUG PARSER: Reducing opt_params (params)\n"); $$ = $1; } 
;

params: 
      IDENTIFIER COLON type {
          printf("DEBUG PARSER: Reducing params (single)\n");
          $$ = new std::vector<std::pair<std::string, std::string>>();
          $$->push_back({std::string($1), *$3});
          delete $3; 
          free($1);
      }
    | params COMMA IDENTIFIER COLON type {
          printf("DEBUG PARSER: Reducing params (multiple)\n");
          $1->push_back({std::string($3), *$5});
          delete $5; 
          free($3);
          $$ = $1;
      }
;

type:
    INT       { printf("DEBUG PARSER: Reducing type (int)\n"); $$ = new std::string("int"); }
  | FLOAT     { printf("DEBUG PARSER: Reducing type (float)\n"); $$ = new std::string("float"); }
  | BOOL      { printf("DEBUG PARSER: Reducing type (bool)\n"); $$ = new std::string("bool"); }
  | STRING_T  { printf("DEBUG PARSER: Reducing type (string)\n"); $$ = new std::string("string"); }
  | VOID      { printf("DEBUG PARSER: Reducing type (void)\n"); $$ = new std::string("void"); }
;

stmt: 
      assign_stmt      { printf("DEBUG PARSER: Reducing stmt (assign_stmt)\n"); $$ = $1; }
    | call_stmt        { printf("DEBUG PARSER: Reducing stmt (call_stmt)\n"); $$ = $1; }
    | return_stmt      { printf("DEBUG PARSER: Reducing stmt (return_stmt)\n"); $$ = $1; }
    | if_stmt          { printf("DEBUG PARSER: Reducing stmt (if_stmt)\n"); $$ = $1; }
    | while_stmt       { printf("DEBUG PARSER: Reducing stmt (while_stmt)\n"); $$ = $1; }
    | for_stmt         { printf("DEBUG PARSER: Reducing stmt (for_stmt)\n"); $$ = $1; }
    | break_stmt       { printf("DEBUG PARSER: Reducing stmt (break_stmt)\n"); $$ = $1; }
    | continue_stmt    { printf("DEBUG PARSER: Reducing stmt (continue_stmt)\n"); $$ = $1; }
    | block            { printf("DEBUG PARSER: Reducing stmt (block)\n"); $$ = $1; }
;

assign_stmt: 
    IDENTIFIER ASSIGN expr SEMI { 
        printf("DEBUG PARSER: Reducing assign_stmt\n");
        auto assign = std::make_unique<ambar::AssignNode>($1, std::unique_ptr<ambar::ASTNode>($3));
        free($1);
        $$ = assign.release();
    }
;

call_stmt: 
    func_call SEMI { printf("DEBUG PARSER: Reducing call_stmt\n"); $$ = $1; }
;

return_stmt: 
      RETURN expr SEMI { 
          printf("DEBUG PARSER: Reducing return_stmt (with expr)\n");
          auto ret = std::make_unique<ambar::ReturnNode>(std::unique_ptr<ambar::ASTNode>($2));
          $$ = ret.release();
      }
    | RETURN SEMI { 
          printf("DEBUG PARSER: Reducing return_stmt (void)\n");
          auto ret = std::make_unique<ambar::ReturnNode>();
          $$ = ret.release();
      }
;

if_stmt: 
      IF LPAREN expr RPAREN stmt {
          printf("DEBUG PARSER: Reducing if_stmt (no else)\n");
          auto ifNode = std::make_unique<ambar::IfNode>(
              std::unique_ptr<ambar::ASTNode>($3), 
              std::unique_ptr<ambar::ASTNode>($5)
          );
          $$ = ifNode.release();
      }
    | IF LPAREN expr RPAREN stmt ELSE stmt {
          printf("DEBUG PARSER: Reducing if_stmt (with else)\n");
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
        printf("DEBUG PARSER: Reducing while_stmt\n");
        auto whileNode = std::make_unique<ambar::WhileNode>(
            std::unique_ptr<ambar::ASTNode>($3), 
            std::unique_ptr<ambar::ASTNode>($5)
        );
        $$ = whileNode.release();
    }
;

for_stmt: 
    FOR LPAREN assign_stmt expr SEMI assign_stmt RPAREN stmt {
        printf("DEBUG PARSER: Reducing for_stmt\n");
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
        printf("DEBUG PARSER: Reducing break_stmt\n");
        auto breakNode = std::make_unique<ambar::BreakNode>();
        $$ = breakNode.release();
    }
;

continue_stmt: 
    CONTINUE SEMI { 
        printf("DEBUG PARSER: Reducing continue_stmt\n");
        auto continueNode = std::make_unique<ambar::ContinueNode>();
        $$ = continueNode.release();
    }
;

block:
    LBRACE RBRACE {
        printf("DEBUG PARSER: Reducing block (empty)\n");
        auto blk = std::make_unique<ambar::BlockNode>();
        $$ = blk.release();
    }
    | LBRACE stmt_list RBRACE {
        printf("DEBUG PARSER: Reducing block (with stmts)\n");
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
        printf("DEBUG PARSER: Reducing stmt_list (single)\n");
        $$ = new std::vector<ambar::ASTNode*>();
        if ($1) $$->push_back($1);
      }
    | stmt_list stmt {
        printf("DEBUG PARSER: Reducing stmt_list (multiple)\n");
        if ($2) $1->push_back($2);
        $$ = $1;
      }
;

opt_args: 
      /* empty */ { printf("DEBUG PARSER: Reducing opt_args (empty)\n"); $$ = new std::vector<ambar::ASTNode*>(); }
    | args { printf("DEBUG PARSER: Reducing opt_args (args)\n"); $$ = $1; }
;

args: 
      expr { 
          printf("DEBUG PARSER: Reducing args (single)\n");
          $$ = new std::vector<ambar::ASTNode*>(); 
          if ($1) $$->push_back($1); 
      }
    | args COMMA expr { 
          printf("DEBUG PARSER: Reducing args (multiple)\n");
          if ($3) $1->push_back($3); 
          $$ = $1; 
      }
;

expr: 
    logic_expr { printf("DEBUG PARSER: Reducing expr\n"); $$ = $1; }
;

logic_expr: 
      rel_expr { printf("DEBUG PARSER: Reducing logic_expr (rel_expr)\n"); $$ = $1; }
    | logic_expr AND rel_expr { 
          printf("DEBUG PARSER: Reducing logic_expr (AND)\n");
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "AND", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
    | logic_expr OR rel_expr { 
          printf("DEBUG PARSER: Reducing logic_expr (OR)\n");
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "OR", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
;

rel_expr: 
      arith_expr { printf("DEBUG PARSER: Reducing rel_expr (arith_expr)\n"); $$ = $1; }
    | arith_expr EQ arith_expr { 
          printf("DEBUG PARSER: Reducing rel_expr (EQ)\n");
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "==", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
    | arith_expr NEQ arith_expr { 
          printf("DEBUG PARSER: Reducing rel_expr (NEQ)\n");
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "!=", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
    | arith_expr LT arith_expr { 
          printf("DEBUG PARSER: Reducing rel_expr (LT)\n");
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "<", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
    | arith_expr LE arith_expr { 
          printf("DEBUG PARSER: Reducing rel_expr (LE)\n");
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "<=", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
    | arith_expr GT arith_expr { 
          printf("DEBUG PARSER: Reducing rel_expr (GT)\n");
          auto binNode = std::make_unique<ambar::BinaryNode>(
              ">", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
    | arith_expr GE arith_expr { 
          printf("DEBUG PARSER: Reducing rel_expr (GE)\n");
          auto binNode = std::make_unique<ambar::BinaryNode>(
              ">=", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
;

arith_expr: 
      term { printf("DEBUG PARSER: Reducing arith_expr (term)\n"); $$ = $1; }
    | arith_expr ADD term { 
          printf("DEBUG PARSER: Reducing arith_expr (ADD)\n");
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "+", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
    | arith_expr SUB term { 
          printf("DEBUG PARSER: Reducing arith_expr (SUB)\n");
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "-", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
;

term: 
      factor { printf("DEBUG PARSER: Reducing term (factor)\n"); $$ = $1; }
    | term MUL factor { 
          printf("DEBUG PARSER: Reducing term (MUL)\n");
          auto binNode = std::make_unique<ambar::BinaryNode>(
              "*", 
              std::unique_ptr<ambar::ASTNode>($1), 
              std::unique_ptr<ambar::ASTNode>($3)
          );
          $$ = binNode.release();
      }
    | term DIV factor { 
          printf("DEBUG PARSER: Reducing term (DIV)\n");
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
          printf("DEBUG PARSER: Reducing factor (NUM_INT)\n");
          auto numNode = std::make_unique<ambar::NumberNode>($1);
          $$ = numNode.release();
      }
    | NUM_REAL       { 
          printf("DEBUG PARSER: Reducing factor (NUM_REAL)\n");
          auto numNode = std::make_unique<ambar::NumberNode>($1);
          $$ = numNode.release();
      }
    | IDENTIFIER     { 
          printf("DEBUG PARSER: Reducing factor (IDENTIFIER)\n");
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
          printf("DEBUG PARSER: Reducing factor (BOOL_TRUE)\n");
          auto boolNode = std::make_unique<ambar::BoolNode>(true);
          $$ = boolNode.release();
      }
    | BOOL_FALSE     { 
          printf("DEBUG PARSER: Reducing factor (BOOL_FALSE)\n");
          auto boolNode = std::make_unique<ambar::BoolNode>(false);
          $$ = boolNode.release();
      }
    | func_call      { printf("DEBUG PARSER: Reducing factor (func_call)\n"); $$ = $1; }
    | LPAREN expr RPAREN { printf("DEBUG PARSER: Reducing factor (parenthesized expr)\n"); $$ = $2; }
;

func_call: 
    IDENTIFIER LPAREN opt_args RPAREN { 
        printf("DEBUG PARSER: Reducing func_call\n");
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
