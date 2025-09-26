#!/bin/bash

# Configurações padrão
OPT_LEVEL="O0" # Sem otimização por padrão
SHOW_IR=false
KEEP_IR=false
KEEP_OBJ=false

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

echo "📂 Diretório: $(pwd)"
echo "🔨 Arquivo: $filename.amb"
echo "⚡ Otimização: -O$OPT_LEVEL"

# Verificar objetos necessários para o compilador
if [[ ! -f "main.o" || ! -f "lex.yy.o" || ! -f "parser.tab.o" || ! -f "LLVMGenerator.o" ]]; then
  echo "🔧 Compilando compilador..."
  ./rm.sh
  ./ir.sh
fi

# Executar o compilador com nível de otimização
echo "🚀 Gerando código IR..."
../ambar "-O$OPT_LEVEL" "$filename.amb"

# Verificar se o arquivo .ll foi gerado
if [ ! -f "$filename.ll" ]; then
  echo "❌ Erro: Arquivo '$filename.ll' não foi gerado"
  exit 1
fi

# Mostrar IR se solicitado
if [ "$SHOW_IR" = true ]; then
  echo ""
  echo "📄 CÓDIGO IR GERADO:"
  echo "=========================================="
  cat "$filename.ll"
  echo "=========================================="
  echo ""
fi

# Compilar IR para objeto
echo "🔨 Compilando IR para objeto..."
llc "-O$OPT_LEVEL" -mtriple=x86_64-unknown-linux-gnu -filetype=obj "$filename.ll" -o "$filename.o"

# Linkar
echo "🔗 Linkando executável..."
gcc -no-pie "$filename.o" -o "$filename"

# Tornar executável
chmod +x "$filename"

echo "✅ Executável gerado: $filename"
echo "🚀 Executando programa..."
echo "=========================================="

# Executar o programa
./"$filename"
EXIT_CODE=$?

echo "=========================================="
echo "📊 Programa finalizado com código: $EXIT_CODE"

# Limpeza de arquivos temporários
echo "🧹 Limpando arquivos temporários..."
if [ "$KEEP_IR" = false ]; then
  rm -f "$filename.ll"
  echo "✅ Removido: $filename.ll"
else
  echo "💾 Mantido: $filename.ll"
fi

if [ "$KEEP_OBJ" = false ]; then
  rm -f "$filename.o"
  echo "✅ Removido: $filename.o"
else
  echo "💾 Mantido: $filename.o"
fi

exit $EXIT_CODE
