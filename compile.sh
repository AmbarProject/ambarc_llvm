#!/bin/bash

echo "Compilando Ambar Compiler..."

# Gerar parser
echo "Gerando parser..."
cd src/parser
bison -d -o parser.cpp parser.y
if [ $? -ne 0 ]; then
  echo "❌ Erro ao gerar parser"
  exit 1
fi
cd ../..

# Gerar lexer
echo "Gerando lexer..."
flex -o src/lexer/lex.yy.cpp src/lexer/lexer.l
if [ $? -ne 0 ]; then
  echo "❌ Erro ao gerar lexer"
  exit 1
fi

# Compilar
echo "Compilando..."
g++ -std=c++17 -Wall -Wextra -g -Isrc \
  -c src/parser/parser.cpp \
  -c src/lexer/lex.yy.cpp \
  -c src/LLVMGenerator.cpp \
  -c src/main.cpp \
  $(llvm-config --cxxflags)

if [ $? -ne 0 ]; then
  echo "❌ Erro na compilação"
  exit 1
fi

# Linkar
echo "Linkando..."
g++ -o ambar \
  parser.o lex.yy.o LLVMGenerator.o main.o \
  $(llvm-config --ldflags --system-libs --libs core)

if [ $? -eq 0 ]; then
  echo "✅ Compilação bem-sucedida!"
else
  echo "❌ Erro no linking"
  exit 1
fi
