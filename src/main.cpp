#include <iostream>
#include <cstdio>
#include <fstream>
#include <memory>
#include <string>

#include "ast/nodes/ASTNode.hpp"
#include "generator/LLVMGenerator.hpp"

extern "C" {
    int yyparse(void);
    extern FILE* yyin;
}

// Correção: usar o namespace ambar e a declaração correta
namespace ambar {
    extern std::unique_ptr<ASTNode> astRoot;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " [-O0|-O1|-O2|-O3|-Os|-Oz] <arquivo.amb>" << std::endl;
        return 1;
    }

    std::string inputFile;
    ambar::OptimizationLevel optLevel = ambar::OptimizationLevel::O0;

    // Parse dos argumentos
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-O0") {
            optLevel = ambar::OptimizationLevel::O0;
        } else if (arg == "-O1") {
            optLevel = ambar::OptimizationLevel::O1;
        } else if (arg == "-O2") {
            optLevel = ambar::OptimizationLevel::O2;
        } else if (arg == "-O3") {
            optLevel = ambar::OptimizationLevel::O3;
        } else if (arg == "-Os") {
            optLevel = ambar::OptimizationLevel::Os;
        } else if (arg == "-Oz") {
            optLevel = ambar::OptimizationLevel::Oz;
        } else if (arg.find(".amb") != std::string::npos) {
            inputFile = arg;
        }
    }

    if (inputFile.empty()) {
        std::cerr << "Erro: Nenhum arquivo .amb especificado" << std::endl;
        return 1;
    }

    yyin = fopen(inputFile.c_str(), "r");
    if (!yyin) {
        std::cerr << "Erro ao abrir o arquivo: " << inputFile << std::endl;
        return 1;
    }

    int result = yyparse();

    if (result == 0) {

        if (ambar::astRoot) {

            try {
                // Criar gerador LLVM com nível de otimização
                ambar::LLVMGenerator generator(optLevel);

                // Gera o IR
                generator.generate(ambar::astRoot);

                // Otimiza
                generator.optimizeModule();

                // generator.analyzeAndReportOptimizationProblems();

                // Dump IR no terminal
                // generator.dumpIR();

                // Salvar em arquivo .ll
                std::string x = inputFile.substr(0, inputFile.find_last_of('.'));
                std::string outputFile = x + ".ll";
                generator.dumpIRToFile(outputFile);

            } catch (const std::exception &e) {
                std::cerr << "❌ Erro durante a geração de código: " << e.what() << std::endl;
                return 1;
            }

        } else {
            std::cerr << "❌ AST não foi construída." << std::endl;
            return 1;
        }
    } else {
        return result;
    }

    if (yyin && yyin != stdin) {
        fclose(yyin);
    }

    return 0;
}

