# Variáveis
CXX = g++
CXXFLAGS = -std=c++17 -Isrc `llvm-config --cxxflags`
LDFLAGS = `llvm-config --ldflags --system-libs --libs core`

# Arquivos fonte
SRC_DIR = src
LEX_SRC = $(SRC_DIR)/lexer.l
YACC_SRC = $(SRC_DIR)/parser.y
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/ast.hpp $(SRC_DIR)/codegen.hpp
OBJS = $(SRC_DIR)/lexer.o $(SRC_DIR)/parser.o $(SRC_DIR)/main.o
TARGET = minillvm

# Regras padrão
all: $(TARGET)

# Regra para gerar os arquivos do parser
$(SRC_DIR)/parser.cpp $(SRC_DIR)/parser.hpp: $(YACC_SRC)
	bison -o $(SRC_DIR)/parser.cpp -d $(YACC_SRC)

# Regra para gerar o lexer
$(SRC_DIR)/lexer.cpp: $(LEX_SRC) $(SRC_DIR)/parser.hpp
	flex -o $(SRC_DIR)/lexer.cpp $(LEX_SRC)

# Regras para compilar objetos
$(SRC_DIR)/lexer.o: $(SRC_DIR)/lexer.cpp $(SRC_DIR)/parser.hpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/lexer.cpp -o $(SRC_DIR)/lexer.o

$(SRC_DIR)/parser.o: $(SRC_DIR)/parser.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/parser.cpp -o $(SRC_DIR)/parser.o

$(SRC_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/parser.hpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(SRC_DIR)/main.o

# Regra para compilar o executável
$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

# Limpeza
clean:
	rm -f $(TARGET) $(OBJS) $(SRC_DIR)/lexer.cpp $(SRC_DIR)/parser.cpp $(SRC_DIR)/parser.hpp

.PHONY: all clean