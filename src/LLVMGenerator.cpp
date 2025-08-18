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
        Value* initVal = generateExpr(node->getInit().get());
        if (initVal) {
            // Conversão de tipo se necessário
            if (initVal->getType() != type) {
                if (type->isIntegerTy() && initVal->getType()->isFloatingPointTy()) {
                    initVal = builder->CreateFPToSI(initVal, type, "convtmp");
                } else if (type->isFloatingPointTy() && initVal->getType()->isIntegerTy()) {
                    initVal = builder->CreateSIToFP(initVal, type, "convtmp");
                }
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

Value* LLVMGenerator::generateExpr(ASTNode* node) {
    if (auto num = dynamic_cast<NumberNode*>(node)) {
        return generateNumber(num);
    } else if (auto binary = dynamic_cast<BinaryNode*>(node)) {
        return generateBinaryExpr(binary);
    }
    // Adicione outros tipos de expressões conforme necessário
    return nullptr;
}

Value* LLVMGenerator::generateBinaryExpr(BinaryNode* node) {
    Value* L = generateExpr(node->getLeft());
    Value* R = generateExpr(node->getRight());
    
    if (!L || !R) return nullptr;

    Type* type = L->getType();
    const std::string& op = node->getOp();
    
    // Verificação de tipo
    if (L->getType() != R->getType()) {
        llvm::errs() << "Erro: Tipos incompatíveis na operação binária\n";
        return nullptr;
    }

    if (op == "+") {
        return type->isFloatTy() ? builder->CreateFAdd(L, R, "addtmp") 
                                : builder->CreateAdd(L, R, "addtmp");
    }
    
    // Operações aritméticas
    if (op == "+") {
        return type->isFloatTy() ? builder->CreateFAdd(L, R, "addtmp") 
                                 : builder->CreateAdd(L, R, "addtmp");
    } else if (op == "-") {
        return type->isFloatTy() ? builder->CreateFSub(L, R, "subtmp") 
                                 : builder->CreateSub(L, R, "subtmp");
    } else if (op == "*") {
        return type->isFloatTy() ? builder->CreateFMul(L, R, "multmp") 
                                 : builder->CreateMul(L, R, "multmp");
    } else if (op == "/") {
        return type->isFloatTy() ? builder->CreateFDiv(L, R, "divtmp") 
                                 : builder->CreateSDiv(L, R, "divtmp");
    }
    // Operações de comparação
    else if (op == "<") {
        return builder->CreateFCmpULT(L, R, "cmptmp");
    } else if (op == ">") {
        return builder->CreateFCmpUGT(L, R, "cmptmp");
    } else if (op == "<=") {
        return builder->CreateFCmpULE(L, R, "cmptmp");
    } else if (op == ">=") {
        return builder->CreateFCmpUGE(L, R, "cmptmp");
    } else if (op == "==") {
        return builder->CreateFCmpUEQ(L, R, "cmptmp");
    } else if (op == "!=") {
        return builder->CreateFCmpUNE(L, R, "cmptmp");
    }
    // Operações lógicas
    else if (op == "&&") {
        return builder->CreateAnd(L, R, "andtmp");
    } else if (op == "||") {
        return builder->CreateOr(L, R, "ortmp");
    }

    llvm::errs() << "Operador binário desconhecido: " << op << "\n";
    return nullptr;
}
void LLVMGenerator::generateFunction(FunctionNode* node) {
    if (module->getFunction(node->getName())) {
        return;
    }

    // Limpa a tabela de símbolos para a nova função
    namedValues.clear();

    std::vector<Type*> paramTypes;
    for (const auto& param : node->getParams()) {
        paramTypes.push_back(getLLVMType(param.first));
    }
    
    Type* returnType = getLLVMType(node->getReturnType());
    FunctionType* funcType = FunctionType::get(returnType, paramTypes, false);
    
    Function* func = Function::Create(funcType, Function::ExternalLinkage, node->getName(), *module);
    
    BasicBlock* bb = BasicBlock::Create(*context, "entry", func);
    builder->SetInsertPoint(bb);
    
    // Registrar parâmetros
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
        Value* bodyValue = generateExpr(node->getBody());
        
        // Adicionar retorno apenas se o corpo não tiver terminador
        if (bodyValue && !returnType->isVoidTy()) {
            builder->CreateRet(bodyValue);
        }
        else if (returnType->isVoidTy()) {
            builder->CreateRetVoid();
        }
    }
    else {
        // Apenas adicionar retorno padrão se não houver corpo
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
    }
    
    if (verifyFunction(*func, &llvm::errs())) {
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