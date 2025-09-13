#include "LLVMGenerator.hpp"
#include "ast/nodes/declarations/ProgramNode.hpp"
#include "ast/nodes/declarations/VarNode.hpp"
#include "ast/nodes/statements/AssignNode.hpp"
#include "ast/nodes/expressions/NumberNode.hpp"
#include "ast/nodes/expressions/BinaryNode.hpp"
#include "ast/nodes/expressions/BoolNode.hpp"
#include "ast/nodes/expressions/IdentifierNode.hpp"
#include "ast/nodes/expressions/StringNode.hpp"
#include "ast/nodes/expressions/UnaryNode.hpp"
#include "ast/nodes/statements/BlockNode.hpp"
#include "ast/nodes/statements/BreakNode.hpp"
#include "ast/nodes/statements/ContinueNode.hpp"
#include "ast/nodes/statements/ForNode.hpp" // ADICIONADO
#include <memory>
#include <utility>
#include <vector>

using namespace ambar;

int main() {
    std::unique_ptr<ASTNode> program = std::make_unique<ProgramNode>();
    auto* programPtr = static_cast<ProgramNode*>(program.get());
    
    // Criar um bloco com escopo local
    auto block = std::make_unique<BlockNode>(SourceLocation());
    
    // 1. Variável local dentro do bloco
    auto localVar = std::make_unique<VarNode>(
        "x",
        "int",
        std::make_unique<NumberNode>(42),
        SourceLocation());
    block->add(std::move(localVar));
    
    // 2. Teste de loop for
    auto forLoop = std::make_unique<ForNode>(
        // Inicialização: int i = 0
        std::make_unique<VarNode>(
            "i",
            "int",
            std::make_unique<NumberNode>(0),
            SourceLocation()
        ),
        // Condição: i < 5
        std::make_unique<BinaryNode>(
            "<",
            std::make_unique<IdentifierNode>("i"),
            std::make_unique<NumberNode>(5),
            SourceLocation()
        ),
        // Atualização: i = i + 1
        std::make_unique<AssignNode>(
            "i",
            std::make_unique<BinaryNode>(
                "+",
                std::make_unique<IdentifierNode>("i"),
                std::make_unique<NumberNode>(1),
                SourceLocation()
            ),
            SourceLocation()
        ),
        // Corpo do loop
        std::make_unique<BlockNode>(SourceLocation())
    );
    
    // Adicionar algumas instruções no corpo do loop
    auto forBody = forLoop->getBody();
    forBody->add(std::make_unique<AssignNode>(
        "x",
        std::make_unique<BinaryNode>(
            "+",
            std::make_unique<IdentifierNode>("x"),
            std::make_unique<IdentifierNode>("i"),
            SourceLocation()
        ),
        SourceLocation()
    ));
    
    block->add(std::move(forLoop));
    
    // 3. Atribuição dentro do bloco
    auto assign = std::make_unique<AssignNode>(
        "x",
        std::make_unique<BinaryNode>(
            "+",                                    
            std::make_unique<IdentifierNode>("x"),
            std::make_unique<NumberNode>(10),       
            SourceLocation()
        ),
        SourceLocation()
    );
    block->add(std::move(assign));
    
    // 4. Adicionar o bloco ao programa
    programPtr->addDeclaration(std::move(block));
    
    // 5. Variável global fora do bloco
    auto globalVar = std::make_unique<VarNode>(
        "global_var",
        "int",
        std::make_unique<NumberNode>(100),
        SourceLocation());
    programPtr->addDeclaration(std::move(globalVar));
    
    // 6. Geração do código LLVM IR
    LLVMGenerator generator;
    generator.generate(program);
    generator.dumpIR();
    
    return 0;
}