#!/bin/bash

# Instalador simples do AmbarC
echo "🚀 Instalador Simples do AmbarC"
echo "================================"

# Verificar se é root
if [ "$EUID" -ne 0 ]; then
    echo "⚠️  Este script requer sudo"
    echo "   Execute: sudo $0"
    exit 1
fi

# Diretórios
SCRIPT_DIR=$(dirname "$(realpath "$0")")
ROOT_DIR=$(dirname "$SCRIPT_DIR")
COMPILER_DIR="$ROOT_DIR/compiler"

echo "📁 Diretório: $ROOT_DIR"

# 1. Verificar/instalar dependências básicas
echo ""
echo "🔍 Verificando dependências..."
REQUIRED_PKGS=("llvm-14" "clang-14" "build-essential" "bison" "flex")

for pkg in "${REQUIRED_PKGS[@]}"; do
    if ! dpkg -l | grep -q "^ii  $pkg "; then
        echo "📦 Instalando: $pkg"
        apt-get install -y "$pkg" > /dev/null 2>&1
    fi
done

# 2. Construir o compilador se necessário
echo ""
echo "🔨 Construindo compilador..."
cd "$COMPILER_DIR" || exit 1

if [ ! -f "bin/ambarc" ]; then
    echo "  Compilando..."
    ./rm.sh > /dev/null 2>&1
    ./ir.sh
    
    if [ ! -f "bin/ambarc" ]; then
        echo "❌ Falha ao construir compilador"
        exit 1
    fi
fi

# 3. Instalar binários
echo ""
echo "📥 Instalando binários..."
install -d /usr/local/bin
install -m 755 "bin/ambarc" /usr/local/bin/
install -m 755 "compile.sh" /usr/local/bin/ambarc-compile

# 4. Configurar PATH
echo ""
echo "⚙️  Configurando ambiente..."
cat > /etc/profile.d/ambarc.sh << 'EOF'
#!/bin/bash
# Configuração do AmbarC

# Adicionar ao PATH se não estiver
AMBARC_PATH="/usr/local/bin"
if [[ ":$PATH:" != *":$AMBARC_PATH:"* ]]; then
    export PATH="$AMBARC_PATH:$PATH"
fi

# Função principal
ambarc() {
    if [ $# -eq 0 ]; then
        echo "AmbarC Compiler v1.0.0"
        echo "Uso: ambarc [opções] <arquivo.amb>"
        echo "     ambarc --help para ajuda completa"
        return 0
    fi
    /usr/local/bin/ambarc "$@"
}

# Alias para compilação rápida
alias ambarc-build='echo "Use: ambarc-compile arquivo.amb"'
EOF

chmod 644 /etc/profile.d/ambarc.sh

# 5. Criar grupo de usuários (opcional)
echo ""
echo "👥 Criando configurações..."
if ! getent group ambarc > /dev/null 2>&1; then
    groupadd -r ambarc
fi

# 6. Carregar configuração no shell atual
source /etc/profile.d/ambarc.sh

# 7. Testar instalação
echo ""
echo "🧪 Testando instalação..."
if command -v ambarc > /dev/null 2>&1; then
    echo "✅ AmbarC instalado com sucesso!"
    echo ""
    echo "🎯 Comandos disponíveis:"
    echo "   ambarc           - Compilador principal"
    echo "   ambarc-compile   - Compilar programas"
    echo ""
    echo "📝 Exemplo rápido:"
    echo "   echo 'func main() -> int { print(\"Olá!\"); return 0; }' > teste.amb"
    echo "   ambarc-compile teste.amb"
    echo "   ./teste"
    echo ""
    echo "🔧 Para recarregar configurações em terminais abertos:"
    echo "   source /etc/profile.d/ambarc.sh"
else
    echo "❌ Falha na instalação"
    exit 1
fi

echo ""
echo "🎉 Instalação completa!"
echo "   Reinicie o terminal para usar em todas as sessões."