%option noyywrap
%option reentrant bison-bridge
%option yylineno
%option nounput
%option nodefault
%option bison-locations

%{
#include "../parser/parser.output"
#include <string>
#include <cstdlib>
#include "../utils/ErrorHandler.hpp"

// Função auxiliar para registro de localização
void register_token_location(YYLTYPE* loc, int first_line, int first_col, int last_line, int last_col) {
    loc->first_line = first_line;
    loc->first_column = first_col;
    loc->last_line = last_line;
    loc->last_column = last_col;
}

// Protótipo da função de análise léxica
int yylex(YYSTYPE* yylval, YYLTYPE* yylloc, yyscan_t scanner);
%}

DIGITO      [0-9]
LETRA       [a-zA-Z_]
NUM_INT     {DIGITO}+
NUM_REAL    {DIGITO}+"."{DIGITO}+
ID          {LETRA}({LETRA}|{DIGITO})*
WS          [ \t\r\n]

%x COMMENT

%%

<INITIAL>{
    "import"        { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return IMPORT; }
    "func"          { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return FUNC; }
    "return"        { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return RETURN; }
    "if"            { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return IF; }
    "else"          { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return ELSE; }
    "while"         { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return WHILE; }
    "for"           { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return FOR; }
    "break"         { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return BREAK; }
    "continue"      { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return CONTINUE; }

    "int"           { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return INT; }
    "float"         { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return FLOAT; }
    "bool"          { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return BOOL; }
    "string"        { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return STRING_T; }
    "void"          { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return VOID; }

    "true"          { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return BOOL_TRUE; }
    "false"         { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return BOOL_FALSE; }

    "&&"            { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return AND; }
    "||"            { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return OR; }
    "=="            { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return EQ; }
    "!="            { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return NEQ; }
    "<="            { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return LE; }
    ">="            { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return GE; }
    "<"             { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return LT; }
    ">"             { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return GT; }
    "+"             { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return ADD; }
    "-"             { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return SUB; }
    "*"             { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return MUL; }
    "/"             { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return DIV; }
    "="             { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return ASSIGN; }
    "->"            { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return ARROW; }

    ";"             { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return SEMI; }
    ":"             { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return COLON; }
    ","             { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return COMMA; }
    "."             { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return DOT; }
    "("             { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return LPAREN; }
    ")"             { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return RPAREN; }
    "{"             { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return LBRACE; }
    "}"             { register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1); return RBRACE; }

    \"              {
        register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn);
        std::string str;
        int c;
        int start_line = yylineno;
        int start_col = yycolumn;
        
        while ((c = yyinput(scanner)) != '"') {
            if (c == '\\') {
                int next = yyinput(scanner);
                switch(next) {
                    case 'n': str += '\n'; break;
                    case 't': str += '\t'; break;
                    case 'r': str += '\r'; break;
                    case '\\': str += '\\'; break;
                    case '"': str += '"'; break;
                    default: str += static_cast<char>(next); break;
                }
            } else if (c == EOF) {
                ErrorHandler::report(
                    ErrorHandler::Error,
                    "Unterminated string literal",
                    start_line,
                    start_col
                );
                yyterminate();
            } else {
                str += static_cast<char>(c);
            }
        }
        yylval->str_val = new std::string(str);
        register_token_location(yylloc, start_line, start_col, yylineno, yycolumn);
        return STRING;
    }

    {NUM_REAL}      {
        register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1);
        yylval->real_val = strtod(yytext, nullptr);
        return NUM_REAL;
    }

    {NUM_INT}       {
        register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1);
        yylval->int_val = strtol(yytext, nullptr, 10);
        return NUM_INT;
    }

    {ID}            {
        register_token_location(yylloc, yylineno, yycolumn, yylineno, yycolumn+yyleng-1);
        yylval->id_val = strdup(yytext);
        return IDENTIFIER;
    }

    "//".*          { /* Ignorar comentários de linha */ }
    "/*"            { BEGIN(COMMENT); }
    
    {WS}            { /* Ignorar espaços em branco */ }

    .               {
        ErrorHandler::report(
            ErrorHandler::Error,
            std::string("Unexpected character: '") + yytext + "'",
            yylineno,
            yycolumn
        );
    }
}

<COMMENT>{
    "*/"            { BEGIN(INITIAL); }
    <<EOF>>         {
        ErrorHandler::report(
            ErrorHandler::Error,
            "Unterminated comment",
            yylineno,
            yycolumn
        );
        yyterminate();
    }
    .|\n            { /* Ignorar conteúdo do comentário */ }
}

%%

int yywrap(yyscan_t scanner) {
    return 1;
}