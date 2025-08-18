#ifndef SOURCE_LOCATION_HPP
#define SOURCE_LOCATION_HPP

#include <string>
#include <ostream>

namespace ambar {

class SourceLocation {
public:
    // Construtor padrão
    SourceLocation() 
        : line(0), column(0), length(0), filename("") {}
    
    // Construtor com parâmetros
    SourceLocation(unsigned line, unsigned column, unsigned length, std::string filename)
        : line(line), column(column), length(length), filename(std::move(filename)) {}
    
    // Getters
    unsigned getLine() const { return line; }
    unsigned getColumn() const { return column; }
    unsigned getLength() const { return length; }
    const std::string& getFilename() const { return filename; }
    
    // Setters
    void setLine(unsigned l) { line = l; }
    void setColumn(unsigned c) { column = c; }
    void setLength(unsigned len) { length = len; }
    void setFilename(std::string fname) { filename = std::move(fname); }
    
    // Método para imprimir a localização
    void print(std::ostream& os) const {
        os << filename << ":" << line << ":" << column;
    }
    
    // Operador de saída para facilitar a impressão
    friend std::ostream& operator<<(std::ostream& os, const SourceLocation& loc) {
        loc.print(os);
        return os;
    }
    
    // Verifica se a localização é válida
    bool isValid() const {
        return line > 0 && column > 0;
    }

private:
    unsigned line;      // Número da linha (1-based)
    unsigned column;    // Número da coluna (1-based)
    unsigned length;    // Comprimento do token/nó
    std::string filename; // Nome do arquivo fonte
};

} // namespace ambar

#endif // SOURCE_LOCATION_HPP