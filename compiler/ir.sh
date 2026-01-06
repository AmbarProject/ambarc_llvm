#!/bin/bash

# Diretórios base
SCRIPT_DIR=$(dirname "$(realpath "$0")")
ROOT_DIR="$SCRIPT_DIR/.."
SOURCE_DIR="$ROOT_DIR/src"
BIN_DIR="$SCRIPT_DIR/bin"

echo "🏗️  Compilando Ambar Compiler..."
echo "Diretório raiz: $ROOT_DIR"
echo "Diretório fonte: $SOURCE_DIR"
echo "Diretório binário: $BIN_DIR"

# Criar diretórios se não existirem
mkdir -p "$BIN_DIR"

# Gerar parser
echo "📝 Gerando parser..."
cd "$SOURCE_DIR/parser" || {
  echo "❌ Erro: Diretório parser não encontrado em $SOURCE_DIR/parser"
  exit 1
}

bison -d -o parser.tab.cc parser.y
if [ $? -ne 0 ]; then
  echo "❌ Erro ao gerar parser"
  exit 1
fi

# Gerar lexer
echo "📝 Gerando lexer..."
cd "$SOURCE_DIR/lexer" || {
  echo "❌ Erro: Diretório lexer não encontrado em $SOURCE_DIR/lexer"
  exit 1
}

flex -o lex.yy.c lexer.l
if [ $? -ne 0 ]; then
  echo "❌ Erro ao gerar lexer"
  exit 1
fi

# Voltar ao diretório do script
cd "$SCRIPT_DIR"

# Obter flags do LLVM
echo "⚙️  Obtendo flags do LLVM..."
LLVM_CXX_FLAGS=$(llvm-config --cxxflags)
LLVM_LD_FLAGS=$(llvm-config --ldflags --system-libs --libs core)
if [ $? -ne 0 ]; then
  echo "❌ Erro ao obter flags do LLVM"
  echo "Verifique se o LLVM está instalado: llvm-config --version"
  exit 1
fi

# Incluir diretórios necessários
INCLUDE_FLAGS="-I$ROOT_DIR/include -I$SOURCE_DIR -I$SOURCE_DIR/ast -I$SOURCE_DIR/ast/nodes"

# Lista de arquivos fonte (relativos ao SOURCE_DIR)
SOURCE_FILES=(
  "parser/parser.tab.cc"
  "lexer/lex.yy.c"
  "generator/LLVMGenerator.cpp"
  "generator/core/LLVMContextManager.cpp"
  "generator/core/IRBuilderFacade.cpp"
  "generator/core/TypeSystem.cpp"
  "main.cpp"
)

# Lista de objetos (serão gerados no BIN_DIR)
OBJECT_FILES=()

echo "🔨 Compilando arquivos..."

# Compilar cada arquivo
for src_file in "${SOURCE_FILES[@]}"; do
  # Caminho completo do arquivo fonte
  full_src_path="$SOURCE_DIR/$src_file"
  
  # Nome do objeto (substitui .cc/.c/.cpp por .o)
  obj_name=$(basename "$src_file")
  obj_name="${obj_name%.*}.o"
  obj_path="$BIN_DIR/$obj_name"
  
  # Verificar se o arquivo fonte existe
  if [ ! -f "$full_src_path" ]; then
    echo "❌ Erro: Arquivo fonte não encontrado: $full_src_path"
    exit 1
  fi
  
  echo "  Compilando $src_file -> $obj_name..."
  
  # Compilar
  g++ -std=c++17 -Wall -Wextra -g \
    $INCLUDE_FLAGS \
    -c "$full_src_path" \
    $LLVM_CXX_FLAGS -fexceptions -o "$obj_path"
    
  if [ $? -ne 0 ]; then
    echo "❌ Erro na compilação de $src_file"
    exit 1
  fi
  
  OBJECT_FILES+=("$obj_path")
done

echo "✅ Compilação dos objetos concluída!"

# Linkar todos os objetos
echo "🔗 Linkando executável..."
EXECUTABLE_PATH="$BIN_DIR/ambar"

g++ -std=c++17 -Wall -Wextra -g \
  "${OBJECT_FILES[@]}" \
  $LLVM_LD_FLAGS -fexceptions -o "$EXECUTABLE_PATH"

if [ $? -eq 0 ]; then
  echo "✅ Executável criado: $EXECUTABLE_PATH"
  
  # Tornar executável
  chmod +x "$EXECUTABLE_PATH"
  
  # Criar link simbólico no diretório raiz se desejado
  if [ ! -f "../ambar" ]; then
    ln -sf "$EXECUTABLE_PATH" "$ROOT_DIR/ambar"
    echo "🔗 Link simbólico criado: $ROOT_DIR/ambar -> $EXECUTABLE_PATH"
  fi
  
  echo ""
  echo "🎉 Compilação bem-sucedida!"
  echo "   Compilador disponível em:"
  echo "   - $EXECUTABLE_PATH"
  echo "   - $ROOT_DIR/ambar (link simbólico)"
else
  echo "❌ Erro no linking"
  exit 1
fi