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
#include "ast/nodes/expressions/UnaryNode.hpp"
#include "ast/nodes/statements/AssignNode.hpp"
#include "ast/nodes/statements/BlockNode.hpp"
#include "ast/nodes/statements/BreakNode.hpp" 
#include "ast/nodes/statements/ContinueNode.hpp" 
#include "ast/nodes/statements/ForNode.hpp"

#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Constants.h>

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector> 

namespace ambar {

class LLVMGenerator {
public:
    LLVMGenerator();
    void generate(std::unique_ptr<ASTNode>& astRoot);
    void dumpIR() const;

private:
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::Module> module;
    std::unique_ptr<llvm::IRBuilder<>> builder;
    llvm::Function* currentFunction = nullptr;

    // Tabelas de símbolos
    std::unordered_map<std::string, llvm::Value*> namedValues;
    std::unordered_map<std::string, llvm::Type*> variableTypes;
    std::unordered_map<std::string, llvm::GlobalVariable*> globalValues;
    std::unordered_map<std::string, llvm::Type*> globalTypes;
    std::unordered_map<std::string, llvm::GlobalVariable*> stringLiterals;
        
    std::vector<llvm::BasicBlock*> breakBlocks;
    std::vector<llvm::BasicBlock*> continueBlocks;

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


    void pushLoopBlocks(llvm::BasicBlock* breakBlock, llvm::BasicBlock* continueBlock); 
    void popLoopBlocks();

    // Utilitários
    llvm::Type* getLLVMType(const std::string& typeName);
    void handleGlobalVariables(ProgramNode* program);
    llvm::GlobalVariable* createStringLiteral(const std::string& value);
};

} // namespace ambar

#endif // LLVM_GENERATOR_HPP
