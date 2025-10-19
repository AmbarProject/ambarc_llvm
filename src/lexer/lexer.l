%option noyywrap

%{
#include "../parser/parser.tab.hh"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>

extern "C" {
    int yylex();
}


void yyerror(const char *s);
%}

DIGIT      [0-9]
LETTER     [a-zA-Z_]
NUM_INT    [0-9]+
NUM_REAL   [0-9]+\.[0-9]+
ID         [a-zA-Z_][a-zA-Z0-9_]*
WS         [ \t\r\n]+

%%

"import"        { return IMPORT; }
"func"          { return FUNC; }
"return"        { return RETURN; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"for"           { return FOR; }
"break"         { return BREAK; }
"continue"      { return CONTINUE; }

"int"           { return INT; }
"float"         { return FLOAT; }
"bool"          { return BOOL; }
"string"        { return STRING_T; }
"void"          { return VOID; }

"true"          { return BOOL_TRUE; }
"false"         { return BOOL_FALSE; }

"&&"            { return AND; }
"||"            { return OR; }
"=="            { return EQ; }
"!="            { return NEQ; }
"<="            { return LE; }
">="            { return GE; }
"<"             { return LT; }
">"             { return GT; }
"+"             { return ADD; }
"-"             { return SUB; }
"*"             { return MUL; }
"/"             { return DIV; }
"="             { return ASSIGN; }
"->"            { return ARROW; }

";"             { return SEMI; }
":"             { return COLON; }
","             { return COMMA; }
"."             { return DOT; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
"["          { return LBRACKET; }
"]"          { return RBRACKET; }

\"([^\"\\]|\\.)*\"  {
    std::string raw(yytext);
    std::string cleaned = raw.substr(1, raw.size() - 2);
    yylval.str = new std::string(cleaned);
    return STRING;
}

{NUM_REAL}       { yylval.real = atof(yytext); return NUM_REAL; }
{NUM_INT}        { yylval.num = atoi(yytext); return NUM_INT; }
{ID}             { yylval.id = strdup(yytext); return IDENTIFIER; }

"//".*           { }
"/*"([^*]|\*+[^*/])*"*/" { }
{WS}             { }

.                { std::cerr << "Unexpected character: " << yytext << std::endl; }

%%

