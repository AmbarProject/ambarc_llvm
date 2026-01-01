#!/bin/bash

# A flag -f do rm ignora arquivos inexistentes e não gera erros
rm -f *.o

cd src/parser
rm -f parser.tab.*
cd ../..

cd src/lexer
rm -f lex.yy.c
cd ../..
