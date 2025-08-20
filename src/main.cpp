#include "LLVMGenerator.hpp"
#include "ast/nodes/declarations/ProgramNode.hpp"
#include "ast/nodes/declarations/VarNode.hpp"
#include "ast/nodes/declarations/FunctionNode.hpp"
#include "ast/nodes/expressions/NumberNode.hpp"
#include "ast/nodes/expressions/BinaryNode.hpp"
#include "ast/nodes/expressions/BoolNode.hpp"
#include "ast/nodes/expressions/IdentifierNode.hpp"
#include "ast/nodes/expressions/StringNode.hpp"
#include "ast/nodes/expressions/UnaryNode.hpp"
#include <memory>
#include <utility>
#include <vector>

using namespace ambar;

int main() {
    std::unique_ptr<ASTNode> program = std::make_unique<ProgramNode>();
    auto* programPtr = static_cast<ProgramNode*>(program.get());
    
    // 1. Variáveis GLOBAIS
    auto intVar = std::make_unique<VarNode>(
        "counter", 
        "int", 
        std::make_unique<NumberNode>(100), 
        SourceLocation());
    programPtr->addDeclaration(std::move(intVar));
    
    // 2. Teste de operadores unários
    auto negateTest = std::make_unique<UnaryNode>(
        "-",
        std::make_unique<NumberNode>(42),
        SourceLocation()
    );
    
    auto negateVar = std::make_unique<VarNode>(
        "negative_value",
        "int",
        std::move(negateTest),
        SourceLocation());
    programPtr->addDeclaration(std::move(negateVar));
    
    // 3. Teste de negação lógica
    auto notTest = std::make_unique<UnaryNode>(
        "!",
        std::make_unique<BoolNode>(true),
        SourceLocation()
    );
    
    auto notVar = std::make_unique<VarNode>(
        "not_result",
        "bool",
        std::move(notTest),
        SourceLocation());
    programPtr->addDeclaration(std::move(notVar));
    
    // 4. Teste de complemento bit a bit
    auto bitwiseNotTest = std::make_unique<UnaryNode>(
        "~",
        std::make_unique<NumberNode>(255),
        SourceLocation()
    );
    
    auto bitwiseNotVar = std::make_unique<VarNode>(
        "bitwise_not_result",
        "int",
        std::move(bitwiseNotTest),
        SourceLocation());
    programPtr->addDeclaration(std::move(bitwiseNotVar));
    
    // 5. Teste de operador unário em expressão complexa
    auto complexExpr = std::make_unique<BinaryNode>(
        "+",
        std::make_unique<UnaryNode>(
            "-",
            std::make_unique<NumberNode>(10),
            SourceLocation()
        ),
        std::make_unique<NumberNode>(20),
        SourceLocation()
    );
    
    auto complexVar = std::make_unique<VarNode>(
        "complex_result",
        "int",
        std::move(complexExpr),
        SourceLocation());
    programPtr->addDeclaration(std::move(complexVar));
    
    // 6. Geração do código LLVM IR
    LLVMGenerator generator;
    generator.generate(program);
    generator.dumpIR();
    
    return 0;
}