%{
#include "../parser/parser.tab.hh"
#include <iostream>
#include <cstdlib>
#include <cstring>

extern "C" {
    int yylex();
}

void yyerror(const char* s);
%}

DIGIT    [0-9]
LETTER   [a-zA-Z_]
ID       {LETTER}({LETTER}|{DIGIT})*
WS       [ \t\r\n]

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

";"             { return SEMI; }
":"             { return COLON; }
","             { return COMMA; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }

[0-9]+\.[0-9]+  { 
    yylval.real = atof(yytext);  // CORREÇÃO: usar 'real' em vez de 'real_val'
    return NUM_REAL; 
}

[0-9]+          { 
    yylval.num = atoi(yytext);   // CORREÇÃO: usar 'num' em vez de 'int_val'
    return NUM_INT; 
}

[a-zA-Z_][a-zA-Z0-9_]* { 
    yylval.id = strdup(yytext);  // CORREÇÃO: usar 'id' em vez de 'str_val'
    return IDENTIFIER; 
}

[ \t\n\r]       { /* skip whitespace */ }

.               { 
    std::cerr << "Unexpected character: " << yytext << std::endl;
    return YYUNDEF;
}

%%

int yywrap() { 
    return 1; 
}
