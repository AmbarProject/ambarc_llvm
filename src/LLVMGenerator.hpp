#ifndef LLVM_GENERATOR_HPP
#define LLVM_GENERATOR_HPP

#include "ast/nodes/ASTNode.hpp"
#include "ast/nodes/declarations/ProgramNode.hpp"
#include "ast/nodes/declarations/VarNode.hpp"
#include "ast/nodes/declarations/FunctionNode.hpp"
#include "ast/nodes/expressions/NumberNode.hpp"
#include "ast/nodes/expressions/BinaryNode.hpp"
#include "ast/nodes/expressions/BoolNode.hpp"
#include "ast/nodes/expressions/IdentifierNode.hpp"
#include "ast/nodes/expressions/StringNode.hpp"
#include "ast/nodes/expressions/CallNode.hpp"
#include "ast/nodes/expressions/UnaryNode.hpp"
#include "ast/nodes/statements/AssignNode.hpp"
#include "ast/nodes/statements/BlockNode.hpp"
#include "ast/nodes/statements/BreakNode.hpp" 
#include "ast/nodes/statements/ContinueNode.hpp" 
#include "ast/nodes/statements/ForNode.hpp"
#include "ast/nodes/statements/IfNode.hpp"
#include "ast/nodes/statements/ReturnNode.hpp"
#include "ast/nodes/statements/WhileNode.hpp"

// LLVM Core includes
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Constants.h>

// LLVM Pass includes - CORRIGIDOS para LLVM 18
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/Utils.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar/Reassociate.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#include <llvm/Transforms/Scalar/SROA.h>
#include <llvm/Transforms/Scalar/JumpThreading.h>
#include <llvm/Transforms/Scalar/CorrelatedValuePropagation.h>
#include <llvm/Transforms/Scalar/LICM.h>
#include <llvm/Transforms/Scalar/LoopUnrollPass.h>
#include <llvm/Transforms/Vectorize/LoopVectorize.h>
#include <llvm/Transforms/Vectorize/SLPVectorizer.h>
#include <llvm/Transforms/IPO/Inliner.h>
#include <llvm/Transforms/IPO/GlobalOpt.h>
#include <llvm/Transforms/IPO/GlobalDCE.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/IR/PassManager.h>

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector> 

namespace ambar {

// Enum para níveis de otimização
enum class OptimizationLevel {
    O0, // Sem otimizações (debug)
    O1, // Otimizações básicas
    O2, // Otimizações padrão
    O3, // Otimizações agressivas
    Os, // Otimizado para tamanho
    Oz  // Otimizado para tamanho máximo
};

class LLVMGenerator {
public:
    // Construtor com nível de otimização opcional
    LLVMGenerator(OptimizationLevel level = OptimizationLevel::O0);
    
    // Método para configurar nível de otimização
    void setOptimizationLevel(OptimizationLevel level);
    
    // Métodos auxiliares para diferentes níveis
    void enableBasicOptimizations();
    void enableAggressiveOptimizations();
    void enableSizeOptimizations();
    void disableOptimizations();
    
    void generate(std::unique_ptr<ASTNode>& astRoot);
    void dumpIR() const;
    void dumpIRToFile(const std::string& filename) const;
    
    std::unique_ptr<llvm::Module> module;
    void optimizeModule(); 
private:
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::IRBuilder<>> builder;
    llvm::Function* currentFunction = nullptr;
    
    // Nível de otimização
    OptimizationLevel optLevel;
    
    // Tabelas de símbolos
    std::unordered_map<std::string, llvm::Value*> namedValues;
    std::unordered_map<std::string, llvm::Type*> variableTypes;
    std::unordered_map<std::string, llvm::GlobalVariable*> globalValues;
    std::unordered_map<std::string, llvm::Type*> globalTypes;
    std::unordered_map<std::string, llvm::GlobalVariable*> stringLiterals;
        
    std::vector<llvm::BasicBlock*> breakBlocks;
    std::vector<llvm::BasicBlock*> continueBlocks;
    
    // Métodos de otimização
    void optimizeFunction(llvm::Function* func);
    
    // Métodos de geração
    void generateProgram(ProgramNode* node);
    void generateFunction(FunctionNode* node);
    llvm::Value* generateVariable(VarNode* node);
    llvm::Value* generateLocalVariable(VarNode* node);
    llvm::Value* generateGlobalVariable(VarNode* node);
    bool isGlobalContext() const { return currentFunction == nullptr; }
    void generateMainFunction(std::unique_ptr<ASTNode>& astRoot);
    
    llvm::Constant* evaluateConstantExpression(ASTNode* node);
    
    // Métodos de geração de expressões
    llvm::Value* generateBinaryExpr(BinaryNode* node);
    llvm::Value* generateUnaryExpr(UnaryNode* node);
    llvm::Value* generateNumber(NumberNode* node);
    llvm::Value* generateExpr(ASTNode* node);
    llvm::Value* generateBool(BoolNode* node);
    llvm::Value* generateIdentifier(IdentifierNode* node);
    llvm::Value* generateString(StringNode* node);
    llvm::Value* generateAssign(AssignNode* node);
    llvm::Value* generateBlock(BlockNode* node);
    llvm::Value* generateBreak(BreakNode* node); 
    llvm::Value* generateContinue(ContinueNode* node); 
    llvm::Value* generateFor(ForNode* node); 
    llvm::Value* generateIf(IfNode* node);
    llvm::Value* generateReturn(ReturnNode* node);
    llvm::Value* generateWhile(WhileNode* node);
    llvm::Value* generateCall(CallNode* node);
    
    void pushLoopBlocks(llvm::BasicBlock* breakBlock, llvm::BasicBlock* continueBlock); 
    void popLoopBlocks();
    
    // Utilitários
    llvm::Type* getLLVMType(const std::string& typeName);
    void handleGlobalVariables(ProgramNode* program);
    llvm::GlobalVariable* createStringLiteral(const std::string& value);
};

} // namespace ambar

#endif // LLVM_GENERATOR_HPP
