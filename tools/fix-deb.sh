#!/bin/bash

# Corrigir estrutura do pacote .deb
echo "🔧 Corrigindo pacote .deb..."

# Procurar por pastas que parecem ser pacotes
PACKAGE_DIRS=$(find . -maxdepth 1 -type d -name "ambarc_*_amd64" | head -1)
if [ -n "$PACKAGE_DIRS" ]; then
    PACKAGE_DIR="$PACKAGE_DIRS"
    DEB_FILE="${PACKAGE_DIR}.deb"
    
    echo "Encontrada pasta: $PACKAGE_DIR"
    
    # Verificar se é uma estrutura de pacote válida
    if [ -f "$PACKAGE_DIR/DEBIAN/control" ]; then
        echo "📦 Estrutura válida encontrada"
        echo "🏗️  Reconstruindo pacote .deb..."
        
        # Garantir permissões corretas
        chmod 755 "$PACKAGE_DIR/DEBIAN"/* 2>/dev/null || true
        chmod 755 "$PACKAGE_DIR/etc/profile.d"/* 2>/dev/null || true
        
        # Reconstruir pacote
        dpkg-deb --build --root-owner-group "$PACKAGE_DIR" "$DEB_FILE"
        
        if [ $? -eq 0 ]; then
            echo "✅ Pacote criado: $DEB_FILE"
            echo ""
            echo "📊 Informações do pacote:"
            dpkg -I "$DEB_FILE" | grep -E "(Package|Version|Architecture|Size)"
            echo ""
            echo "📦 Para instalar:"
            echo "   sudo dpkg -i $DEB_FILE"
            echo ""
            echo "🔍 Para verificar dependências:"
            echo "   dpkg -I $DEB_FILE | grep Depends"
        else
            echo "❌ Falha ao criar pacote"
            echo ""
            echo "🔧 Tentando diagnóstico..."
            if ! command -v dpkg-deb >/dev/null; then
                echo "   ❌ dpkg-deb não encontrado"
                echo "   Instale com: sudo apt-get install dpkg-dev"
            else
                echo "   Verifique a estrutura em: $PACKAGE_DIR"
                echo "   A pasta DEBIAN/ deve conter pelo menos: control"
            fi
        fi
    else
        echo "❌ Estrutura de pacote inválida"
        echo "   Arquivo control não encontrado em: $PACKAGE_DIR/DEBIAN/"
        
        # Tentar criar estrutura básica
        echo ""
        echo "🛠️  Tentando criar estrutura básica..."
        mkdir -p "$PACKAGE_DIR/DEBIAN"
        cat > "$PACKAGE_DIR/DEBIAN/control" << EOF
Package: ambarc
Version: 1.0.0
Section: devel
Priority: optional
Architecture: amd64
Depends: llvm-18, clang-18, build-essential
Maintainer: AmbarC Team <ambarc@example.com>
Description: AmbarC Compiler
 Compiler for the AmbarC programming language.
EOF
        
        if [ -f "$PACKAGE_DIR/DEBIAN/control" ]; then
            echo "   ✅ Estrutura básica criada"
            echo "   Execute novamente este script"
        fi
    fi
else
    echo "❌ Pasta do pacote não encontrada"
    echo ""
    echo "📋 Pastas disponíveis:"
    ls -la | grep "^d" | awk '{print "   "$9}'
    echo ""
    echo "⚡ Para criar um pacote do zero, execute:"
    echo "   ./tools/create-deb.sh"
    echo ""
    echo "🔨 Ou construa manualmente:"
    echo "   make build"
    echo "   ./tools/create-deb.sh"
fi