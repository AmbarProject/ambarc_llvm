#include "LLVMGenerator.hpp"
#include "ast/nodes/declarations/ProgramNode.hpp"
#include "ast/nodes/declarations/VarNode.hpp"
#include "ast/nodes/declarations/FunctionNode.hpp"
#include "ast/nodes/expressions/NumberNode.hpp"
#include "ast/nodes/expressions/BinaryNode.hpp"
#include <memory>

using namespace ambar;

int main() {
    // Cria o programa como unique_ptr<ASTNode>
    std::unique_ptr<ASTNode> program = std::make_unique<ProgramNode>();
    auto* programPtr = static_cast<ProgramNode*>(program.get());
    
    // 1. Adiciona declarações de variáveis
    auto intVar = std::make_unique<VarNode>(
        "x", 
        "int", 
        std::make_unique<NumberNode>(42), 
        SourceLocation());
    programPtr->addDeclaration(std::move(intVar));
    
    // 2. Adiciona uma expressão binária como inicializador
    auto leftExpr = std::make_unique<NumberNode>(10);
    auto rightExpr = std::make_unique<NumberNode>(20);
    auto binaryExpr = std::make_unique<BinaryNode>(
        "+",
        std::move(leftExpr),
        std::move(rightExpr),
        SourceLocation());
    
    auto varWithBinary = std::make_unique<VarNode>(
        "sum",
        "int",
        std::move(binaryExpr),
        SourceLocation());
    programPtr->addDeclaration(std::move(varWithBinary));
    
    // 3. Adiciona uma função com expressão binária no retorno
    auto funcBody = std::make_unique<BinaryNode>(
        "*",
        std::make_unique<NumberNode>(5),
        std::make_unique<NumberNode>(6),
        SourceLocation());
    
    auto funcWithBinary = std::make_unique<FunctionNode>(
        "calculate",
        "int",
        FunctionNode::ParamList(),
        std::move(funcBody),  // Agora isso é válido
        SourceLocation());
    programPtr->addDeclaration(std::move(funcWithBinary));
    
    // Gera o código
    LLVMGenerator generator;
    generator.generate(program);
    generator.dumpIR();
    
    return 0;
}