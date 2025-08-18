#ifndef LLVM_GENERATOR_HPP
#define LLVM_GENERATOR_HPP

#include "ast/nodes/ASTNode.hpp"
#include "ast/nodes/declarations/ProgramNode.hpp"
#include "ast/nodes/declarations/VarNode.hpp"
#include "ast/nodes/declarations/FunctionNode.hpp"
#include "ast/nodes/expressions/NumberNode.hpp"
#include "ast/nodes/expressions/BinaryNode.hpp"

#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>

#include <map>
#include <memory>
#include <string>

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
    std::map<std::string, llvm::Value*> namedValues;

    void generateProgram(ProgramNode* node);
    void generateFunction(FunctionNode* node);
    void generateVariable(VarNode* node);
    void generateMainFunction(std::unique_ptr<ASTNode>& astRoot);
    llvm::Value* generateBinaryExpr(BinaryNode* node);
    llvm::Value* generateNumber(NumberNode* node);
    llvm::Value* generateExpr(ASTNode* node);
    llvm::Type* getLLVMType(const std::string& typeName);
};

} // namespace ambar

#endif // LLVM_GENERATOR_HPP