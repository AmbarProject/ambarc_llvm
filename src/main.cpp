#include "LLVMGenerator.hpp"
#include "ast/nodes/declarations/ProgramNode.hpp"
#include "ast/nodes/declarations/VarNode.hpp"
#include "ast/nodes/declarations/FunctionNode.hpp"
#include "ast/nodes/expressions/NumberNode.hpp"
#include "ast/nodes/expressions/BinaryNode.hpp"
#include "ast/nodes/expressions/BoolNode.hpp"
#include "ast/nodes/expressions/IdentifierNode.hpp"
#include "ast/nodes/expressions/StringNode.hpp" // <-- Adicionar esta linha
#include <memory>
#include <utility>
#include <vector>

using namespace ambar;

int main() {
    std::unique_ptr<ASTNode> program = std::make_unique<ProgramNode>();
    auto* programPtr = static_cast<ProgramNode*>(program.get());
    
    // 1. Variáveis GLOBAIS (incluindo string)
    auto intVar = std::make_unique<VarNode>(
        "counter", 
        "int", 
        std::make_unique<NumberNode>(100), 
        SourceLocation());
    programPtr->addDeclaration(std::move(intVar));
    
    auto stringVar = std::make_unique<VarNode>( // <-- Adicionar variável string
        "message", 
        "string", 
        std::make_unique<StringNode>("Hello, World!"), 
        SourceLocation());
    programPtr->addDeclaration(std::move(stringVar));
    
    auto floatVar = std::make_unique<VarNode>(
        "pi", 
        "float", 
        std::make_unique<NumberNode>(3.14159f), 
        SourceLocation());
    programPtr->addDeclaration(std::move(floatVar));
    
    auto boolVar = std::make_unique<VarNode>(
        "is_active", 
        "bool", 
        std::make_unique<BoolNode>(true), 
        SourceLocation());
    programPtr->addDeclaration(std::move(boolVar));
    
    // 2. Concatenação de strings (exemplo simples)
    auto stringConcat = std::make_unique<BinaryNode>(
        "+",
        std::make_unique<StringNode>("Prefix: "),
        std::make_unique<IdentifierNode>("message"),
        SourceLocation()
    );
    
    auto concatVar = std::make_unique<VarNode>(
        "concatenated",
        "string",
        std::move(stringConcat),
        SourceLocation());
    programPtr->addDeclaration(std::move(concatVar));
    
    // 3. Função que retorna string
    FunctionNode::ParamList stringParams;
    stringParams.emplace_back("int", "value");
    
    auto stringFuncBody = std::make_unique<StringNode>("Result: ");
    // Nota: Em uma implementação real, você precisaria de operações de concatenação
    
    auto stringFunc = std::make_unique<FunctionNode>(
        "getResultMessage",
        "string",
        std::move(stringParams),
        std::move(stringFuncBody),
        SourceLocation());
    programPtr->addDeclaration(std::move(stringFunc));
    
    // 4. Geração do código LLVM IR
    LLVMGenerator generator;
    generator.generate(program);
    generator.dumpIR();
    
    return 0;
}