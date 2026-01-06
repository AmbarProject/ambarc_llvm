#!/bin/bash

# Diretórios base
SCRIPT_DIR=$(dirname "$(realpath "$0")")
ROOT_DIR="$SCRIPT_DIR/.."
SOURCE_DIR="$ROOT_DIR/src"
BIN_DIR="$SCRIPT_DIR/bin"

echo "🧹 Limpando arquivos temporários..."

# Remover arquivos gerados pelo bison/flex
echo "  Limpando parser..."
rm -f "$SOURCE_DIR/parser/parser.tab.cc" "$SOURCE_DIR/parser/parser.tab.hh"
rm -f "$SOURCE_DIR/parser/parser.output"

echo "  Limpando lexer..."
rm -f "$SOURCE_DIR/lexer/lex.yy.c"

echo "  Limpando objetos..."
rm -f "$BIN_DIR"/*.o

echo "  Limpando executáveis..."
rm -f "$BIN_DIR/ambar"
rm -f "$ROOT_DIR/ambar"

echo "✅ Limpeza concluída!"