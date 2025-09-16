#!/bin/bash

echo "Deletando arquivos"

rm *.o

cd src/parser
rm parser.tab.*
cd ../..

cd src/lexer
rm lex.yy.c
cd ..
