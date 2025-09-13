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
#include "ast/nodes/expressions/CallNode.hpp"
#include "ast/nodes/statements/AssignNode.hpp"
#include "ast/nodes/statements/BlockNode.hpp"
#include "ast/nodes/statements/BreakNode.hpp"
#include "ast/nodes/statements/ContinueNode.hpp"
#include "ast/nodes/statements/ForNode.hpp"
#include "ast/nodes/statements/IfNode.hpp"
#include "ast/nodes/statements/ReturnNode.hpp"
#include "ast/nodes/statements/WhileNode.hpp"
#include <memory>
#include <utility>
#include <vector>

using namespace ambar;

int main() {
    std::unique_ptr<ASTNode> program = std::make_unique<ProgramNode>();
    auto* programPtr = static_cast<ProgramNode*>(program.get());
    
    // Função de exemplo que será chamada
    FunctionNode::ParamList addParams = {{"int", "a"}, {"int", "b"}};
    auto addFunc = std::make_unique<FunctionNode>(
        "add",                    // nome
        "int",                    // tipo de retorno
        addParams,                // parâmetros
        nullptr,                  // corpo (será definido depois)
        SourceLocation()          // localização
    );
    
    // Corpo da função add
    auto addBody = std::make_unique<BlockNode>(SourceLocation());
    addBody->add(std::make_unique<ReturnNode>(
        std::make_unique<BinaryNode>(
            "+",
            std::make_unique<IdentifierNode>("a"),
            std::make_unique<IdentifierNode>("b"),
            SourceLocation()
        ),
        SourceLocation()
    ));
    
    addFunc->setBody(std::move(addBody));
    programPtr->addDeclaration(std::move(addFunc));
    
    // Bloco principal
    auto mainBlock = std::make_unique<BlockNode>(SourceLocation());
    
    // Chamada da função add
    std::vector<ASTNodePtr> callArgs;
    callArgs.push_back(std::make_unique<NumberNode>(5));
    callArgs.push_back(std::make_unique<NumberNode>(3));
    
    auto callAdd = std::make_unique<CallNode>("add", std::move(callArgs), SourceLocation());
    
    // Atribuir resultado a uma variável
    mainBlock->add(std::make_unique<VarNode>(
        "result",
        "int",
        std::move(callAdd),
        SourceLocation()
    ));
    
    // Chamada de função sem retorno (exemplo com print)
    FunctionNode::ParamList printParams = {{"int", "value"}};
    auto printFunc = std::make_unique<FunctionNode>(
        "print",                  // nome
        "void",                   // tipo de retorno
        printParams,              // parâmetros
        nullptr,                  // corpo
        SourceLocation()          // localização
    );
    
    programPtr->addDeclaration(std::move(printFunc));
    
    // Chamar print com o resultado
    std::vector<ASTNodePtr> printArgs;
    printArgs.push_back(std::make_unique<IdentifierNode>("result"));
    
    mainBlock->add(std::make_unique<CallNode>("print", std::move(printArgs), SourceLocation()));
    
    programPtr->addDeclaration(std::move(mainBlock));
    
    // Geração do código LLVM IR
    LLVMGenerator generator;
    generator.generate(program);
    generator.dumpIR();
    
    return 0;
}