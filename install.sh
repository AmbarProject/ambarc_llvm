#!/bin/bash

set -e

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Informações do projeto
VERSION="1.0.0"
REPO_URL="https://github.com/seu-usuario/ambar-compiler"

echo -e "${GREEN}🚀 Instalador Ambar Compiler v${VERSION}${NC}"
echo "=========================================="

# Detectar sistema operacional e arquitetura
detect_platform() {
  case "$(uname -s)" in
  Linux*) OS="linux" ;;
  Darwin*) OS="macos" ;;
  CYGWIN*) OS="windows" ;;
  MINGW*) OS="windows" ;;
  *) OS="unknown" ;;
  esac

  case "$(uname -m)" in
  x86_64*) ARCH="x64" ;;
  aarch64*) ARCH="arm64" ;;
  arm64*) ARCH="arm64" ;;
  *) ARCH="unknown" ;;
  esac

  echo "${OS}-${ARCH}"
}

PLATFORM=$(detect_platform)
BINARY_NAME="ambar-${PLATFORM}"
INSTALL_DIR="/usr/local/bin"

# Verificar se é root para instalação global
if [ "$EUID" -ne 0 ]; then
  echo -e "${YELLOW}⚠️  Executando como usuário normal. Instalando em ~/.local/bin${NC}"
  INSTALL_DIR="$HOME/.local/bin"
  mkdir -p "$INSTALL_DIR"
fi

# Download do binário
echo -e "📦 Baixando Ambar Compiler para ${PLATFORM}..."
DOWNLOAD_URL="${REPO_URL}/releases/download/v${VERSION}/${BINARY_NAME}"

if command -v curl &>/dev/null; then
  curl -L -o "/tmp/${BINARY_NAME}" "$DOWNLOAD_URL"
elif command -v wget &>/dev/null; then
  wget -O "/tmp/${BINARY_NAME}" "$DOWNLOAD_URL"
else
  echo -e "${RED}❌ Erro: curl ou wget não encontrado${NC}"
  exit 1
fi

# Verificar download
if [ ! -f "/tmp/${BINARY_NAME}" ]; then
  echo -e "${RED}❌ Erro: Download falhou${NC}"
  exit 1
fi

# Tornar executável
chmod +x "/tmp/${BINARY_NAME}"

# Instalar
echo -e "🔧 Instalando em ${INSTALL_DIR}..."
cp "/tmp/${BINARY_NAME}" "${INSTALL_DIR}/ambar"

# Verificar instalação
if command -v ambar &>/dev/null; then
  echo -e "${GREEN}✅ Ambar Compiler instalado com sucesso!${NC}"
  echo -e "📝 Use: ${YELLOW}ambar -O2 arquivo.amb${NC}"

  # Mostrar versão
  echo -e "\nℹ️  Verificando instalação:"
  ambar --version
else
  # Adicionar ao PATH se necessário
  if [[ ":$PATH:" != *":$INSTALL_DIR:"* ]]; then
    echo -e "${YELLOW}⚠️  Adicione ao seu .bashrc/.zshrc:${NC}"
    echo "export PATH=\"\$PATH:${INSTALL_DIR}\""
  fi
  echo -e "${GREEN}✅ Instalação completa! Reinicie o terminal ou execute:${NC}"
  echo "export PATH=\"\$PATH:${INSTALL_DIR}\""
fi

# Limpar
rm -f "/tmp/${BINARY_NAME}"

echo -e "\n🎉 Pronto! Comece a compilar:"
echo "ambar -O2 exemplos/hello.amb"
