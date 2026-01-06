#!/bin/bash

# Instalador completo do AmbarC
echo "🚀 Instalador do AmbarC v1.0.0"
echo "================================"

# 1. Verificar se é root
if [ "$EUID" -ne 0 ]; then
    echo "⚠️  Este instalador requer privilégios de superusuário."
    echo "   Por favor, execute com sudo:"
    echo "   sudo $0"
    exit 1
fi

# 2. Verificar/instalar dependências
echo ""
echo "🔍 Verificando dependências..."
"$(dirname "$0")/requirements.sh"

if [ $? -ne 0 ]; then
    echo "❌ Falha na verificação de dependências"
    exit 1
fi

# 3. Criar pacote .deb
echo ""
echo "📦 Criando pacote de instalação..."
DEB_SCRIPT="$(dirname "$0")/create-deb.sh"
if [ -f "$DEB_SCRIPT" ]; then
    "$DEB_SCRIPT"
else
    echo "❌ Script create-deb.sh não encontrado"
    exit 1
fi

# 4. Encontrar o pacote .deb criado
DEB_FILE=$(find . -name "ambarc_*.deb" -type f | head -1)
if [ -z "$DEB_FILE" ]; then
    echo "❌ Pacote .deb não encontrado"
    exit 1
fi

echo ""
echo "📥 Instalando pacote: $DEB_FILE"

# 5. Instalar o pacote
dpkg -i "$DEB_FILE"

# 6. Corrigir dependências se necessário
if [ $? -ne 0 ]; then
    echo ""
    echo "🔧 Corrigindo dependências..."
    apt-get install -f -y
fi

# 7. Configurar ambiente imediatamente
echo ""
echo "⚙️  Configurando ambiente..."
if [ -f /etc/profile.d/ambarc.sh ]; then
    # Carregar configuração no shell atual
    source /etc/profile.d/ambarc.sh
    echo "✅ Configuração do PATH carregada"
else
    echo "⚠️  Configuração não encontrada. Reinicie o terminal."
fi

# 8. Testar instalação
echo ""
echo "🧪 Testando instalação..."
if command -v ambarc &> /dev/null; then
    echo "✅ AmbarC instalado com sucesso!"
    echo ""
    echo "🎯 Comandos disponíveis:"
    echo "   ambarc           - Compilador principal"
    echo "   ambarc-compile   - Script de compilação"
    echo "   ambarc --help    - Ajuda"
    echo ""
    echo "📝 Exemplo rápido:"
    echo "   echo 'func main() -> int { print(\"Olá!\"); return 0; }' > teste.amb"
    echo "   ambarc-compile teste.amb"
    echo "   ./teste"
else
    echo "❌ Falha na instalação"
    exit 1
fi

echo ""
echo "🎉 Instalação completa!"
echo "   Reinicie o terminal para carregar todas as configurações."