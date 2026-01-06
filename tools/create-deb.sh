#!/bin/bash

# Criar pacote .deb para AmbarC
echo "📦 Criando pacote Debian/Ubuntu..."

VERSION="1.0.0"
ARCH="amd64"
PACKAGE_NAME="ambarc"
PACKAGE_DIR="${PACKAGE_NAME}_${VERSION}_${ARCH}"

# Encontrar diretório correto
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"
COMPILER_DIR="$ROOT_DIR/compiler"

echo "Versão: $VERSION"
echo "Arquitetura: $ARCH"
echo "Diretório raiz: $ROOT_DIR"
echo "Diretório do compilador: $COMPILER_DIR"

# Limpar pacote anterior
echo "🧹 Limpando pacotes antigos..."
rm -rf "$PACKAGE_DIR"
rm -f "${PACKAGE_DIR}.deb"

# Criar estrutura completa
echo "🏗️  Criando estrutura completa do pacote..."
mkdir -p "$PACKAGE_DIR/DEBIAN"
mkdir -p "$PACKAGE_DIR/usr/local/bin"
mkdir -p "$PACKAGE_DIR/usr/local/lib/ambarc"
mkdir -p "$PACKAGE_DIR/usr/local/share/ambarc"
mkdir -p "$PACKAGE_DIR/etc/profile.d"
mkdir -p "$PACKAGE_DIR/usr/local/share/man/man1"
mkdir -p "$PACKAGE_DIR/usr/local/share/doc/ambarc"
mkdir -p "$PACKAGE_DIR/usr/local/share/ambarc/examples"

# 1. Construir compilador
echo "🔨 Construindo compilador..."
cd "$COMPILER_DIR" || exit 1

if [ ! -f "bin/ambarc" ]; then
    echo "  Construindo..."
    if [ -f "rm.sh" ]; then
        chmod +x rm.sh
        ./rm.sh
    fi
    
    if [ -f "ir.sh" ]; then
        chmod +x ir.sh
        ./ir.sh
    fi
    
    if [ ! -f "bin/ambarc" ]; then
        echo "❌ Falha ao construir compilador"
        exit 1
    fi
fi

# 2. Copiar binário REAL do compilador
echo "📋 Copiando binário real do compilador..."
mkdir -p "$ROOT_DIR/$PACKAGE_DIR/usr/local/lib/ambarc/bin"
cp "bin/ambarc" "$ROOT_DIR/$PACKAGE_DIR/usr/local/lib/ambarc/bin/ambarc-real"
chmod 755 "$ROOT_DIR/$PACKAGE_DIR/usr/local/lib/ambarc/bin/ambarc-real"

# 3. Criar wrapper que gera executável completo
echo "📝 Criando wrapper de compilação completa..."
cat > "$ROOT_DIR/$PACKAGE_DIR/usr/local/bin/ambarc" << 'EOF'
#!/bin/bash
# Wrapper do AmbarC que gera executável completo

# Configurações padrão
OPT_LEVEL="0"
SHOW_IR=false
KEEP_IR=false
KEEP_OBJ=false
OUTPUT_FILE=""
INPUT_FILE=""
OTHER_ARGS=()

# Parse argumentos
while [[ $# -gt 0 ]]; do
    case $1 in
        -O0|-O1|-O2|-O3|-Os)
            OPT_LEVEL="${1#-O}"
            shift
            ;;
        --show-ir)
            SHOW_IR=true
            shift
            ;;
        --keep-ir)
            KEEP_IR=true
            shift
            ;;
        --keep-obj)
            KEEP_OBJ=true
            shift
            ;;
        -o)
            if [[ -n "$2" ]]; then
                OUTPUT_FILE="$2"
                shift 2
            else
                echo "❌ Erro: Opção -o requer um argumento"
                exit 1
            fi
            ;;
        --help|-h)
            echo "Uso: ambarc [OPÇÕES] <arquivo.amb>"
            echo ""
            echo "Opções:"
            echo "  -O0, -O1, -O2, -O3, -Os  Nível de otimização"
            echo "  -o <arquivo>             Nome do executável de saída"
            echo "  --show-ir                Mostrar código IR gerado"
            echo "  --keep-ir                Manter arquivo .ll"
            echo "  --keep-obj               Manter arquivo .o"
            echo "  --help, -h               Mostrar ajuda"
            echo ""
            echo "Exemplos:"
            echo "  ambarc programa.amb      # Gera 'programa'"
            echo "  ambarc -O2 -o prog programa.amb"
            echo "  ambarc -O3 --show-ir programa.amb"
            exit 0
            ;;
        --version|-v)
            echo "AmbarC Compiler v1.0.0"
            exit 0
            ;;
        -*)
            # Passar outros argumentos para o compilador real
            OTHER_ARGS+=("$1")
            shift
            ;;
        *)
            if [[ -z "$INPUT_FILE" ]]; then
                INPUT_FILE="$1"
            else
                echo "⚠️  Aviso: Ignorando argumento extra '$1'"
            fi
            shift
            ;;
    esac
done

# Verificar arquivo de entrada
if [[ -z "$INPUT_FILE" ]]; then
    echo "❌ Erro: Nenhum arquivo .amb especificado"
    echo "   Use: ambarc [opções] <arquivo.amb>"
    exit 1
fi

if [[ ! -f "$INPUT_FILE" ]]; then
    echo "❌ Erro: Arquivo '$INPUT_FILE' não encontrado"
    exit 1
fi

if [[ "$INPUT_FILE" != *.amb ]]; then
    echo "⚠️  Aviso: O arquivo '$INPUT_FILE' não tem extensão .amb"
fi

# Extrair nome base
BASENAME=$(basename "$INPUT_FILE" .amb)
SRC_DIR=$(dirname "$(realpath "$INPUT_FILE")")

# Definir arquivos temporários
LL_FILE="$SRC_DIR/$BASENAME.ll"
OBJ_FILE="$SRC_DIR/$BASENAME.o"
EXEC_FILE=""

if [[ -n "$OUTPUT_FILE" ]]; then
    EXEC_FILE="$OUTPUT_FILE"
else
    EXEC_FILE="$SRC_DIR/$BASENAME"
fi

echo "🔍 Compilando: $INPUT_FILE"

# 1. Gerar código LLVM IR
echo "🏗️  Gerando código LLVM IR..."
if ! /usr/local/lib/ambarc/bin/ambarc-real "-O$OPT_LEVEL" "${OTHER_ARGS[@]}" "$INPUT_FILE"; then
    echo "❌ Erro: Falha ao gerar código LLVM"
    exit 1
fi

# Verificar se o arquivo .ll foi gerado
if [[ ! -f "$LL_FILE" ]]; then
    echo "❌ Erro: Arquivo LLVM IR não foi gerado: $LL_FILE"
    exit 1
fi

# Mostrar IR se solicitado
if [[ "$SHOW_IR" = true ]]; then
    echo ""
    echo "📄 CÓDIGO IR GERADO:"
    echo "=========================================="
    cat "$LL_FILE"
    echo "=========================================="
    echo ""
fi

# 2. Compilar IR para objeto
echo "🔨 Compilando IR para objeto..."
if ! llc "-O$OPT_LEVEL" -mtriple=x86_64-unknown-linux-gnu -filetype=obj "$LL_FILE" -o "$OBJ_FILE"; then
    echo "❌ Erro: Falha ao compilar objeto"
    exit 1
fi

# 3. Linkar para executável
echo "🔗 Linkando executável..."
if ! gcc -no-pie "$OBJ_FILE" -o "$EXEC_FILE"; then
    echo "❌ Erro: Falha no linking"
    exit 1
fi

chmod +x "$EXEC_FILE"

echo "✅ Executável criado: $EXEC_FILE"

# 4. Limpeza
if [[ "$KEEP_IR" = false ]]; then
    rm -f "$LL_FILE"
    echo "🗑️  Removido arquivo IR: $LL_FILE"
fi

if [[ "$KEEP_OBJ" = false ]]; then
    rm -f "$OBJ_FILE"
    echo "🗑️  Removido arquivo objeto: $OBJ_FILE"
fi

echo "🎉 Compilação concluída com sucesso!"
EOF

chmod 755 "$ROOT_DIR/$PACKAGE_DIR/usr/local/bin/ambarc"

# 4. Criar ambarc-compile (alias para compatibilidade)
echo "📜 Criando ambarc-compile (alias)..."
cat > "$ROOT_DIR/$PACKAGE_DIR/usr/local/bin/ambarc-compile" << 'EOF'
#!/bin/bash
echo "📢 Nota: 'ambarc-compile' está obsoleto. Use 'ambarc' diretamente."
echo "       Exemplo: ambarc $@"
echo ""
/usr/local/bin/ambarc "$@"
EOF
chmod 755 "$ROOT_DIR/$PACKAGE_DIR/usr/local/bin/ambarc-compile"

cd "$ROOT_DIR" || exit 1

# 5. Criar arquivo de controle
echo "📄 Criando control..."
cat > "$PACKAGE_DIR/DEBIAN/control" << EOF
Package: $PACKAGE_NAME
Version: $VERSION
Section: devel
Priority: optional
Architecture: $ARCH
Depends: llvm-18 (>= 18.1.3), clang-18 (>= 18.1.3), build-essential, bison, flex, libstdc++6, libtinfo6, zlib1g, libffi8 | libffi7, libedit2, libxml2, libncurses6 | libncurses5, libz3-4, gcc
Recommends: gdb, valgrind, cmake, ninja-build, ccache, python3, git
Suggests: ambarc-examples, ambarc-doc
Maintainer: AmbarC Team <ambarc@example.com>
Description: AmbarC Programming Language Compiler
 AmbarC é uma linguagem de programação moderna e tipada estaticamente
 que compila para LLVM IR 18. Este pacote gera executáveis completos
 diretamente a partir do código fonte.
 .
 Características:
  * Geração automática de executáveis
  * Suporte completo a LLVM 18.1.3
  * Sintaxe limpa e moderna
  * Tipagem estática forte
  * Compilação completa (fonte → executável)
  * Sistema de tipos avançado
  * Otimizações automáticas
Homepage: https://github.com/ambarc/compiler
Vcs-Browser: https://github.com/ambarc/compiler
Vcs-Git: https://github.com/ambarc/compiler.git
EOF

# 6. Criar scripts de instalação
echo "🔧 Criando scripts de instalação..."
cat > "$PACKAGE_DIR/DEBIAN/postinst" << 'EOF'
#!/bin/bash
set -e

echo "🎉 AmbarC v1.0.0 instalado com sucesso!"
echo ""
echo "✨ Funcionalidades:"
echo "  • ambarc          - Compila arquivos .amb para executáveis"
echo "  • ambarc-compile  - Alias para compatibilidade"
echo ""
echo "📚 Exemplos de uso:"
echo "  ambarc programa.amb              # Gera 'programa'"
echo "  ambarc -O2 -o saida arquivo.amb  # Otimizado, nome personalizado"
echo "  ambarc --help                    # Ajuda completa"
echo ""
echo "💡 Dica: Reinicie o terminal ou execute:"
echo "        source /etc/profile.d/ambarc.sh"
EOF

cat > "$PACKAGE_DIR/DEBIAN/prerm" << 'EOF'
#!/bin/bash
echo "Removendo AmbarC..."
EOF

cat > "$PACKAGE_DIR/DEBIAN/postrm" << 'EOF'
#!/bin/bash
echo "AmbarC removido com sucesso!"
EOF

chmod 755 "$PACKAGE_DIR/DEBIAN/postinst"
chmod 755 "$PACKAGE_DIR/DEBIAN/prerm"
chmod 755 "$PACKAGE_DIR/DEBIAN/postrm"

# 7. Criar configuração de ambiente
echo "⚙️  Criando configuração de ambiente..."
cat > "$PACKAGE_DIR/etc/profile.d/ambarc.sh" << 'EOF'
#!/bin/bash
# Configuração do PATH para AmbarC
AMBARC_PATH="/usr/local/bin"

if [[ ":$PATH:" != *":$AMBARC_PATH:"* ]]; then
    export PATH="$PATH:$AMBARC_PATH"
fi

# Função helper (opcional)
ambarc-build() {
    if [ $# -eq 0 ]; then
        echo "Uso: ambarc-build <arquivo.amb>"
        echo "     (alias para: ambarc -O2 <arquivo.amb>)"
        return 1
    fi
    ambarc -O2 "$@"
}
EOF
chmod 755 "$PACKAGE_DIR/etc/profile.d/ambarc.sh"

# 8. Criar script de build para uso avançado
echo "🔨 Criando script de build avançado..."
cat > "$PACKAGE_DIR/usr/local/lib/ambarc/build.sh" << 'EOF'
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
EOF
chmod 755 "$PACKAGE_DIR/usr/local/lib/ambarc/build.sh"

# 9. Criar documentação
echo "📚 Criando documentação..."
cat > "$PACKAGE_DIR/usr/local/share/doc/ambarc/README" << 'EOF'
AMBARC - COMPILER v1.0.0
========================

AmbarC é uma linguagem de programação moderna que compila para executáveis nativos.

📦 INSTALAÇÃO
-------------
O pacote já está instalado. Para usar:

1. Reinicie o terminal OU execute:
   source /etc/profile.d/ambarc.sh

2. Teste:
   ambarc --version

🚀 USO BÁSICO
-------------
# Compilar para executável
ambarc programa.amb              # Gera 'programa'

# Com otimizações
ambarc -O2 programa.amb

# Nome personalizado
ambarc -o meuapp programa.amb

# Ver código intermediário
ambarc --show-ir programa.amb

🔧 USO AVANÇADO
---------------
# Apenas gerar IR
/usr/local/lib/ambarc/build.sh --only-ir programa.amb

# Apenas gerar objeto
/usr/local/lib/ambarc/build.sh --only-obj programa.amb

# Build com opções personalizadas
/usr/local/lib/ambarc/build.sh --llc-opts "-O3 -march=native" programa.amb

📝 EXEMPLOS
-----------
Veja exemplos em: /usr/local/share/ambarc/examples/

💡 DICAS
--------
• Use 'ambarc --help' para todas as opções
• O comando gera executáveis prontos para uso
• Arquivos temporários (.ll, .o) são removidos automaticamente
• Use --keep-ir ou --keep-obj para manter arquivos intermediários

🐛 PROBLEMAS
------------
Se encontrar problemas:
1. Verifique se tem gcc instalado: gcc --version
2. Verifique LLVM: llc --version
3. Reporte em: https://github.com/ambarc/compiler/issues
EOF

# 10. Criar exemplos
echo "📝 Criando exemplos..."
cat > "$PACKAGE_DIR/usr/local/share/ambarc/examples/hello.amb" << 'EOF'
// Exemplo Hello World em AmbarC
func main() -> int {
    print("Olá, Mundo do AmbarC!");
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
    for i in 0..10 {
        print("fib(", i, ") = ", fibonacci(i));
    }
    return 0;
}
EOF

# 11. Criar man page simplificada
echo "📖 Criando man page..."
mkdir -p "$PACKAGE_DIR/usr/share/man/man1"
cat > "$PACKAGE_DIR/usr/share/man/man1/ambarc.1" << 'EOF'
.TH AMBARC 1 "2024-01-01" "AmbarC v1.0.0"
.SH NAME
ambarc \- Compilador AmbarC (fonte para executável)
.SH SYNOPSIS
.B ambarc
[\fIOPTIONS\fR] \fIFILE.amb\fR
.SH DESCRIPTION
Compila arquivos fonte AmbarC (.amb) diretamente para executáveis nativos.
O processo inclui geração de LLVM IR, compilação para objeto e linking.
.SH OPTIONS
.TP
.B \-O0, \-O1, \-O2, \-O3, \-Os
Nível de otimização (padrão: -O0)
.TP
.B \-o \fIFILE\fR
Nome do executável de saída
.TP
.B \-\-show\-ir
Mostrar código LLVM IR gerado
.TP
.B \-\-keep\-ir
Manter arquivo .ll após compilação
.TP
.B \-\-keep\-obj
Manter arquivo .o após compilação
.TP
.B \-\-help
Mostrar ajuda
.TP
.B \-\-version
Mostrar versão
.SH EXAMPLES
Compilar com otimizações:
.PP
.nf
.RS
ambarc \-O2 programa.amb
.RE
.fi
.PP
Compilar com nome personalizado:
.PP
.nf
.RS
ambarc \-o meuapp programa.amb
.RE
.fi
.SH FILES
.TP
.I /usr/local/bin/ambarc
Wrapper de compilação completa
.TP
.I /usr/local/lib/ambarc/bin/ambarc\-real
Compilador real (gera apenas IR)
.TP
.I /usr/local/lib/ambarc/build.sh
Script de build avançado
.SH SEE ALSO
.BR llc (1),
.BR gcc (1)
.SH AUTHORS
AmbarC Team <ambarc@example.com>
EOF
gzip -9 "$PACKAGE_DIR/usr/share/man/man1/ambarc.1"

# 12. Construir pacote
echo "🏗️  Construindo pacote .deb..."
dpkg-deb --build --root-owner-group "$PACKAGE_DIR"

if [ $? -eq 0 ]; then
    echo ""
    echo "✅✅✅ PACOTE CRIADO COM SUCESSO! ✅✅✅"
    echo ""
    echo "📦 NOVAS FUNCIONALIDADES:"
    echo "   • ambarc agora gera executáveis completos"
    echo "   • Suporte a otimizações (-O0 a -Os)"
    echo "   • Opção para manter arquivos intermediários"
    echo "   • Script de build avançado incluído"
    echo ""
    echo "🚀 PARA INSTALAR:"
    echo "   sudo dpkg -i ambarc_1.0.0_amd64.deb"
    echo "   sudo apt-get install -f  # se necessário"
    echo ""
    echo "🧪 TESTE:"
    echo "   ambarc --version"
    echo "   ambarc /usr/local/share/ambarc/examples/hello.amb"
    echo "   ./hello"
else
    echo "❌ Erro ao criar pacote .deb"
    exit 1
fi