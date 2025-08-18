#include "LLVMGenerator.hpp"
#include <llvm/IR/Verifier.h>
#include <memory>

using namespace ambar;
using namespace llvm;

LLVMGenerator::LLVMGenerator() {
    context = std::make_unique<LLVMContext>();
    module = std::make_unique<Module>("ambar", *context);
    builder = std::make_unique<IRBuilder<>>(*context);
}

void LLVMGenerator::generate(std::unique_ptr<ASTNode>& astRoot) {
    // Primeiro geramos a função main com as variáveis globais
    generateMainFunction(astRoot);
    
    // Depois geramos outras funções
    if (auto programNode = dynamic_cast<ProgramNode*>(astRoot.get())) {
        for (const auto& decl : programNode->getDeclarations()) {
            if (auto funcNode = dynamic_cast<FunctionNode*>(decl.get())) {
                generateFunction(funcNode);
            }
        }
    }
}

void LLVMGenerator::generateMainFunction(std::unique_ptr<ASTNode>& astRoot) {
    FunctionType* mainFuncType = FunctionType::get(
        Type::getInt32Ty(*context),
        false
    );
    
    Function* mainFunc = Function::Create(
        mainFuncType,
        Function::ExternalLinkage,
        "main",
        *module
    );
    
    BasicBlock* bb = BasicBlock::Create(*context, "entry", mainFunc);
    builder->SetInsertPoint(bb);
    
    // Processa as declarações de variáveis
    if (auto programNode = dynamic_cast<ProgramNode*>(astRoot.get())) {
        for (const auto& decl : programNode->getDeclarations()) {
            if (auto varNode = dynamic_cast<VarNode*>(decl.get())) {
                generateVariable(varNode);
            }
        }
    }
    
    builder->CreateRet(ConstantInt::get(*context, APInt(32, 0)));
    verifyFunction(*mainFunc);
}

void LLVMGenerator::generateProgram(ProgramNode* node) {
    for (const auto& decl : node->getDeclarations()) {
        if (auto varNode = dynamic_cast<VarNode*>(decl.get())) {
            // Variáveis globais serão tratadas separadamente
        } else if (auto funcNode = dynamic_cast<FunctionNode*>(decl.get())) {
            generateFunction(funcNode);
        }
    }
}

void LLVMGenerator::generateVariable(VarNode* node) {
    Type* type = getLLVMType(node->getType());
    AllocaInst* alloca = builder->CreateAlloca(type, nullptr, node->getName());
    namedValues[node->getName()] = alloca;
    
    if (node->getInit()) {
        if (auto num = dynamic_cast<NumberNode*>(node->getInit().get())) {
            Value* initVal;
            if (node->getType() == "float") {
                initVal = ConstantFP::get(*context, APFloat((float)num->getFloatValue()));
            } else if (node->getType() == "int") {
                initVal = ConstantInt::get(*context, APInt(32, (int)num->getIntValue()));
            }
            builder->CreateStore(initVal, alloca);
        }
    }
}

Value* LLVMGenerator::generateNumber(NumberNode* node) {
    if (node->isFloatingPoint()) {
        return ConstantFP::get(*context, APFloat(node->getFloatValue()));
    } else {
        return ConstantInt::get(*context, APInt(32, node->getIntValue()));
    }
}

void LLVMGenerator::generateFunction(FunctionNode* node) {
    // Verifica se a função já foi declarada
    if (module->getFunction(node->getName())) {
        return;
    }

    // Coletar tipos dos parâmetros
    std::vector<Type*> paramTypes;
    for (const auto& param : node->getParams()) {
        paramTypes.push_back(getLLVMType(param.first));
    }
    
    // Criar tipo da função
    Type* returnType = getLLVMType(node->getReturnType());
    FunctionType* funcType = FunctionType::get(
        returnType,
        paramTypes,
        false
    );
    
    // Criar a função
    Function* func = Function::Create(
        funcType,
        Function::ExternalLinkage,
        node->getName(),
        *module
    );
    
    // Criar bloco básico
    BasicBlock* bb = BasicBlock::Create(*context, "entry", func);
    builder->SetInsertPoint(bb);
    
    // Registrar parâmetros na tabela de símbolos
    unsigned idx = 0;
    for (auto& arg : func->args()) {
        const auto& paramName = node->getParams()[idx].second;
        arg.setName(paramName);
        AllocaInst* alloca = builder->CreateAlloca(
            getLLVMType(node->getParams()[idx].first),
            nullptr,
            paramName + ".addr"
        );
        builder->CreateStore(&arg, alloca);
        namedValues[paramName] = alloca;
        idx++;
    }
    
    // Gerar o corpo da função (se existir)
    if (node->getBody()) {
        // TODO: Implementar geração do corpo da função
        // Por enquanto, apenas adicionamos um retorno padrão
    }
    
    // Garantir que todas as funções não-void tenham um retorno
    if (!returnType->isVoidTy()) {
        Value* retVal = nullptr;
        if (returnType->isFloatTy()) {
            retVal = ConstantFP::get(*context, APFloat(0.0f));
        } else if (returnType->isIntegerTy(32)) {
            retVal = ConstantInt::get(*context, APInt(32, 0));
        } else if (returnType->isIntegerTy(1)) {
            retVal = ConstantInt::get(*context, APInt(1, 0));
        }
        
        if (retVal) {
            builder->CreateRet(retVal);
        } else {
            builder->CreateUnreachable();
        }
    } else {
        builder->CreateRetVoid();
    }
    
    // Verificação final da função
    if (verifyFunction(*func, &llvm::errs())) {
        // Se houver erro, remove a função mal formada
        func->eraseFromParent();
        llvm::errs() << "Erro: Função '" << node->getName() << "' não pôde ser verificada\n";
    }
}

Type* LLVMGenerator::getLLVMType(const std::string& typeName) {
    if (typeName == "int") return Type::getInt32Ty(*context);
    if (typeName == "float") return Type::getFloatTy(*context);
    if (typeName == "bool") return Type::getInt1Ty(*context);
    return Type::getVoidTy(*context);
}

void LLVMGenerator::dumpIR() const {
    module->print(llvm::errs(), nullptr);
}