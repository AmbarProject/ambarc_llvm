#!/bin/bash
# Installer for AmbarC - https://github.com/AmbarProject/ambarc_llvm

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}🚀 AmbarC Installer v1.0.0${NC}"
echo "=================================="

# Detect OS
if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$NAME
    VER=$VERSION_ID
else
    echo -e "${RED}❌ Não foi possível detectar o sistema operacional${NC}"
    exit 1
fi

echo -e "📋 Sistema: ${YELLOW}$OS $VER${NC}"

# Check architecture
ARCH=$(uname -m)
case $ARCH in
    x86_64)
        ARCH="amd64"
        ;;
    aarch64)
        ARCH="arm64"
        ;;
    *)
        echo -e "${RED}❌ Arquitetura não suportada: $ARCH${NC}"
        exit 1
        ;;
esac
echo -e "🔧 Arquitetura: ${YELLOW}$ARCH${NC}"

# Function to check command
check_cmd() {
    if command -v $1 >/dev/null 2>&1; then
        echo -e "  ✅ $1"
        return 0
    else
        echo -e "  ❌ $1"
        return 1
    fi
}

# Check requirements
echo ""
echo "🔍 Verificando requisitos..."
REQUIRED_CMDS=("curl" "wget" "gcc" "make")
MISSING_CMDS=()

for cmd in "${REQUIRED_CMDS[@]}"; do
    if ! check_cmd "$cmd"; then
        MISSING_CMDS+=("$cmd")
    fi
done

if [ ${#MISSING_CMDS[@]} -gt 0 ]; then
    echo -e "${YELLOW}⚠️  Comandos faltando: ${MISSING_CMDS[*]}${NC}"
    read -p "Deseja instalar automaticamente? [S/n]: " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Nn]$ ]]; then
        echo -e "${RED}❌ Instalação cancelada${NC}"
        exit 1
    fi
    
    # Install missing packages
    if [ "$ID" = "ubuntu" ] || [ "$ID" = "debian" ]; then
        sudo apt-get update
        sudo apt-get install -y "${MISSING_CMDS[@]}"
    elif [ "$ID" = "fedora" ]; then
        sudo dnf install -y "${MISSING_CMDS[@]}"
    elif [ "$ID" = "arch" ]; then
        sudo pacman -S --noconfirm "${MISSING_CMDS[@]}"
    else
        echo -e "${RED}❌ Sistema não suportado para instalação automática${NC}"
        exit 1
    fi
fi

# Installation method selection
echo ""
echo "📦 Método de instalação:"
echo "  1) Download do pacote .deb (Recomendado para Ubuntu/Debian)"
echo "  2) Build a partir do código fonte"
echo "  3) Instalar via Docker"
read -p "Selecione uma opção [1-3]: " -n 1 -r
echo ""

case $REPLY in
    1)
        # .deb package installation
        echo "📥 Baixando pacote .deb..."
        LATEST_RELEASE="https://github.com/AmbarProject/ambarc_llvm/releases/latest/download/ambarc_1.0.0_amd64.deb"
        
        if curl -s -I "$LATEST_RELEASE" | grep -q "200 OK"; then
            wget -q "$LATEST_RELEASE" -O ambarc.deb
            echo -e "${GREEN}✅ Pacote baixado${NC}"
        else
            echo -e "${YELLOW}⚠️  Release não encontrada, usando fallback${NC}"
            # Build locally if no release
            if [ -f "tools/create-deb.sh" ]; then
                ./tools/create-deb.sh
                DEB_FILE=$(ls ambarc_*.deb | head -1)
                cp "$DEB_FILE" ambarc.deb
            else
                echo -e "${RED}❌ Não foi possível obter o pacote${NC}"
                exit 1
            fi
        fi
        
        echo "📦 Instalando pacote..."
        sudo dpkg -i ambarc.deb 2>/dev/null || true
        sudo apt-get install -f -y
        
        rm -f ambarc.deb
        ;;
    
    2)
        # Source build installation
        echo "🔨 Construindo a partir do código fonte..."
        
        # Install build dependencies
        echo "📦 Instalando dependências de build..."
        if [ "$ID" = "ubuntu" ] || [ "$ID" = "debian" ]; then
            sudo apt-get update
            sudo apt-get install -y \
                build-essential \
                bison \
                flex \
                llvm-18 \
                clang-18 \
                libllvm-18-dev \
                libclang-18-dev \
                cmake \
                ninja-build \
                git
        fi
        
        # Build compiler
        if [ -f "Makefile" ]; then
            make build
        elif [ -d "compiler" ]; then
            cd compiler
            ./ir.sh
            cd ..
        else
            echo -e "${RED}❌ Diretório do compilador não encontrado${NC}"
            exit 1
        fi
        
        # Install manually
        echo "📥 Instalando binários..."
        sudo mkdir -p /usr/local/bin
        sudo cp compiler/bin/ambarc /usr/local/bin/
        sudo chmod 755 /usr/local/bin/ambarc
        
        # Create compile script
        sudo tee /usr/local/bin/ambarc-compile > /dev/null << 'EOF'
#!/bin/bash
/usr/local/bin/ambarc "$@"
EOF
        sudo chmod 755 /usr/local/bin/ambarc-compile
        
        # Create environment script
        sudo tee /etc/profile.d/ambarc.sh > /dev/null << 'EOF'
#!/bin/bash
export PATH="/usr/local/bin:$PATH"
EOF
        sudo chmod 755 /etc/profile.d/ambarc.sh
        ;;
    
    3)
        # Docker installation
        echo "🐳 Instalando via Docker..."
        
        if ! command -v docker >/dev/null 2>&1; then
            echo -e "${YELLOW}⚠️  Docker não encontrado, instalando...${NC}"
            curl -fsSL https://get.docker.com | sh
            sudo usermod -aG docker $USER
        fi
        
        echo "📦 Baixando imagem Docker..."
        docker pull ghcr.io/ambarproject/ambarc:latest
        
        echo "🚀 Criando alias para ambarc..."
        echo 'alias ambarc="docker run --rm -v $(pwd):/app ghcr.io/ambarproject/ambarc"' >> ~/.bashrc
        echo -e "${GREEN}✅ Docker configurado. Reinicie o terminal.${NC}"
        exit 0
        ;;
    
    *)
        echo -e "${RED}❌ Opção inválida${NC}"
        exit 1
        ;;
esac

# Post-installation
echo ""
echo "🎉 ${GREEN}AmbarC instalado com sucesso!${NC}"
echo ""
echo "📋 ${YELLOW}Próximos passos:${NC}"
echo "   1. Reinicie o terminal OU execute:"
echo "      source /etc/profile.d/ambarc.sh"
echo ""
echo "   2. Teste a instalação:"
echo "      ambarc --version"
echo ""
echo "   3. Compile um exemplo:"
echo "      echo 'func main() -> int { print(\"Olá!\"); return 0; }' > hello.amb"
echo "      ambarc hello.amb"
echo "      ./hello"
echo ""
echo "📚 ${YELLOW}Documentação:${NC}"
echo "   - GitHub: https://github.com/AmbarProject/ambarc_llvm"
echo "   - Wiki: https://github.com/AmbarProject/ambarc_llvm/wiki"
echo ""
echo "🐛 ${YELLOW}Problemas?${NC}"
echo "   - Issues: https://github.com/AmbarProject/ambarc_llvm/issues"