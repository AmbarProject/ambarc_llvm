#!/bin/bash

# Criar pacote .deb para AmbarC
echo "📦 Criando pacote Debian/Ubuntu..."

VERSION="1.0.0"
ARCH="amd64"
PACKAGE_NAME="ambarc"
PACKAGE_DIR="${PACKAGE_NAME}_${VERSION}_${ARCH}"
ROOT_DIR=$(dirname "$(dirname "$(realpath "$0")")")

echo "Versão: $VERSION"
echo "Arquitetura: $ARCH"
echo "Diretório raiz: $ROOT_DIR"

# Criar estrutura do pacote
echo "🏗️  Criando estrutura do pacote..."
rm -rf "$PACKAGE_DIR"
mkdir -p "$PACKAGE_DIR/DEBIAN"
mkdir -p "$PACKAGE_DIR/usr/local/bin"
mkdir -p "$PACKAGE_DIR/usr/local/lib/ambarc"
mkdir -p "$PACKAGE_DIR/usr/local/share/ambarc"
mkdir -p "$PACKAGE_DIR/etc/profile.d"
mkdir -p "$PACKAGE_DIR/usr/local/share/man/man1"
mkdir -p "$PACKAGE_DIR/usr/local/share/doc/ambarc"

# 1. Construir o compilador se necessário
echo "🔨 Construindo compilador..."
cd "$ROOT_DIR/compiler" || exit 1

if [ ! -f "bin/ambarc" ]; then
    echo "  Compilador não encontrado, construindo..."
    ./rm.sh
    ./ir.sh
    
    if [ ! -f "bin/ambarc" ]; then
        echo "❌ Falha ao construir compilador"
        exit 1
    fi
fi

# 2. Copiar binário
echo "📋 Copiando binário..."
cp "bin/ambarc" "$PACKAGE_DIR/usr/local/bin/"
chmod 755 "$PACKAGE_DIR/usr/local/bin/ambarc"

# 3. Copiar scripts auxiliares
echo "📜 Copiando scripts..."
cp "compile.sh" "$PACKAGE_DIR/usr/local/bin/ambarc-compile"
chmod 755 "$PACKAGE_DIR/usr/local/bin/ambarc-compile"

# 4. Criar script para configurar PATH
echo "⚙️  Criando configuração de ambiente..."
cat > "$PACKAGE_DIR/etc/profile.d/ambarc.sh" << 'EOF'
#!/bin/bash
# Configuração do PATH para AmbarC
AMBARC_PATH="/usr/local/bin"

if [[ ":$PATH:" != *":$AMBARC_PATH:"* ]]; then
    export PATH="$PATH:$AMBARC_PATH"
fi

# Aliases úteis
alias ambarc-build='cd /usr/local/lib/ambarc && ./build.sh'
alias ambarc-clean='rm -f *.ll *.o'

# Função para compilar rapidamente
ambarc() {
    if [ $# -eq 0 ]; then
        echo "Uso: ambarc [opções] <arquivo.amb>"
        echo "     ambarc --help para ajuda"
        return 1
    fi
    /usr/local/bin/ambarc "$@"
}
EOF

chmod 644 "$PACKAGE_DIR/etc/profile.d/ambarc.sh"

# 5. Criar script de inicialização
echo "🚀 Criando script de inicialização..."
cat > "$PACKAGE_DIR/usr/local/lib/ambarc/init.sh" << 'EOF'
#!/bin/bash
# Inicialização do AmbarC

echo "✨ AmbarC v1.0.0 inicializado!"
echo "Comandos disponíveis:"
echo "  ambarc           - Compilador principal"
echo "  ambarc-compile   - Compilar programas Ambar"
echo "  ambarc --help    - Mostrar ajuda"
echo ""
echo "Exemplo: ambarc-compile programa.amb"
EOF

chmod 755 "$PACKAGE_DIR/usr/local/lib/ambarc/init.sh"

# 6. Criar arquivo de controle do pacote
echo "📄 Criando control..."
cat > "$PACKAGE_DIR/DEBIAN/control" << EOF
Package: $PACKAGE_NAME
Version: $VERSION
Section: devel
Priority: optional
Architecture: $ARCH
Depends: 
  llvm-18 (>= 18.1.3),
  clang-18 (>= 18.1.3),
  build-essential,
  bison,
  flex,
  libstdc++6,
  libtinfo5,
  zlib1g,
  libffi7,
  libedit2,
  libxml2,
  libncurses5,
  libz3-4
Recommends: 
  gdb,
  valgrind,
  cmake,
  ninja-build,
  ccache,
  python3,
  git
Suggests:
  ambarc-examples,
  ambarc-doc
Maintainer: AmbarC Team <ambarc@example.com>
Description: AmbarC Programming Language Compiler
 AmbarC é uma linguagem de programação moderna e tipada estaticamente
 que compila para LLVM IR 18. Este pacote contém o compilador completo
 e todas as ferramentas necessárias para desenvolvimento.
 .
 Características:
  * Suporte completo a LLVM 18.1.3
  * Sintaxe limpa e moderna
  * Tipagem estática forte
  * Compilação para LLVM IR
  * Sistema de tipos avançado
  * Otimizações automáticas
  * Integração com ferramentas de debugging
Homepage: https://github.com/ambarc/compiler
Vcs-Browser: https://github.com/ambarc/compiler
Vcs-Git: https://github.com/ambarc/compiler.git
EOF

# 7. Criar scripts de pós-instalação
echo "🔧 Criando scripts de instalação..."
cat > "$PACKAGE_DIR/DEBIAN/postinst" << 'EOF'
#!/bin/bash
# Script pós-instalação do AmbarC

echo "🎉 AmbarC v1.0.0 instalado com sucesso!"
echo ""
echo "Para usar o AmbarC:"
echo "1. Reinicie seu terminal OU execute:"
echo "   source /etc/profile.d/ambarc.sh"
echo ""
echo "2. Teste a instalação:"
echo "   ambarc --version"
echo ""
echo "3. Para compilar um programa:"
echo "   ambarc-compile programa.amb"
echo ""
echo "Documentação: /usr/local/share/doc/ambarc/"
EOF

cat > "$PACKAGE_DIR/DEBIAN/prerm" << 'EOF'
#!/bin/bash
# Script pré-remoção
echo "Removendo AmbarC..."
EOF

cat > "$PACKAGE_DIR/DEBIAN/postrm" << 'EOF'
#!/bin/bash
# Script pós-remoção
echo "AmbarC removido com sucesso!"
echo "Para remover completamente do PATH, edite seu ~/.bashrc"
EOF

chmod 755 "$PACKAGE_DIR/DEBIAN/postinst"
chmod 755 "$PACKAGE_DIR/DEBIAN/prerm"
chmod 755 "$PACKAGE_DIR/DEBIAN/postrm"

# 8. Criar documentação
echo "📚 Criando documentação..."
cat > "$PACKAGE_DIR/usr/local/share/doc/ambarc/README" << 'EOF'
AMBARC - COMPILER v1.0.0
========================

AmbarC é uma linguagem de programação moderna que compila para LLVM IR.

INSTALAÇÃO
----------
O pacote já configura automaticamente:
1. Binário em /usr/local/bin/ambarc
2. PATH configurado via /etc/profile.d/ambarc.sh
3. Aliases úteis no terminal

USO BÁSICO
----------
# Compilar um programa
ambarc-compile programa.amb

# Ou usar diretamente
ambarc -O2 programa.amb

# Mostrar ajuda
ambarc --help

EXEMPLOS
--------
Veja exemplos em /usr/local/share/ambarc/examples/

AJUDA
-----
- Manual: man ambarc
- Documentação: /usr/local/share/doc/ambarc/
- GitHub: https://github.com/ambarc/compiler

LICENÇA
-------
MIT License - veja /usr/share/common-licenses/MIT
EOF

# 9. Criar man page
echo "📖 Criando man page..."
cat > "$PACKAGE_DIR/usr/local/share/man/man1/ambarc.1" << 'EOF'
.TH AMBARC 1 "2024-01-01" "AmbarC v1.0.0"
.SH NAME
ambarc \- AmbarC Programming Language Compiler
.SH SYNOPSIS
.B ambarc
[\fIOPTIONS\fR] \fIFILE\fR
.PP
.B ambarc\-compile
[\fIOPTIONS\fR] \fIFILE\fR
.SH DESCRIPTION
AmbarC is a modern, statically-typed programming language that compiles
to LLVM Intermediate Representation (IR).
.PP
The compiler reads AmbarC source files (.amb) and generates LLVM IR code
which can then be compiled to native executables.
.SH OPTIONS
.TP
.B \-O0, \-O1, \-O2, \-O3, \-Os
Set optimization level
.TP
.B \-\-show\-ir
Show generated LLVM IR
.TP
.B \-\-version
Show version information
.TP
.B \-\-help
Show help message
.SH ENVIRONMENT
The installer automatically adds /usr/local/bin to your PATH and creates
useful aliases. See /etc/profile.d/ambarc.sh for details.
.SH FILES
.TP
.B /usr/local/bin/ambarc
The main compiler executable
.TP
.B /usr/local/bin/ambarc\-compile
Convenience script for compiling programs
.TP
.B /etc/profile.d/ambarc.sh
Environment configuration
.TP
.B /usr/local/share/ambarc/examples/
Example programs
.SH EXAMPLES
Compile a program with optimizations:
.PP
.nf
.RS
ambarc \-O2 hello.amb
.RE
.fi
.PP
Compile using the helper script:
.PP
.nf
.RS
ambarc\-compile \-O3 program.amb
.RE
.fi
.SH SEE ALSO
.BR clang (1),
.BR llc (1),
.BR llvm\-config (1)
.SH AUTHORS
AmbarC Team <ambarc@example.com>
.SH BUGS
Report bugs at https://github.com/ambarc/compiler/issues
EOF

gzip -9 "$PACKAGE_DIR/usr/local/share/man/man1/ambarc.1"

# 10. Criar exemplos
echo "📝 Criando exemplos..."
mkdir -p "$PACKAGE_DIR/usr/local/share/ambarc/examples"
cat > "$PACKAGE_DIR/usr/local/share/ambarc/examples/hello.amb" << 'EOF'
// Exemplo Hello World em AmbarC
func main() -> int {
    print("Olá, Mundo!");
    return 0;
}
EOF

cat > "$PACKAGE_DIR/usr/local/share/ambarc/examples/fibonacci.amb" << 'EOF'
// Exemplo Fibonacci em AmbarC
func fibonacci(n: int) -> int {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

func main() -> int {
    print("Fibonacci(10) = ", fibonacci(10));
    return 0;
}
EOF

# 11. Construir pacote .deb
echo "🏗️  Construindo pacote .deb..."
dpkg-deb --build --root-owner-group "$PACKAGE_DIR"

if [ $? -eq 0 ]; then
    echo "✅ Pacote criado com sucesso: ${PACKAGE_DIR}.deb"
    echo ""
    echo "📦 PARA INSTALAR:"
    echo "   sudo dpkg -i ${PACKAGE_DIR}.deb"
    echo ""
    echo "🔧 SE HOUVER DEPENDÊNCIAS FALTANDO:"
    echo "   sudo apt-get install -f"
    echo ""
    echo "🚀 APÓS INSTALAR:"
    echo "   Reinicie o terminal OU execute:"
    echo "   source /etc/profile"
    echo ""
    echo "🎯 TESTE A INSTALAÇÃO:"
    echo "   ambarc --version"
else
    echo "❌ Erro ao criar pacote .deb"
    exit 1
fi