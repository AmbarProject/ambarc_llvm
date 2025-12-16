#!/bin/bash
set -e

IMAGE="ambar:latest"

show_help() {
  echo "Uso: $0 [nível de otimização] [opções] arquivo.amb"
  echo ""
  echo "Nível de otimização (apenas um):"
  echo "  -O0          Sem otimizações (padrão)"
  echo "  -O1          Otimizações básicas"
  echo "  -O2          Otimizações padrão"
  echo "  -O3          Otimizações agressivas"
  echo "  -Os          Otimizado para tamanho"
  echo ""
  echo "Opções adicionais (podem ser combinadas):"
  echo "  --show-ir    Mostrar código IR gerado"
  echo "  --keep-ir    Manter arquivo .ll após execução"
  echo "  --keep-obj   Manter arquivo .o após execução"
  echo "  -h, --help   Mostrar esta ajuda"
  echo ""
  echo "Exemplos:"
  echo "  $0 programa.amb           # Sem otimizações"
  echo "  $0 -O2 programa.amb       # Com otimizações O2"
  echo "  $0 -O3 --show-ir programa.amb"
  exit 0
}

# Mostrar ajuda se solicitado
if [[ "$1" == "-h" || "$1" == "--help" ]]; then
  show_help
fi

# ---------- FORA DO DOCKER ----------
if [ ! -f "/.dockerenv" ]; then
  if ! command -v docker &>/dev/null; then
    echo "❌ Docker não está instalado."
    exit 1
  fi

  docker run --rm -it \
    -v "${PWD}:/workspace" \
    -w /workspace \
    "$IMAGE" \
    ./ambar.sh "$@"

  exit $?
fi

# ---------- DENTRO DO DOCKER ----------
OPT_LEVEL=""
OPTIONS=()
INPUT_FILE=""

# Processa argumentos
for arg in "$@"; do
  case "$arg" in
  -O0 | -O1 | -O2 | -O3 | -Os)
    if [ -n "$OPT_LEVEL" ]; then
      echo "❌ Apenas um nível de otimização pode ser especificado."
      exit 1
    fi
    OPT_LEVEL="$arg"
    ;;
  --show-ir | --keep-ir | --keep-obj)
    OPTIONS+=("$arg")
    ;;
  *.amb)
    if [ -n "$INPUT_FILE" ]; then
      echo "❌ Apenas um arquivo de entrada pode ser especificado."
      exit 1
    fi
    INPUT_FILE="$arg"
    ;;
  *)
    echo "❌ Argumento inválido ou arquivo sem extensão .amb: $arg"
    show_help
    ;;
  esac
done

# ---------- Validações finais ----------
if [ -z "$INPUT_FILE" ]; then
  echo "❌ Nenhum arquivo .amb fornecido."
  show_help
fi

# Reforça que o arquivo termina em .amb
if [[ ! "$INPUT_FILE" =~ \.amb$ ]]; then
  echo "❌ Erro: O arquivo '$INPUT_FILE' não tem extensão .amb"
  exit 1
fi

# Define nível de otimização padrão
[ -z "$OPT_LEVEL" ] && OPT_LEVEL="-O0"

# ---------- Executa compile.sh ----------
./compile.sh "$OPT_LEVEL" "${OPTIONS[@]}" "$INPUT_FILE"
