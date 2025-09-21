#!/bin/bash

# Verificar se foi passado um argumento
if [ $# -eq 0 ]; then
  echo "Erro: Por favor, informe o nome do arquivo .amb"
  echo "Uso: $0 <arquivo.amb>"
  exit 1
fi

# Verificar se o arquivo existe
if [ ! -f "$1" ]; then
  echo "Erro: Arquivo '$1' não encontrado"
  exit 1
fi

# Verificar se é um arquivo .amb
if [[ "$1" != *.amb ]]; then
  echo "Aviso: O arquivo '$1' não tem extensão .amb"
fi

# Extrair o nome do arquivo sem extensão
filename=$(basename "$1" .amb)

echo "🔨 Processando arquivo: $1"

if [[ ! -f "main.o" || ! -f "lex.yy.o" || ! -f "parser.tab.o" || ! -f "LLVMGenerator.o" ]]; then
  ./rm.sh
  ./ir.sh
fi

# Executar o compilador com o arquivo fornecido
./ambar "$1"

# Verificar se o arquivo .ll foi gerado
if [ ! -f "$filename.ll" ]; then
  echo "Erro: Arquivo '$filename.ll' não foi gerado"
  exit 1
fi

# Compilar IR para objeto com flags PIE
llc -mtriple=x86_64-unknown-linux-gnu -filetype=obj "$filename.ll" -o output.o

# Linkar com flags PIE
gcc -no-pie output.o -o output

# Tornar executável e executar
chmod +x output
echo "🚀 Executando programa..."
./output

# Limpar arquivos temporários (opcional)
# rm -f output.o "$filename.ll"
