#!/bin/bash

# Verificar e instalar dependências do AmbarC
echo "🔍 Verificando dependências..."

DEPENDENCIES=(
    "build-essential"
    "llvm-14"
    "clang-14"
    "bison"
    "flex"
    "cmake"
    "git"
    "dpkg-dev"
    "debhelper"
)

MISSING_DEPS=()

# Verificar cada dependência
for dep in "${DEPENDENCIES[@]}"; do
    if ! dpkg -l | grep -q "^ii  $dep "; then
        MISSING_DEPS+=("$dep")
    fi
done

if [ ${#MISSING_DEPS[@]} -gt 0 ]; then
    echo "📦 Instalando dependências faltantes: ${MISSING_DEPS[*]}"
    
    # Atualizar repositórios
    sudo apt-get update
    
    # Instalar dependências
    sudo apt-get install -y "${MISSING_DEPS[@]}"
    
    if [ $? -eq 0 ]; then
        echo "✅ Dependências instaladas com sucesso!"
    else
        echo "❌ Falha ao instalar dependências"
        exit 1
    fi
else
    echo "✅ Todas as dependências estão instaladas!"
fi

# Verificar LLVM específico
echo "⚙️  Verificando LLVM..."
if ! command -v llvm-config &> /dev/null; then
    echo "❌ llvm-config não encontrado"
    echo "Instale LLVM development tools:"
    echo "  sudo apt install llvm-14-dev llvm-14-runtime"
    exit 1
fi

LLVM_VERSION=$(llvm-config --version | cut -d. -f1)
if [ "$LLVM_VERSION" -lt 14 ]; then
    echo "⚠️  LLVM versão $LLVM_VERSION detectada. Recomendado: LLVM 14+"
fi

echo "✅ Ambiente pronto para AmbarC!"