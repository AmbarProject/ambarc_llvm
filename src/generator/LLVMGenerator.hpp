#ifndef LLVM_GENERATOR_HPP
#define LLVM_GENERATOR_HPP

// ============================================================================
// INCLUSÕES DA ÁRVORE DE SINTAXE ABSTRATA (AST)
// ============================================================================

// Nós de declaração (estruturas de alto nível)
#include "ast/nodes/ASTNode.hpp"
#include "ast/nodes/declarations/ProgramNode.hpp"
#include "ast/nodes/declarations/VarNode.hpp"
#include "ast/nodes/declarations/FunctionNode.hpp"

// Nós de expressões (cálculos e valores)
#include "ast/nodes/expressions/NumberNode.hpp"
#include "ast/nodes/expressions/BinaryNode.hpp"
#include "ast/nodes/expressions/BoolNode.hpp"
#include "ast/nodes/expressions/IdentifierNode.hpp"
#include "ast/nodes/expressions/StringNode.hpp"
#include "ast/nodes/expressions/CallNode.hpp"
#include "ast/nodes/expressions/ArrayNode.hpp"
#include "ast/nodes/expressions/ArrayAccessNode.hpp"
#include "ast/nodes/expressions/UnaryNode.hpp"

// Nós de statements (controle de fluxo)
#include "ast/nodes/statements/AssignNode.hpp"
#include "ast/nodes/statements/BlockNode.hpp"
#include "ast/nodes/statements/BreakNode.hpp" 
#include "ast/nodes/statements/ContinueNode.hpp" 
#include "ast/nodes/statements/ForNode.hpp"
#include "ast/nodes/statements/IfNode.hpp"
#include "ast/nodes/statements/ReturnNode.hpp"
#include "ast/nodes/statements/WhileNode.hpp"

// ============================================================================
// INCLUSÕES DAS NOVAS CLASSES MODULARIZADAS
// ============================================================================

#include "./core/LLVMContextManager.hpp"
#include "./core/IRBuilderFacade.hpp"
#include "./core/TypeSystem.hpp"

// ============================================================================
// INCLUSÕES DO LLVM CORE (INFRAESTRUTURA BÁSICA)
// ============================================================================

#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Constants.h>

// ============================================================================
// INCLUSÕES DO SISTEMA DE PASSES DO LLVM (OTIMIZAÇÕES)
// ============================================================================

// Gerenciadores de passes (compatíveis com LLVM 18)
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/IR/PassManager.h>

// Passes de transformação escalar (otimizações em instruções)
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/Reassociate.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#include <llvm/Transforms/Scalar/SROA.h>
#include <llvm/Transforms/Scalar/JumpThreading.h>
#include <llvm/Transforms/Scalar/CorrelatedValuePropagation.h>
#include <llvm/Transforms/Scalar/LICM.h>
#include <llvm/Transforms/Scalar/LoopUnrollPass.h>

// Passes de transformação vetorial (otimizações SIMD)
#include <llvm/Transforms/Vectorize/LoopVectorize.h>
#include <llvm/Transforms/Vectorize/SLPVectorizer.h>

// Passes IPO (Inter-Procedural Optimizations)
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/IPO/Inliner.h>
#include <llvm/Transforms/IPO/GlobalOpt.h>
#include <llvm/Transforms/IPO/GlobalDCE.h>

// Passes de combinação de instruções
#include <llvm/Transforms/InstCombine/InstCombine.h>

// Passes utilitários
#include <llvm/Transforms/Utils.h>

// ============================================================================
// INCLUSÕES DO SISTEMA DE ANÁLISE DO LLVM
// ============================================================================

#include <llvm/Analysis/CFG.h>                 // Análise de fluxo de controle
#include <llvm/Analysis/LoopInfo.h>           // Informações sobre loops
#include <llvm/Analysis/ScalarEvolution.h>    // Análise de evolução escalar
#include <llvm/Analysis/BranchProbabilityInfo.h> // Probabilidades de branch
#include <llvm/Analysis/BlockFrequencyInfo.h> // Frequência de execução de blocos
#include <llvm/Analysis/DominanceFrontier.h>  // Análise de dominância
#include <llvm/Analysis/PostDominators.h>     // Análise de pós-dominância

// ============================================================================
// INCLUSÕES DA BIBLIOTECA PADRÃO C++
// ============================================================================

#include <sstream>       // Para manipulação de strings
#include <map>           // Para mapas ordenados
#include <memory>        // Para ponteiros inteligentes
#include <string>        // Para strings
#include <unordered_map> // Para tabelas hash
#include <vector>        // Para arrays dinâmicos

// ============================================================================
// NAMESPACE PRINCIPAL DO COMPILADOR
// ============================================================================

namespace ambar {

// ============================================================================
// ENUMERAÇÃO DE NÍVEIS DE OTIMIZAÇÃO
// ============================================================================

/**
 * @brief Define os níveis de otimização disponíveis no compilador
 * 
 * Cada nível representa um conjunto diferente de transformações e análises
 * que são aplicadas ao código gerado para melhorar performance ou tamanho.
 */
enum class OptimizationLevel {
    O0, ///< Nenhuma otimização - ideal para debug e desenvolvimento
    O1, ///< Otimizações básicas - bom equilíbrio entre tempo e performance
    O2, ///< Otimizações padrão - recomendado para builds de release
    O3, ///< Otimizações agressivas - máxima performance (pode aumentar tamanho)
    Os, ///< Otimizado para tamanho - reduz tamanho do código
    Oz  ///< Otimizado para tamanho máximo - prioridade máxima em tamanho
};

// ============================================================================
// CLASSE DE ANÁLISE DE OTIMIZAÇÃO
// ============================================================================

/**
 * @brief Analisa o código LLVM gerado e identifica oportunidades de otimização
 * 
 * Esta classe percorre o código LLVM e aplica heurísticas para detectar
 * padrões que podem ser otimizados, gerando relatórios para o desenvolvedor.
 */
class OptimizationAnalyzer {
public:
    /**
     * @brief Estrutura que representa um nó problemático detectado na análise
     * 
     * Contém todas as informações necessárias para identificar e descrever
     * um ponto específico no código que pode ser otimizado.
     */
    struct ProblematicNode {
        std::string functionName;   ///< Nome da função onde o problema foi encontrado
        std::string blockName;      ///< Nome do bloco básico problemático
        std::string problemType;    ///< Categoria do problema (ex: "computação redundante")
        std::string description;    ///< Descrição detalhada do problema
        int severity;               ///< Severidade de 1-10 (10 = mais crítico)
        llvm::BasicBlock* block;    ///< Ponteiro para o bloco básico LLVM
    };

    /**
     * @brief Constrói um analisador para um módulo LLVM específico
     * @param module Referência para o módulo LLVM a ser analisado
     */
    OptimizationAnalyzer(llvm::Module& module) : module(module) {}
    
    // ========================================================================
    // INTERFACE PÚBLICA DA ANÁLISE
    // ========================================================================
    
    /**
     * @brief Analisa todo o módulo em busca de problemas de otimização
     * @return Vetor com todos os nós problemáticos encontrados
     */
    std::vector<ProblematicNode> analyzeOptimizationProblems();
    
    /**
     * @brief Imprime um relatório formatado dos problemas encontrados
     * @param problems Vetor de nós problemáticos a serem exibidos
     */
    void printProblematicNodes(const std::vector<ProblematicNode>& problems);

private:
    llvm::Module& module; ///< Referência para o módulo LLMW sendo analisado
    
    // ========================================================================
    // MÉTODOS INTERNOS DE ANÁLISE
    // ========================================================================
    
    /**
     * @brief Analisa uma função individual em busca de problemas
     * @param F Função LLVM a ser analisada
     * @param problems Vetor onde problemas serão acumulados
     */
    void analyzeFunction(llvm::Function& F, std::vector<ProblematicNode>& problems);
    
    /**
     * @brief Analisa um bloco básico individual
     * @param BB Bloco básico a ser analisado
     * @param F Função à qual o bloco pertence
     * @param problems Vetor para acumular problemas encontrados
     */
    void analyzeBasicBlock(llvm::BasicBlock& BB, llvm::Function& F, 
                          std::vector<ProblematicNode>& problems);
    
    /**
     * @brief Verifica se o bloco contém computações redundantes
     * @param BB Bloco básico a ser verificado
     * @return true se houver computações redundantes
     */
    bool hasRedundantComputations(llvm::BasicBlock& BB);
    
    /**
     * @brief Verifica se o bloco contém operações computacionalmente caras
     * @param BB Bloco básico a ser verificado
     * @return true se houver operações caras
     */
    bool hasExpensiveOperations(llvm::BasicBlock& BB);
    
    /**
     * @brief Verifica se o bloco tem padrões de branch ineficientes
     * @param BB Bloco básico a ser verificado
     * @return true se houver padrões de branch problemáticos
     */
    bool hasPoorBranchPattern(llvm::BasicBlock& BB);
    
    /**
     * @brief Verifica se o bloco tem ineficiências de acesso à memória
     * @param BB Bloco básico a ser verificado
     * @return true se houver ineficiências de memória
     */
    bool hasMemoryInefficiencies(llvm::BasicBlock& BB);
    
    /**
     * @brief Verifica se o bloco tem ineficiências relacionadas a loops
     * @param BB Bloco básico a ser verificado
     * @param F Função à qual o bloco pertence
     * @return true se houver ineficiências em loops
     */
    bool hasLoopInefficiencies(llvm::BasicBlock& BB, llvm::Function& F);
    
    /**
     * @brief Gera uma descrição textual de um bloco básico
     * @param BB Bloco básico a ser descrito
     * @return String com descrição do bloco
     */
    std::string getBlockDescription(llvm::BasicBlock& BB);
};

// ============================================================================
// CLASSE PRINCIPAL DO GERADOR LLVM
// ============================================================================

/**
 * @brief Gerador de código LLVM para a linguagem Ambar
 * 
 * Esta classe é responsável por transformar a Árvore de Sintaxe Abstrata (AST)
 * em código LLVM, incluindo otimizações, análise de performance e geração
 * de relatórios de qualidade de código.
 */
class LLVMGenerator {
public:
    // ========================================================================
    // CONSTRUTOR E CONFIGURAÇÃO
    // ========================================================================
    
    /**
     * @brief Constrói um gerador LLVM com nível de otimização configurável
     * @param level Nível inicial de otimização (padrão: O0 - sem otimizações)
     */
    LLVMGenerator(OptimizationLevel level = OptimizationLevel::O0);
    
    /**
     * @brief Configura o nível de otimização dinamicamente
     * @param level Novo nível de otimização a ser aplicado
     */
    void setOptimizationLevel(OptimizationLevel level);
    
    // ========================================================================
    // MÉTODOS AUXILIARES PARA CONFIGURAÇÃO RÁPIDA
    // ========================================================================
    
    /** @brief Ativa otimizações básicas (equivalente a -O1) */
    void enableBasicOptimizations();
    
    /** @brief Ativa otimizações agressivas (equivalente a -O3) */
    void enableAggressiveOptimizations();
    
    /** @brief Ativa otimizações para reduzir tamanho do código */
    void enableSizeOptimizations();
    
    /** @brief Desativa todas as otimizações (equivalente a -O0) */
    void disableOptimizations();
    
    // ========================================================================
    // INTERFACE PRINCIPAL DE GERAÇÃO DE CÓDIGO
    // ========================================================================
    
    /**
     * @brief Gera código LLVM a partir da raiz da AST
     * @param astRoot Ponteiro único para a raiz da Árvore de Sintaxe Abstrata
     */
    void generate(std::unique_ptr<ASTNode>& astRoot);
    
    // ========================================================================
    // UTILITÁRIOS DE SAÍDA E DEBUG
    // ========================================================================
    
    /**
     * @brief Imprime o código LLVM gerado na saída de erro padrão
     */
    void dumpIR() const;
    
    /**
     * @brief Salva o código LLVM gerado em um arquivo
     * @param filename Caminho do arquivo onde o código será salvo
     */
    void dumpIRToFile(const std::string& filename) const;
    
    // ========================================================================
    // OTIMIZAÇÃO E ANÁLISE
    // ========================================================================
    
    /**
     * @brief Aplica otimizações em todo o módulo LLVM
     * 
     * As otimizações aplicadas dependem do nível configurado
     * (O1, O2, O3, Os, Oz). Nível O0 não aplica otimizações.
     */
    void optimizeModule();
    
    /**
     * @brief Executa análise de otimização e gera relatório
     * 
     * Identifica padrões problemáticos no código gerado e sugere
     * melhorias para o desenvolvedor.
     */
    void analyzeAndReportOptimizationProblems();
    
    // ========================================================================
    // ACESSO AOS COMPONENTES INTERNOS
    // ========================================================================
    
    /**
     * @brief Obtém o gerenciador de contexto
     * @return Referência para o gerenciador de contexto
     */
    LLVMContextManager& getContextManager() { return *contextManager; }
    
    /**
     * @brief Obtém o construtor de IR
     * @return Referência para o construtor de IR
     */
    IRBuilderFacade& getIRBuilder() { return *irBuilder; }
    
    /**
     * @brief Obtém o sistema de tipos
     * @return Referência para o sistema de tipos
     */
    TypeSystem& getTypeSystem() { return *typeSystem; }
    
    /**
     * @brief Obtém o módulo LLVM
     * @return Referência para o módulo LLVM
     */
    llvm::Module& getModule() { return contextManager->getModule(); }

private:
    // ========================================================================
    // COMPONENTES MODULARIZADOS
    // ========================================================================
    
    std::unique_ptr<LLVMContextManager> contextManager; ///< Gerencia contexto e módulo LLVM
    std::unique_ptr<IRBuilderFacade> irBuilder;         ///< Fachada para construção de IR
    std::unique_ptr<TypeSystem> typeSystem;             ///< Sistema de tipos
    
    llvm::Function* currentFunction = nullptr; ///< Função sendo gerada atualmente
    llvm::Function* printfFunc = nullptr;      ///< Referência para função printf
    
    OptimizationLevel optLevel; ///< Nível atual de otimização
    
    // ========================================================================
    // TABELAS DE SÍMBOLOS (ESCOPO)
    // ========================================================================
    
    std::unordered_map<std::string, llvm::Value*> namedValues;  ///< Variáveis locais
    std::unordered_map<std::string, llvm::Type*> variableTypes; ///< Tipos das locais
    
    std::unordered_map<std::string, llvm::GlobalVariable*> globalValues; ///< Globais
    std::unordered_map<std::string, llvm::Type*> globalTypes;           ///< Tipos globais
    
    std::unordered_map<std::string, llvm::GlobalVariable*> stringLiterals; ///< Cache de strings
    
    // ========================================================================
    // CONTROLE DE FLUXO (LOOPS)
    // ========================================================================
    
    std::vector<llvm::BasicBlock*> breakBlocks;    ///< Pilha de destinos de break
    std::vector<llvm::BasicBlock*> continueBlocks; ///< Pilha de destinos de continue
    
    // ========================================================================
    // FUNÇÕES BUILT-IN DO SISTEMA
    // ========================================================================
    
    /**
     * @brief Declara a função printf no módulo LLVM
     * 
     * printf é necessária para operações de impressão na linguagem.
     * Assinatura: int printf(const char* format, ...)
     */
    void declarePrintfFunction();
    
    /**
     * @brief Gera chamada para a função print da linguagem
     * @param node Nó da AST representando a chamada print(...)
     * @return Valor LLVM resultante da chamada (ou nullptr se erro)
     */
    llvm::Value* generatePrintCall(CallNode* node);
    
    // ========================================================================
    // GERADORES PARA DECLARAÇÕES (ALTO NÍVEL)
    // ========================================================================
    
    /**
     * @brief Gera código para um programa completo
     * @param node Nó ProgramNode representando o programa
     */
    void generateProgram(ProgramNode* node);
    
    /**
     * @brief Gera código para uma função
     * @param node Nó FunctionNode representando a função
     */
    void generateFunction(FunctionNode* node);
    
    /**
     * @brief Gera código para declaração de variável
     * @param node Nó VarNode representando a variável
     * @return Valor LLVM alocado para a variável
     */
    llvm::Value* generateVariable(VarNode* node);
    
    /**
     * @brief Gera código para variável local
     * @param node Nó VarNode representando a variável local
     * @return Valor LLVM alocado na stack
     */
    llvm::Value* generateLocalVariable(VarNode* node);
    
    /**
     * @brief Gera código para variável global
     * @param node Nó VarNode representando a variável global
     * @return Variável global LLVM
     */
    llvm::Value* generateGlobalVariable(VarNode* node);
    
    /**
     * @brief Processa todas as variáveis globais de um programa
     * @param program Nó ProgramNode contendo as declarações
     */
    void handleGlobalVariables(ProgramNode* program);
    
    // ========================================================================
    // GERADORES PARA ARRAYS
    // ========================================================================
    
    /**
     * @brief Gera código para literal de array
     * @param node Nó ArrayNode representando o array literal
     * @return Valor LLVM para o array alocado
     */
    llvm::Value* generateArrayLiteral(ArrayNode* node);
    
    /**
     * @brief Gera código para acesso a elemento de array
     * @param node Nó ArrayAccessNode representando o acesso
     * @return Valor LLVM do elemento acessado
     */
    llvm::Value* generateArrayAccess(ArrayAccessNode* node);
    
    /**
     * @brief Gera código para atribuição a elemento de array
     * @param node Nó AssignNode representando a atribuição
     * @return Valor LLVM atribuído
     */
    llvm::Value* generateArrayAssignment(AssignNode* node);
    
    // ========================================================================
    // GERADORES PARA EXPRESSÕES (VALORES)
    // ========================================================================
    
    /**
     * @brief Gera código para expressão binária
     * @param node Nó BinaryNode representando a operação binária
     * @return Valor LLVM resultante da operação
     */
    llvm::Value* generateBinaryExpr(BinaryNode* node);
    
    /**
     * @brief Gera código para expressão unária
     * @param node Nó UnaryNode representando a operação unária
     * @return Valor LLVM resultante da operação
     */
    llvm::Value* generateUnaryExpr(UnaryNode* node);
    
    /**
     * @brief Gera código para literal numérico
     * @param node Nó NumberNode representando o número
     * @return Valor LLVM constante do número
     */
    llvm::Value* generateNumber(NumberNode* node);
    
    /**
     * @brief Gera código para literal booleano
     * @param node Nó BoolNode representando o booleano
     * @return Valor LLVM constante do booleano
     */
    llvm::Value* generateBool(BoolNode* node);
    
    /**
     * @brief Gera código para identificador (variável)
     * @param node Nó IdentifierNode representando o identificador
     * @return Valor LLVM da variável (carregado da memória)
     */
    llvm::Value* generateIdentifier(IdentifierNode* node);
    
    /**
     * @brief Gera código para literal de string
     * @param node Nó StringNode representando a string
     * @return Ponteiro LLVM para a string constante
     */
    llvm::Value* generateString(StringNode* node);
    
    /**
     * @brief Gera código para chamada de função
     * @param node Nó CallNode representando a chamada
     * @return Valor LLVM retornado pela função (ou nullptr para void)
     */
    llvm::Value* generateCall(CallNode* node);
    
    // ========================================================================
    // GERADORES PARA STATEMENTS (CONTROLE DE FLUXO)
    // ========================================================================
    
    /**
     * @brief Gera código para atribuição
     * @param node Nó AssignNode representando a atribuição
     * @return Valor LLVM atribuído
     */
    llvm::Value* generateAssign(AssignNode* node);
    
    /**
     * @brief Gera código para bloco de código
     * @param node Nó BlockNode representando o bloco
     * @return Último valor LLVM gerado no bloco
     */
    llvm::Value* generateBlock(BlockNode* node);
    
    /**
     * @brief Gera código para statement break
     * @param node Nó BreakNode representando o break
     * @return Valor dummy (break não retorna valor útil)
     */
    llvm::Value* generateBreak(BreakNode* node);
    
    /**
     * @brief Gera código para statement continue
     * @param node Nó ContinueNode representando o continue
     * @return Valor dummy (continue não retorna valor útil)
     */
    llvm::Value* generateContinue(ContinueNode* node);
    
    /**
     * @brief Gera código para loop for
     * @param node Nó ForNode representando o loop
     * @return Valor dummy (for não retorna valor útil)
     */
    llvm::Value* generateFor(ForNode* node);
    
    /**
     * @brief Gera código para statement if/else
     * @param node Nó IfNode representando a condicional
     * @return Valor LLVM resultante da condicional (usando PHI node)
     */
    llvm::Value* generateIf(IfNode* node);
    
    /**
     * @brief Gera código para statement return
     * @param node Nó ReturnNode representando o retorno
     * @return Valor LLVM retornado (ou nullptr para void)
     */
    llvm::Value* generateReturn(ReturnNode* node);
    
    /**
     * @brief Gera código para loop while
     * @param node Nó WhileNode representando o loop
     * @return Valor dummy (while não retorna valor útil)
     */
    llvm::Value* generateWhile(WhileNode* node);
    
    // ========================================================================
    // GERADOR GENÉRICO DE EXPRESSÕES
    // ========================================================================
    
    /**
     * @brief Roteador genérico que delega para o gerador apropriado
     * @param node Nó AST genérico a ser processado
     * @return Valor LLVM resultante da expressão
     */
    llvm::Value* generateExpr(ASTNode* node);
    
    // ========================================================================
    // CONTROLE DE ESCOPO DE LOOPS
    // ========================================================================
    
    /**
     * @brief Empilha blocos de break/continue para loop atual
     * @param breakBlock Bloco destino para instruções break
     * @param continueBlock Bloco destino para instruções continue
     */
    void pushLoopBlocks(llvm::BasicBlock* breakBlock, llvm::BasicBlock* continueBlock);
    
    /** @brief Desempilha blocos de break/continue (sai do loop) */
    void popLoopBlocks();
    
    // ========================================================================
    // FUNÇÃO MAIN (IMPLÍCITA)
    // ========================================================================
    
    /**
     * @brief Gera função main implícita se não for fornecida explicitamente
     * @param astRoot Raiz da AST para extrair statements globais
     */
    void generateMainFunction(std::unique_ptr<ASTNode>& astRoot);
    
    /**
     * @brief Verifica se estamos no contexto global (fora de função)
     * @return true se estivermos gerando código no escopo global
     */
    bool isGlobalContext() const { return currentFunction == nullptr; }
    
    // ========================================================================
    // UTILITÁRIOS DE TIPOS E CONSTANTES
    // ========================================================================
    
    /**
     * @brief Mapeia nome de tipo da linguagem para tipo LLVM
     * @param typeName Nome do tipo na linguagem Ambar
     * @return Tipo LLVM correspondente (ou nullptr se desconhecido)
     */
    llvm::Type* getLLVMType(const std::string& typeName);
    
    /**
     * @brief Extrai tipo do elemento de um tipo de array
     * @param arrayType Tipo LLVM do array (ponteiro ou array)
     * @return Tipo LLVM do elemento (ou nullptr se não for array)
     */
    llvm::Type* getArrayElementType(llvm::Type* arrayType);
    
    /**
     * @brief Avalia expressão constante em tempo de compilação
     * @param node Nó AST representando a expressão constante
     * @return Constante LLVM resultante (ou nullptr se não for constante)
     */
    llvm::Constant* evaluateConstantExpression(ASTNode* node);
    
    /**
     * @brief Cria ou reusa uma string literal no módulo
     * @param value Conteúdo da string
     * @return Variável global LLVM contendo a string
     */
    llvm::GlobalVariable* createStringLiteral(const std::string& value);
    
    // ========================================================================
    // OTIMIZAÇÕES (IMPLEMENTAÇÃO INTERNA)
    // ========================================================================
    
    /**
     * @brief Otimiza uma função individual
     * @param func Ponteiro para a função LLVM a ser otimizada
     */
    void optimizeFunction(llvm::Function* func);
    
    // ========================================================================
    // ANÁLISE AVANÇADA (IMPLEMENTAÇÃO INTERNA)
    // ========================================================================
    
    /**
     * @brief Executa análise avançada de grafos de controle
     * 
     * Complementa a análise básica com heurísticas mais sofisticadas
     * para identificar padrões complexos de ineficiência.
     */
    void performAdvancedOptimizationAnalysis();
};

} // namespace ambar

#endif // LLVM_GENERATOR_HPP