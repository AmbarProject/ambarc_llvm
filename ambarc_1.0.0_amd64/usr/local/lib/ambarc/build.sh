#!/bin/bash
# Script de build avançado para AmbarC
# Permite controle mais fino sobre o processo de compilação

set -e

if [ $# -eq 0 ]; then
    echo "Uso: $0 [OPÇÕES] <arquivo.amb>"
    echo ""
    echo "Opções avançadas:"
    echo "  --only-ir        Apenas gera código IR (.ll)"
    echo "  --only-obj       Apenas gera objeto (.o)"
    echo "  --llc-opts OPTS  Opções extras para llc"
    echo "  --gcc-opts OPTS  Opções extras para gcc"
    echo "  --verbose        Mostrar todos os comandos"
    echo ""
    echo "Exemplos:"
    echo "  $0 --only-ir programa.amb"
    echo "  $0 --llc-opts '-O3 -march=native' programa.amb"
    exit 1
fi

# Variáveis
ONLY_IR=false
ONLY_OBJ=false
VERBOSE=false
LLC_OPTS=""
GCC_OPTS=""
INPUT_FILE=""
OUTPUT_FILE=""

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --only-ir)
            ONLY_IR=true
            shift
            ;;
        --only-obj)
            ONLY_OBJ=true
            shift
            ;;
        --llc-opts)
            LLC_OPTS="$2"
            shift 2
            ;;
        --gcc-opts)
            GCC_OPTS="$2"
            shift 2
            ;;
        --verbose|-v)
            VERBOSE=true
            shift
            ;;
        -o)
            OUTPUT_FILE="$2"
            shift 2
            ;;
        -*)
            echo "❌ Opção desconhecida: $1"
            exit 1
            ;;
        *)
            INPUT_FILE="$1"
            shift
            ;;
    esac
done

if [ -z "$INPUT_FILE" ]; then
    echo "❌ Nenhum arquivo de entrada especificado"
    exit 1
fi

if [ ! -f "$INPUT_FILE" ]; then
    echo "❌ Arquivo não encontrado: $INPUT_FILE"
    exit 1
fi

BASENAME=$(basename "$INPUT_FILE" .amb)
SRC_DIR=$(dirname "$(realpath "$INPUT_FILE")")

if [ -z "$OUTPUT_FILE" ]; then
    if [ "$ONLY_IR" = true ]; then
        OUTPUT_FILE="$SRC_DIR/$BASENAME.ll"
    elif [ "$ONLY_OBJ" = true ]; then
        OUTPUT_FILE="$SRC_DIR/$BASENAME.o"
    else
        OUTPUT_FILE="$SRC_DIR/$BASENAME"
    fi
fi

echo "🔧 Build avançado: $INPUT_FILE"

# Gerar IR
echo "1. Gerando LLVM IR..."
if [ "$VERBOSE" = true ]; then
    echo "   Comando: /usr/local/lib/ambarc/bin/ambarc-real -O2 \"$INPUT_FILE\""
fi

if ! /usr/local/lib/ambarc/bin/ambarc-real -O2 "$INPUT_FILE"; then
    echo "❌ Falha ao gerar IR"
    exit 1
fi

LL_FILE="$SRC_DIR/$BASENAME.ll"
if [ ! -f "$LL_FILE" ]; then
    echo "❌ Arquivo IR não gerado: $LL_FILE"
    exit 1
fi

if [ "$ONLY_IR" = true ]; then
    echo "✅ IR gerado: $LL_FILE"
    if [ "$LL_FILE" != "$OUTPUT_FILE" ]; then
        mv "$LL_FILE" "$OUTPUT_FILE"
        echo "✅ Renomeado para: $OUTPUT_FILE"
    fi
    exit 0
fi

# Compilar para objeto
echo "2. Compilando para objeto..."
LLC_CMD="llc -O2 -mtriple=x86_64-unknown-linux-gnu -filetype=obj $LLC_OPTS \"$LL_FILE\" -o \"$SRC_DIR/$BASENAME.o\""
if [ "$VERBOSE" = true ]; then
    echo "   Comando: $LLC_CMD"
fi

if ! eval "$LLC_CMD"; then
    echo "❌ Falha ao compilar objeto"
    exit 1
fi

if [ "$ONLY_OBJ" = true ]; then
    echo "✅ Objeto gerado: $SRC_DIR/$BASENAME.o"
    if [ "$SRC_DIR/$BASENAME.o" != "$OUTPUT_FILE" ]; then
        mv "$SRC_DIR/$BASENAME.o" "$OUTPUT_FILE"
        echo "✅ Renomeado para: $OUTPUT_FILE"
    fi
    rm -f "$LL_FILE"
    exit 0
fi

# Linkar
echo "3. Linkando executável..."
GCC_CMD="gcc -no-pie $GCC_OPTS \"$SRC_DIR/$BASENAME.o\" -o \"$OUTPUT_FILE\""
if [ "$VERBOSE" = true ]; then
    echo "   Comando: $GCC_CMD"
fi

if ! eval "$GCC_CMD"; then
    echo "❌ Falha no linking"
    exit 1
fi

chmod +x "$OUTPUT_FILE"

# Limpeza
rm -f "$LL_FILE" "$SRC_DIR/$BASENAME.o"

echo "✅ Executável criado: $OUTPUT_FILE"
echo "🎉 Build concluído!"
