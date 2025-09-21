#!/bin/bash

rm -f *.o

cd src/parser
rm -f parser.tab.*
cd ../..

cd src/lexer
rm -f lex.yy.c
cd ../..

rm -f output
