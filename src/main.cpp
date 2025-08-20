#include "LLVMGenerator.hpp"
#include "ast/nodes/declarations/ProgramNode.hpp"
#include "ast/nodes/declarations/VarNode.hpp"
#include "ast/nodes/declarations/FunctionNode.hpp"
#include "ast/nodes/expressions/NumberNode.hpp"
#include "ast/nodes/expressions/BinaryNode.hpp"
#include "ast/nodes/expressions/BoolNode.hpp"
#include "ast/nodes/expressions/IdentifierNode.hpp"
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
    
    // 2. Expressão matemática com variáveis globais
    auto complexMath = std::make_unique<BinaryNode>(
        "*",
        std::make_unique<BinaryNode>(
            "+",
            std::make_unique<IdentifierNode>("counter"),
            std::make_unique<NumberNode>(50),
            SourceLocation()
        ),
        std::make_unique<BinaryNode>(
            "-",
            std::make_unique<NumberNode>(200),
            std::make_unique<IdentifierNode>("counter"),
            SourceLocation()
        ),
        SourceLocation()
    );
    
    auto mathVar = std::make_unique<VarNode>(
        "math_result",
        "int",
        std::move(complexMath),
        SourceLocation());
    programPtr->addDeclaration(std::move(mathVar));
    
    // 3. Expressão booleana com variáveis globais
    auto complexBool = std::make_unique<BinaryNode>(
        "&&",
        std::make_unique<BinaryNode>(
            ">",
            std::make_unique<IdentifierNode>("counter"),
            std::make_unique<NumberNode>(50),
            SourceLocation()
        ),
        std::make_unique<BinaryNode>(
            "==",
            std::make_unique<IdentifierNode>("is_active"),
            std::make_unique<BoolNode>(true),
            SourceLocation()
        ),
        SourceLocation()
    );
    
    auto boolResult = std::make_unique<VarNode>(
        "should_process",
        "bool",
        std::move(complexBool),
        SourceLocation());
    programPtr->addDeclaration(std::move(boolResult));
    
    // 4. Função com múltiplos parâmetros
    FunctionNode::ParamList multiParams;
    multiParams.emplace_back("int", "a");
    multiParams.emplace_back("int", "b");
    multiParams.emplace_back("bool", "flag");
    
    auto multiParamFuncBody = std::make_unique<BinaryNode>(
        "&&",
        std::make_unique<BinaryNode>(
            ">",
            std::make_unique<BinaryNode>(
                "*",
                std::make_unique<IdentifierNode>("a"),
                std::make_unique<IdentifierNode>("b"),
                SourceLocation()
            ),
            std::make_unique<NumberNode>(0),
            SourceLocation()
        ),
        std::make_unique<IdentifierNode>("flag"),
        SourceLocation()
    );
    
    auto multiParamFunc = std::make_unique<FunctionNode>(
        "complexCheck",
        "bool",
        std::move(multiParams),
        std::move(multiParamFuncBody),
        SourceLocation());
    programPtr->addDeclaration(std::move(multiParamFunc));
    
    // 5. Função que usa parâmetro (local implícito)
    FunctionNode::ParamList validateParams;
    validateParams.emplace_back("int", "value");
    
    // Corpo: return value + 10 > 50
    auto validateBody = std::make_unique<BinaryNode>(
        ">",
        std::make_unique<BinaryNode>(
            "+",
            std::make_unique<IdentifierNode>("value"),
            std::make_unique<NumberNode>(10),
            SourceLocation()
        ),
        std::make_unique<NumberNode>(50),
        SourceLocation()
    );
    
    auto validateFunc = std::make_unique<FunctionNode>(
        "validateCounter",
        "bool",
        std::move(validateParams),
        std::move(validateBody),
        SourceLocation());
    programPtr->addDeclaration(std::move(validateFunc));
    
    // 6. Geração do código LLVM IR
    LLVMGenerator generator;
    generator.generate(program);
    generator.dumpIR();
    
    return 0;
}
