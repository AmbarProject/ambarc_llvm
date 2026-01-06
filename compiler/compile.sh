#!/bin/bash

# Configurações padrão
OPT_LEVEL="O0" # Sem otimização por padrão
SHOW_IR=false
KEEP_IR=false
KEEP_OBJ=false

# Diretórios base
SCRIPT_DIR=$(dirname "$(realpath "$0")")
ROOT_DIR="$SCRIPT_DIR/.."
BIN_DIR="$SCRIPT_DIR/bin"
COMPILER_EXE="$BIN_DIR/ambarc"

# Função para mostrar ajuda
show_help() {
  echo "Uso: $0 [OPÇÕES] <arquivo.amb>"
  echo ""
  echo "Opções:"
  echo "  -O0          Sem otimizações (padrão)"
  echo "  -O1          Otimizações básicas"
  echo "  -O2          Otimizações padrão"
  echo "  -O3          Otimizações agressivas"
  echo "  -Os          Otimizado para tamanho"
  echo "  --show-ir    Mostrar código IR gerado"
  echo "  --keep-ir    Manter arquivo .ll após execução"
  echo "  --keep-obj   Manter arquivo .o após execução"
  echo "  -h, --help   Mostrar esta ajuda"
  echo ""
  echo "Exemplos:"
  echo "  $0 programa.amb           # Sem otimizações"
  echo "  $0 -O2 programa.amb       # Com otimizações O2"
  echo "  $0 -O3 --show-ir programa.amb"
}

# Parse argumentos
POSITIONAL_ARGS=()

while [[ $# -gt 0 ]]; do
  case $1 in
  -O0 | -O1 | -O2 | -O3 | -Os)
    OPT_LEVEL="${1#-O}" # Remove o -O
    shift
    ;;
  --show-ir)
    SHOW_IR=true
    shift
    ;;
  --keep-ir)
    KEEP_IR=true
    shift
    ;;
  --keep-obj)
    KEEP_OBJ=true
    shift
    ;;
  -h | --help)
    show_help
    exit 0
    ;;
  -*)
    echo "Erro: Opção desconhecida $1"
    show_help
    exit 1
    ;;
  *)
    POSITIONAL_ARGS+=("$1")
    shift
    ;;
  esac
done

# Restaurar positional arguments
set -- "${POSITIONAL_ARGS[@]}"

# Verificar se foi passado um argumento
if [ $# -eq 0 ]; then
  echo "Erro: Por favor, informe o nome do arquivo .amb"
  show_help
  exit 1
fi

# Verificar se o arquivo existe
INPUT_FILE="$1"
if [ ! -f "$INPUT_FILE" ]; then
  echo "❌ Erro: Arquivo '$INPUT_FILE' não encontrado"
  exit 1
fi

# Verificar se é um arquivo .amb
if [[ "$INPUT_FILE" != *.amb ]]; then
  echo "⚠️  Aviso: O arquivo '$INPUT_FILE' não tem extensão .amb"
fi

# Extrair diretório e nome do arquivo
src_dir=$(dirname "$(realpath "$INPUT_FILE")")
filename=$(basename "$INPUT_FILE" .amb)
original_dir=$(pwd)

echo "🔍 Processando: $INPUT_FILE"

# Verificar se o compilador existe
if [ ! -f "$COMPILER_EXE" ]; then
  echo "🔧 Compilador não encontrado, construindo..."
  "$SCRIPT_DIR/rm.sh"
  "$SCRIPT_DIR/ir.sh"
  
  if [ ! -f "$COMPILER_EXE" ]; then
    echo "❌ Erro: Falha ao construir compilador"
    exit 1
  fi
fi

# Executar o compilador com nível de otimização
echo "🏗️  Gerando código LLVM..."
"$COMPILER_EXE" "-$OPT_LEVEL" "$INPUT_FILE"

# Verificar se o arquivo .ll foi gerado no diretório correto
LL_FILE="$src_dir/$filename.ll"
if [ ! -f "$LL_FILE" ]; then
  echo "❌ Erro: Falha ao gerar código LLVM"
  exit 1
fi

# Mostrar IR se solicitado
if [ "$SHOW_IR" = true ]; then
  echo ""
  echo "📄 CÓDIGO IR GERADO:"
  echo "=========================================="
  cat "$LL_FILE"
  echo "=========================================="
  echo ""
fi

# Entrar no diretório do arquivo para compilar
cd "$src_dir" || {
  echo "❌ Erro: Não foi possível entrar no diretório '$src_dir'"
  exit 1
}

echo "🔨 Compilando IR para objeto..."
OBJ_FILE="$filename.o"
llc "-O$OPT_LEVEL" -mtriple=x86_64-unknown-linux-gnu -filetype=obj "$filename.ll" -o "$OBJ_FILE"

if [ ! -f "$OBJ_FILE" ]; then
  echo "❌ Erro: Falha ao compilar objeto"
  exit 1
fi

echo "🔗 Linkando..."
EXECUTABLE_FILE="$filename"
gcc -no-pie "$OBJ_FILE" -o "$EXECUTABLE_FILE"

if [ ! -f "$EXECUTABLE_FILE" ]; then
  echo "❌ Erro: Falha no linking"
  exit 1
fi

# Tornar executável
chmod +x "$EXECUTABLE_FILE"

echo "✅ Executável criado: $EXECUTABLE_FILE"

# Limpeza de arquivos temporários
if [ "$KEEP_IR" = false ]; then
  rm -f "$LL_FILE"
  echo "🗑️  Removido: $LL_FILE"
else
  echo "💾 Mantido: $LL_FILE"
fi

if [ "$KEEP_OBJ" = false ]; then
  rm -f "$OBJ_FILE"
  echo "🗑️  Removido: $OBJ_FILE"
else
  echo "💾 Mantido: $OBJ_FILE"
fi

# Voltar ao diretório original
cd "$original_dir" || true

echo ""
echo "🎉 Compilação concluída com sucesso!"
echo "   Executável: $src_dir/$EXECUTABLE_FILE"