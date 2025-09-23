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

# Extrair diretório e nome do arquivo
src_dir=$(dirname "$1")
filename=$(basename "$1" .amb)

# Entrar na pasta do arquivo fonte
cd "$src_dir" || {
  echo "Erro: Não foi possível entrar no diretório '$src_dir'"
  exit 1
}

echo "📂 Diretório atual: $(pwd)"
echo "🔨 Processando arquivo: $filename.amb"

# Verificar objetos necessários para o compilador
if [[ ! -f "main.o" || ! -f "lex.yy.o" || ! -f "parser.tab.o" || ! -f "LLVMGenerator.o" ]]; then
  ../rm.sh
  ../ir.sh
fi

# Executar o compilador (espera que o binário 'ambar' esteja na pasta pai)
../ambar "$filename.amb"

# Verificar se o arquivo .ll foi gerado
if [ ! -f "$filename.ll" ]; then
  echo "Erro: Arquivo '$filename.ll' não foi gerado"
  exit 1
fi

# Compilar IR para objeto com flags PIE
llc -mtriple=x86_64-unknown-linux-gnu -filetype=obj "$filename.ll" -o "$filename.o"

# Linkar com flags PIE
gcc -no-pie "$filename.o" -o "$filename"

# Tornar executável e executar
chmod +x "$filename"
echo "🚀 Executando programa..."
./"$filename"
