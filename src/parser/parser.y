%language "c++"
%code requires {
    #include "ast/nodes/ASTNode.hpp"
    #include "ast/nodes/expressions/BinaryNode.hpp"
    #include "ast/nodes/expressions/BoolNode.hpp"
    #include "ast/nodes/expressions/CallNode.hpp"
    #include "ast/nodes/expressions/IdentifierNode.hpp"
    #include "ast/nodes/expressions/NumberNode.hpp"
    #include "ast/nodes/expressions/UnaryNode.hpp"

    #include "ast/nodes/statements/AssignNode.hpp"
    #include "ast/nodes/statements/BlockNode.hpp"
    #include "ast/nodes/statements/IfNode.hpp"
    #include "ast/nodes/statements/ReturnNode.hpp"
    #include "ast/nodes/statements/WhileNode.hpp"

    #include "ast/nodes/declarations/FunctionNode.hpp"
    #include "ast/nodes/declarations/ProgramNode.hpp"
    #include "ast/nodes/declarations/VarNode.hpp"

    #include <memory>
    #include <vector>
    #include <string>
    
    namespace ambar {
        extern std::unique_ptr<ASTNode> astRoot;
    }
}

%code top {
    #include "ast/nodes/ASTNode.hpp"
    
    extern "C" {
        int yylex();
        void yyerror(const char *s);
        extern FILE *yyin;
        extern int yylineno;
    }
    
    using namespace ambar;
}

%define api.value.type {std::unique_ptr<ambar::ASTNode>}
%define parse.error verbose

%token <std::string*> IDENTIFIER
%token <long> NUM_INT
%token <double> NUM_REAL
%token BOOL_TRUE BOOL_FALSE

%token FUNC RETURN IF ELSE WHILE
%token INT FLOAT BOOL VOID
%token AND OR NOT EQ NEQ LT LE GT GE
%token ADD SUB MUL DIV MOD ASSIGN
%token SEMI COLON COMMA LPAREN RPAREN LBRACE RBRACE

%right ASSIGN            
%left OR                 
%left AND               
%left EQ NEQ                
%left LT GT LE GE           
%left ADD SUB               
%left MUL DIV MOD           
%right UNARY NOT

%type <std::unique_ptr<ASTNode>*> program decl_list decl var_decl func_decl
%type <std::unique_ptr<ASTNode>*> stmt assign_stmt return_stmt
%type <std::unique_ptr<ASTNode>*> if_stmt while_stmt block
%type <std::unique_ptr<ASTNode>*> expr logic_expr
%type <std::unique_ptr<ASTNode>*> rel_expr arith_expr term factor unary_expr
%type <std::string*> type
%type <std::vector<std::unique_ptr<ASTNode>>*> stmt_list
%type <std::vector<std::pair<std::string, std::string>>*> params opt_params

%start program

%%

program:
    decl_list {
        ambar::astRoot = std::move(*$1);
        delete $1;
    }
;

decl_list:
    decl {
        $$ = new std::unique_ptr<ASTNode>(std::make_unique<ProgramNode>());
        dynamic_cast<ProgramNode*>($$->get())->add(std::move(*$1));
        delete $1;
    }
    | decl_list decl {
        ProgramNode* prog = dynamic_cast<ProgramNode*>($1->get());
        if (prog && $2) {
            prog->add(std::move(*$2));
            delete $2;
        }
        $$ = $1;
    }
;

decl:
    var_decl        { $$ = $1; }
    | func_decl     { $$ = $1; }
    | stmt          { $$ = $1; }
;

var_decl:
    IDENTIFIER COLON type ASSIGN expr SEMI {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<VarNode>(*$1, *$3, std::move(*$5)));
        delete $1;
        delete $3;
        delete $5;
    }
    | IDENTIFIER COLON type SEMI {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<VarNode>(*$1, *$3, nullptr));
        delete $1;
        delete $3;
    }
;

func_decl:
    FUNC IDENTIFIER LPAREN opt_params RPAREN COLON type block {
        auto blockPtr = std::unique_ptr<BlockNode>(dynamic_cast<BlockNode*>($8->release()));
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<FunctionNode>(*$2, *$7, std::move(*$4), std::move(blockPtr)));
        delete $2;
        delete $4;
        delete $7;
    }
;

opt_params:
    %empty {
        $$ = new std::vector<std::pair<std::string, std::string>>();
    }
    | params {
        $$ = $1;
    }
;

params:
    IDENTIFIER COLON type {
        $$ = new std::vector<std::pair<std::string, std::string>>{ { *$1, *$3 } };
        delete $1;
        delete $3;
    }
    | params COMMA IDENTIFIER COLON type {
        $1->emplace_back(*$3, *$5);
        delete $3;
        delete $5;
        $$ = $1;
    }
;

type:
    INT       { $$ = new std::string("int"); }
    | FLOAT   { $$ = new std::string("float"); }
    | BOOL    { $$ = new std::string("bool"); }
    | VOID    { $$ = new std::string("void"); }
;

stmt:
    var_decl         { $$ = $1; }
    | assign_stmt    { $$ = $1; }
    | return_stmt    { $$ = $1; }
    | if_stmt        { $$ = $1; }
    | while_stmt     { $$ = $1; }
    | block          { $$ = $1; }
;

assign_stmt:
    IDENTIFIER ASSIGN expr SEMI {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<AssignNode>(*$1, std::move(*$3)));
        delete $1;
        delete $3;
    }
;

return_stmt:
    RETURN expr SEMI {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<ReturnNode>(std::move(*$2)));
        delete $2;
    }
    | RETURN SEMI {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<ReturnNode>(nullptr));
    }
;

if_stmt:
    IF LPAREN expr RPAREN block {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<IfNode>(std::move(*$3), std::move(*$5)));
        delete $3;
        delete $5;
    }
    | IF LPAREN expr RPAREN block ELSE block {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<IfNode>(std::move(*$3), std::move(*$5), std::move(*$7)));
        delete $3;
        delete $5;
        delete $7;
    }
;
while_stmt:
    WHILE LPAREN expr RPAREN block {
        auto blockPtr = std::unique_ptr<BlockNode>(dynamic_cast<BlockNode*>($5->release()));
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<WhileNode>(std::move(*$3), std::move(blockPtr)));
        delete $3;
        delete $5;
    }
;

block:
    LBRACE RBRACE {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<BlockNode>());
    }
    | LBRACE stmt_list RBRACE {
        auto blk = std::make_unique<BlockNode>();
        for (auto& stmt : *$2) {
            if (stmt) {
                blk->add(std::move(stmt));
            }
        }
        $$ = new std::unique_ptr<ASTNode>(std::move(blk));
        delete $2;
    }
;

stmt_list:
    stmt {
        $$ = new std::vector<std::unique_ptr<ASTNode>>();
        if ($1) {
            $$->push_back(std::move(*$1));
            delete $1;
        }
    }
    | stmt_list stmt {
        if ($2) {
            $1->push_back(std::move(*$2));
            delete $2;
        }
        $$ = $1;
    }
;

expr:
    logic_expr {
        $$ = $1;
    }
;

logic_expr:
    rel_expr {
        $$ = $1;
    }
    | logic_expr AND rel_expr {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<BinaryNode>("&&", std::move(*$1), std::move(*$3)));
        delete $1;
        delete $3;
    }
    | logic_expr OR rel_expr {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<BinaryNode>("||", std::move(*$1), std::move(*$3)));
        delete $1;
        delete $3;
    }
    | NOT rel_expr {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<UnaryNode>("!", std::move(*$2)));
        delete $2;
    }
;

rel_expr:
    arith_expr {
        $$ = $1;
    }
    | arith_expr EQ arith_expr {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<BinaryNode>("==", std::move(*$1), std::move(*$3)));
        delete $1;
        delete $3;
    }
    | arith_expr NEQ arith_expr {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<BinaryNode>("!=", std::move(*$1), std::move(*$3)));
        delete $1;
        delete $3;
    }
    | arith_expr LT arith_expr {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<BinaryNode>("<", std::move(*$1), std::move(*$3)));
        delete $1;
        delete $3;
    }
    | arith_expr LE arith_expr {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<BinaryNode>("<=", std::move(*$1), std::move(*$3)));
        delete $1;
        delete $3;
    }
    | arith_expr GT arith_expr {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<BinaryNode>(">", std::move(*$1), std::move(*$3)));
        delete $1;
        delete $3;
    }
    | arith_expr GE arith_expr {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<BinaryNode>(">=", std::move(*$1), std::move(*$3)));
        delete $1;
        delete $3;
    }
;

arith_expr:
    term {
        $$ = $1;
    }
    | arith_expr ADD term {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<BinaryNode>("+", std::move(*$1), std::move(*$3)));
        delete $1;
        delete $3;
    }
    | arith_expr SUB term {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<BinaryNode>("-", std::move(*$1), std::move(*$3)));
        delete $1;
        delete $3;
    }
;

term:
    factor {
        $$ = $1;
    }
    | term MUL factor {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<BinaryNode>("*", std::move(*$1), std::move(*$3)));
        delete $1;
        delete $3;
    }
    | term DIV factor {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<BinaryNode>("/", std::move(*$1), std::move(*$3)));
        delete $1;
        delete $3;
    }
    | term MOD factor {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<BinaryNode>("%", std::move(*$1), std::move(*$3)));
        delete $1;
        delete $3;
    }
;

factor:
    NUM_INT {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<NumberNode>($1));
    }
    | NUM_REAL {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<NumberNode>($1));
    }
    | IDENTIFIER {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<IdentifierNode>(*$1));
        delete $1;
    }
    | BOOL_TRUE {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<BoolNode>(true));
    }
    | BOOL_FALSE {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<BoolNode>(false));
    }
    | LPAREN expr RPAREN {
        $$ = $2;
    }
    | ADD factor %prec UNARY {
        $$ = $2;
    }
    | SUB factor %prec UNARY {
        $$ = new std::unique_ptr<ASTNode>(
            std::make_unique<UnaryNode>("-", std::move(*$2)));
        delete $2;
    }
;

%%