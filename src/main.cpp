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
#include <memory>
#include <utility>
#include <vector>

using namespace ambar;

int main() {
    std::unique_ptr<ASTNode> program = std::make_unique<ProgramNode>();
    auto* programPtr = static_cast<ProgramNode*>(program.get());
    
    // 1. Variável local
    auto localVar = std::make_unique<VarNode>(
        "x",
        "int",
        std::make_unique<NumberNode>(42),
        SourceLocation());
    programPtr->addDeclaration(std::move(localVar));
    
    // 2. Atribuição simples
    auto assign = std::make_unique<AssignNode>(
        "x",
        std::make_unique<NumberNode>(100),
        SourceLocation()
    );
    programPtr->addDeclaration(std::move(assign));
    
    // 3. Geração do código LLVM IR
    LLVMGenerator generator;
    generator.generate(program);
    generator.dumpIR();
    
    return 0;
}