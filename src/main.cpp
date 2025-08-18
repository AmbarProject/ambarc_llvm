#include "LLVMGenerator.hpp"
#include "ast/nodes/declarations/ProgramNode.hpp"
#include "ast/nodes/declarations/VarNode.hpp"
#include "ast/nodes/declarations/FunctionNode.hpp"
#include "ast/nodes/expressions/NumberNode.hpp"
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
    
    auto floatVar = std::make_unique<VarNode>(
        "pi",
        "float",
        std::make_unique<NumberNode>(3.14f),
        SourceLocation());
    programPtr->addDeclaration(std::move(floatVar));
    
    // 2. Adiciona função com parâmetros (exemplo anterior)
    FunctionNode::ParamList params = {{"int", "a"}, {"float", "b"}};
    auto funcWithParams = std::make_unique<FunctionNode>(
        "funcao_com_parametros",
        "float",
        std::move(params),
        nullptr,
        SourceLocation());
    programPtr->addDeclaration(std::move(funcWithParams));
    
    // 3. Adiciona função SEM parâmetros (novo teste)
    auto funcNoParams = std::make_unique<FunctionNode>(
        "funcao_sem_parametros",
        "int",
        FunctionNode::ParamList(),  // Lista vazia de parâmetros
        nullptr,
        SourceLocation());
    programPtr->addDeclaration(std::move(funcNoParams));
    
    // 4. Adiciona função void sem parâmetros
    auto voidFunc = std::make_unique<FunctionNode>(
        "funcao_void",
        "void",
        FunctionNode::ParamList(),
        nullptr,
        SourceLocation());
    programPtr->addDeclaration(std::move(voidFunc));
    
    // Gera o código
    LLVMGenerator generator;
    generator.generate(program);
    generator.dumpIR();
    
    return 0;
}