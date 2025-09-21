#include <iostream>
#include <cstdio>
#include <fstream>
#include <memory>
#include "ast/nodes/ASTNode.hpp"
#include "LLVMGenerator.hpp"

extern "C" {
    int yyparse(void);
    extern FILE* yyin;
}

// Correção: usar o namespace ambar e a declaração correta
namespace ambar {
    extern std::unique_ptr<ASTNode> astRoot;
}

int main(int argc, char **argv) {
    // Imprime ASCII Art
    std::cout << R"(



              @@                                                                                                            
              @@                                                                                                            
              @@@                                                                                                           
           @@  @@@                                                                                                          
          @@@@  @@@                                                                                                         
           @@@@  @@@                                                                                                        
            @@@@  @@@                                                                                                       
             @@@@  @@@           @@@@@@       @@@@@@  @@@@@@@@@@@@         @@@@@@@       @@@@@@@@@@@        @@@@@@@@@       
              @@@@  @@@          @@@@@@@     @@@@@@@  @@@@@@@@@@@@@@      @@@@@@@@       @@@@@@@@@@@@@    @@@@@@@@@@@@@     
               @@@   @@@         @@@@@@@     @@@@@@@  @@@@@    @@@@@      @@@@@@@@@      @@@@    @@@@@   @@@@@     @@@@@    
                @@@   @@@        @@@@@@@@   @@@@@@@@  @@@@@   @@@@@      @@@@@ @@@@@     @@@@    @@@@@  @@@@@               
    @@@@@@@@@@@@@@@@   @@@       @@@@ @@@@ @@@@ @@@@  @@@@@@@@@@@@      @@@@@  @@@@@     @@@@@@@@@@@@@  @@@@@               
   @@@@@@@@@@@@@@@@@@  @@@@      @@@@ @@@@@@@@  @@@@  @@@@@    @@@@@    @@@@@@@@@@@@@    @@@@@@@@@@@    @@@@@               
                        @@@@     @@@@  @@@@@@@  @@@@  @@@@@    @@@@@@  @@@@@@@@@@@@@@@   @@@@  @@@@@     @@@@@     @@@@@    
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@    @@@@   @@@@@   @@@@  @@@@@@@@@@@@@@  @@@@@      @@@@@   @@@@   @@@@@     @@@@@@@@@@@@@     
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@    @@@    @@@@  @@@@@@@@@@@@    @@@@@       @@@@@  @@@@    @@@@@      @@@@@@@@@       
                                                                                                                            
                                                                                                                            
                                                                                                                            
Welcome to Ambar Compiler (v0.1).

Type your code below:

)" << std::endl;

   std::string inputFile;
    
    if (argc > 1) {
        inputFile = argv[1];
        yyin = fopen(inputFile.c_str(), "r");
        if (!yyin) {
            std::cerr << "Erro ao abrir o arquivo: " << inputFile << std::endl;
            return 1;
        }
        std::cout << "Compilando: " << inputFile << std::endl;
    } else {
        std::cout << "Digite seu código Ambar (Ctrl+D para finalizar):\n\n" << std::endl;
        yyin = stdin;
    }

    int result = yyparse();
    
    if (result == 0) {
        std::cout << "✅ Análise sintática concluída com sucesso!" << std::endl;

        if (ambar::astRoot) {
            std::cout << "✅ AST construída com sucesso!" << std::endl;
            
            try {
                // Criar gerador LLVM
                ambar::LLVMGenerator generator;
                
                // Gerar código LLVM IR a partir da AST
                generator.generate(ambar::astRoot);
                
                std::cout << "✅ Geração de código LLVM IR concluída!" << std::endl;
                
                // Exibir o código IR gerado
                std::cout << "\n=== CÓDIGO LLVM IR GERADO ===" << std::endl;
                generator.dumpIR();
                
                // Salvar em arquivo se foi passado um arquivo de entrada
                if (!inputFile.empty()) {
                    std::string x = inputFile.erase(inputFile.length() - 4);
                    std::string outputFile = x + ".ll";
                    std::error_code EC;
                    llvm::raw_fd_ostream dest(outputFile, EC);
                    if (EC) {
                        std::cerr << "❌ Erro ao salvar IR: " << EC.message() << std::endl;
                        return 1;
                    }
                    generator.dumpIRToFile(outputFile);
                    std::cout << "✅ IR salvo em: " << outputFile << std::endl;
                }
                
            } catch (const std::exception& e) {
                std::cerr << "❌ Erro durante a geração de código: " << e.what() << std::endl;
                return 1;
            }
            
        } else {
            std::cerr << "❌ AST não foi construída." << std::endl;
            return 1;
        }

    } else {
        std::cerr << "❌ Erro na análise sintática." << std::endl;
        return result;
    }

    if (yyin && yyin != stdin) {
        fclose(yyin);
    }

    return 0;
}
