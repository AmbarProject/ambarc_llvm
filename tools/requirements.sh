#!/bin/bash

# Verificar e instalar dependências do AmbarC
echo "🔍 Verificando dependências do AmbarC..."
echo "=========================================="

# 1. Verificar sistema
echo "📋 Verificando sistema..."
DISTRO=$(lsb_release -si 2>/dev/null || echo "Unknown")
VERSION=$(lsb_release -sr 2>/dev/null || echo "Unknown")

echo "  Distribuição: $DISTRO"
echo "  Versão: $VERSION"

if [ "$DISTRO" != "Ubuntu" ] && [ "$DISTRO" != "Debian" ]; then
    echo "⚠️  Aviso: Este script é otimizado para Ubuntu/Debian"
    echo "   Outras distribuições podem precisar de ajustes"
fi

# 2. Lista completa de dependências (baseada no Dockerfile)
DEPENDENCIES=(
    # Dependências básicas do sistema
    "software-properties-common"
    "wget"
    "curl"
    "gnupg"
    "lsb-release"
    "ca-certificates"
    
    # Ferramentas de compilação
    "build-essential"
    "gcc"
    "g++"
    "make"
    "cmake"
    "ninja-build"
    
    # Ferramentas do compilador
    "flex"
    "bison"
    
    # Ferramentas de desenvolvimento
    "git"
    "vim"
    "lld"
    
    # Bibliotecas C/C++
    "libstdc++6"
    "libc6-dev"
    "libgcc-11-dev"
    "libtinfo5"
    "zlib1g-dev"
    "libncurses5-dev"
    "libxml2-dev"
    "libedit-dev"
    
    # LLVM 18 específico
    "llvm-18"
    "llvm-18-dev"
    "llvm-18-tools"
    "llvm-18-runtime"
    "clang-18"
    "clang-18-tools"
    "clang-18-doc"
    "lld-18"
    "libllvm18"
    "libclang-18-dev"
    
    # Bibliotecas adicionais para LLVM
    "libffi-dev"
    "libz3-dev"
    "libomp-18-dev"
    "libpolly-18-dev"
)

# Dependências opcionais/recomendadas
RECOMMENDED=(
    "gdb"
    "valgrind"
    "doxygen"
    "graphviz"
    "python3-dev"
    "python3-pip"
    "ccache"
    "bear"  # Para compilation database
)

echo ""
echo "📦 Dependências necessárias:"
printf '  - %s\n' "${DEPENDENCIES[@]}"

echo ""
echo "🌟 Dependências recomendadas:"
printf '  - %s\n' "${RECOMMENDED[@]}"

# 3. Verificar se é root/sudo
echo ""
if [ "$EUID" -ne 0 ]; then
    echo "🔑 Este script precisa de privilégios de superusuário"
    echo "   Tentando usar sudo..."
    
    # Testar sudo
    sudo -v
    if [ $? -ne 0 ]; then
        echo "❌ Não foi possível obter privilégios sudo"
        echo "   Execute como root ou configure sudo"
        exit 1
    fi
    SUDO_CMD="sudo"
else
    SUDO_CMD=""
fi

# 4. Adicionar repositórios necessários
echo ""
echo "➕ Adicionando repositórios..."

# Adicionar PPA para gcc mais recente se necessário
if ! $SUDO_CMD add-apt-repository -y ppa:ubuntu-toolchain-r/test 2>/dev/null; then
    echo "⚠️  Não foi possível adicionar PPA ubuntu-toolchain-r/test"
    echo "   Continuando sem ele..."
fi

# Adicionar repositório LLVM 18
echo "📥 Adicionando repositório LLVM 18..."
$SUDO_CMD bash -c "$(wget -O - https://apt.llvm.org/llvm.sh 2>/dev/null || curl -fsSL https://apt.llvm.org/llvm.sh)"

# 5. Atualizar repositórios
echo ""
echo "🔄 Atualizando lista de pacotes..."
$SUDO_CMD apt-get update

# 6. Verificar e instalar dependências principais
echo ""
echo "📥 Instalando dependências principais..."
MISSING_DEPS=()

# Verificar cada dependência
for dep in "${DEPENDENCIES[@]}"; do
    if ! dpkg -l | grep -q "^ii  $dep "; then
        MISSING_DEPS+=("$dep")
    fi
done

if [ ${#MISSING_DEPS[@]} -gt 0 ]; then
    echo "  Instalando ${#MISSING_DEPS[@]} pacotes..."
    $SUDO_CMD apt-get install -y "${MISSING_DEPS[@]}"
    
    if [ $? -eq 0 ]; then
        echo "✅ Dependências principais instaladas!"
    else
        echo "❌ Falha ao instalar algumas dependências"
        echo "   Tentando instalar individualmente..."
        
        for dep in "${MISSING_DEPS[@]}"; do
            echo "  Tentando instalar: $dep"
            $SUDO_CMD apt-get install -y "$dep" || echo "  ⚠️  Falha: $dep"
        done
    fi
else
    echo "✅ Todas as dependências principais já estão instaladas!"
fi

# 7. Instalar dependências recomendadas
echo ""
echo "🌟 Instalando dependências recomendadas..."
MISSING_RECOMMENDED=()

for rec in "${RECOMMENDED[@]}"; do
    if ! dpkg -l | grep -q "^ii  $rec "; then
        MISSING_RECOMMENDED+=("$rec")
    fi
done

if [ ${#MISSING_RECOMMENDED[@]} -gt 0 ]; then
    read -p "Deseja instalar dependências recomendadas? (${#MISSING_RECOMMENDED[@]} pacotes) [S/n]: " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Nn]$ ]]; then
        echo "  Pulando dependências recomendadas..."
    else
        echo "  Instalando dependências recomendadas..."
        $SUDO_CMD apt-get install -y "${MISSING_RECOMMENDED[@]}"
        echo "✅ Dependências recomendadas instaladas!"
    fi
else
    echo "✅ Todas as dependências recomendadas já estão instaladas!"
fi

# 8. Configurar alternativas LLVM
echo ""
echo "⚙️  Configurando LLVM 18 como padrão..."

# Configurar symlinks para LLVM 18
LLVM_TOOLS=("llvm-config" "clang" "clang++" "lld" "llc" "opt" "llvm-dis")

for tool in "${LLVM_TOOLS[@]}"; do
    if [ -f "/usr/lib/llvm-18/bin/$tool" ]; then
        $SUDO_CMD update-alternatives --install "/usr/bin/$tool" "$tool" "/usr/lib/llvm-18/bin/$tool" 100
        echo "  ✅ $tool configurado"
    fi
done

# 9. Verificar instalação
echo ""
echo "🧪 Verificando instalação..."

# Função para verificar comando
check_cmd() {
    if command -v $1 >/dev/null 2>&1; then
        echo "  ✅ $1: $(command -v $1)"
        return 0
    else
        echo "  ❌ $1: NÃO ENCONTRADO"
        return 1
    fi
}

echo "  Ferramentas essenciais:"
check_cmd gcc
check_cmd g++
check_cmd make
check_cmd cmake
check_cmd flex
check_cmd bison
check_cmd git

echo ""
echo "  Ferramentas LLVM:"
check_cmd llvm-config
check_cmd clang
check_cmd clang++
check_cmd llc
check_cmd opt

# Verificar versão do LLVM
echo ""
if command -v llvm-config >/dev/null 2>&1; then
    LLVM_VERSION=$(llvm-config --version)
    echo "  📊 LLVM versão: $LLVM_VERSION"
    
    # Verificar se é LLVM 18
    if [[ $LLVM_VERSION == 18* ]]; then
        echo "  ✅ LLVM 18 detectado!"
    else
        echo "  ⚠️  LLVM versão diferente de 18 detectada"
        echo "     Versão esperada: 18.x"
    fi
else
    echo "  ❌ llvm-config não encontrado"
fi

# 10. Verificar bibliotecas
echo ""
echo "📚 Verificando bibliotecas..."

check_lib() {
    if ldconfig -p | grep -q "$1"; then
        echo "  ✅ $1: OK"
        return 0
    else
        echo "  ❌ $1: NÃO ENCONTRADA"
        return 1
    fi
}

check_lib "libLLVM-18"
check_lib "libclang-18"
check_lib "libstdc++"
check_lib "libtinfo"

# 11. Criar ambiente de desenvolvimento
echo ""
echo "🚀 Configurando ambiente de desenvolvimento..."

# Criar diretório para exemplos
EXAMPLES_DIR="$HOME/ambarc-examples"
if [ ! -d "$EXAMPLES_DIR" ]; then
    mkdir -p "$EXAMPLES_DIR"
    echo "  📁 Criado: $EXAMPLES_DIR"
fi

# Criar arquivo de configuração
CONFIG_DIR="$HOME/.config/ambarc"
mkdir -p "$CONFIG_DIR"

cat > "$CONFIG_DIR/env.sh" << 'EOF'
#!/bin/bash
# Configuração do ambiente AmbarC

export AMBARC_HOME="$HOME/.config/ambarc"

# Adicionar LLVM 18 ao PATH se não estiver
LLVM_18_PATH="/usr/lib/llvm-18/bin"
if [[ ":$PATH:" != *":$LLVM_18_PATH:"* ]]; then
    export PATH="$LLVM_18_PATH:$PATH"
fi

# Configurar variáveis para desenvolvimento
export CC=clang-18
export CXX=clang++-18
export LD=ld.lld-18

# Flags de compilação recomendadas
export CFLAGS="-O2 -march=native"
export CXXFLAGS="-O2 -march=native -stdlib=libc++"

# Para debugging
export AMBARC_DEBUG=0
EOF

chmod +x "$CONFIG_DIR/env.sh"
echo "  ⚙️  Configuração criada: $CONFIG_DIR/env.sh"

# 12. Testar compilação simples
echo ""
echo "🔨 Testando ambiente de compilação..."

cat > /tmp/test_ambarc.c << 'EOF'
#include <stdio.h>
int main() {
    printf("✅ Ambiente de compilação funcionando!\n");
    return 0;
}
EOF

if clang-18 /tmp/test_ambarc.c -o /tmp/test_ambarc 2>/dev/null; then
    /tmp/test_ambarc
    echo "  ✅ Compilação C funcionando!"
else
    echo "  ❌ Falha na compilação C"
fi

rm -f /tmp/test_ambarc /tmp/test_ambarc.c

# 13. Resumo
echo ""
echo "=========================================="
echo "🎉 VERIFICAÇÃO DE DEPENDÊNCIAS CONCLUÍDA!"
echo "=========================================="
echo ""
echo "📊 RESUMO:"
echo "  ✅ Sistema: $DISTRO $VERSION"
echo "  ✅ Dependências instaladas"
echo "  ✅ LLVM 18 configurado"
echo "  ✅ Ambiente pronto para AmbarC"
echo ""
echo "🚀 PRÓXIMOS PASSOS:"
echo "  1. Construir o compilador AmbarC:"
echo "     cd ambarc_llvm && make build"
echo ""
echo "  2. Testar o compilador:"
echo "     ./compiler/bin/ambarc --version"
echo ""
echo "  3. (Opcional) Instalar globalmente:"
echo "     sudo make install"
echo ""
echo "💡 DICA: Para carregar as configurações do ambiente:"
echo "     source $CONFIG_DIR/env.sh"
echo ""
echo "🐛 Para problemas, verifique:"
echo "   - Permissões de arquivo"
echo "   - Conflitos de versão do LLVM"
echo "   - Espaço em disco disponível"