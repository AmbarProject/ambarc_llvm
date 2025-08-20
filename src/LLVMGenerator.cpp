#include "LLVMGenerator.hpp"
#include <llvm/IR/Verifier.h>
#include <memory>

using namespace ambar;
using namespace llvm;

LLVMGenerator::LLVMGenerator() {
    context = std::make_unique<LLVMContext>();
    module = std::make_unique<Module>("ambar", *context);
    builder = std::make_unique<IRBuilder<>>(*context);
    currentFunction = nullptr;
}

void LLVMGenerator::generate(std::unique_ptr<ASTNode>& astRoot) {
    if (auto programNode = dynamic_cast<ProgramNode*>(astRoot.get())) {
        // Processa variáveis globais primeiro
        handleGlobalVariables(programNode);
        
        // Gera a função main
        generateMainFunction(astRoot);
        
        // Gera outras funções
        for (const auto& decl : programNode->getDeclarations()) {
            if (auto funcNode = dynamic_cast<FunctionNode*>(decl.get())) {
                if (funcNode->getName() != "main") {
                    generateFunction(funcNode);
                }
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
    
    currentFunction = mainFunc;
    BasicBlock* bb = BasicBlock::Create(*context, "entry", mainFunc);
    builder->SetInsertPoint(bb);
    
    if (auto programNode = dynamic_cast<ProgramNode*>(astRoot.get())) {
        for (const auto& decl : programNode->getDeclarations()) {
            if (auto varNode = dynamic_cast<VarNode*>(decl.get())) {
                // Se não for global, trata como local
                if (!globalValues.count(varNode->getName())) {
                    generateLocalVariable(varNode);
                }
            }
        }
    }
    
    builder->CreateRet(ConstantInt::get(*context, APInt(32, 0)));
    verifyFunction(*mainFunc);
    currentFunction = nullptr;
}

Value* LLVMGenerator::generateLocalVariable(VarNode* node) {
    Type* type = getLLVMType(node->getType());
    AllocaInst* alloca = builder->CreateAlloca(type, nullptr, node->getName());
    namedValues[node->getName()] = alloca;
    variableTypes[node->getName()] = type;

    if (node->getInit()) {
        Value* initVal = generateExpr(node->getInit().get());
        if (initVal) {
            if (initVal->getType() != type) {
                if (type->isIntegerTy(1) && initVal->getType()->isIntegerTy(32)) {
                    initVal = builder->CreateICmpNE(
                        initVal, 
                        ConstantInt::get(*context, APInt(32, 0)), 
                        "boolconv");
                } else if (type->isIntegerTy(32) && initVal->getType()->isIntegerTy(1)) {
                    initVal = builder->CreateZExt(initVal, type, "intconv");
                }
            }
            builder->CreateStore(initVal, alloca);
        }
    }
    return alloca;
}

Value* LLVMGenerator::generateGlobalVariable(VarNode* node) {
    Type* type = getLLVMType(node->getType());
    Constant* initVal = nullptr;
    
    if (node->getInit()) {
        initVal = evaluateConstantExpression(node->getInit().get());
        
        if (!initVal) {
            // Se não for constante, tenta avaliar em tempo de compilação
            BasicBlock* currentBB = builder->GetInsertBlock();
            builder->ClearInsertionPoint();
            
            Function* tempFunc = Function::Create(
                FunctionType::get(type, false),
                Function::InternalLinkage,
                "temp_eval",
                *module
            );
            
            BasicBlock* bb = BasicBlock::Create(*context, "entry", tempFunc);
            builder->SetInsertPoint(bb);
            
            Value* init = generateExpr(node->getInit().get());
            if (auto constant = dyn_cast<Constant>(init)) {
                initVal = constant;
            }
            
            tempFunc->eraseFromParent();
            if (currentBB) {
                builder->SetInsertPoint(currentBB);
            }
        }
        
        if (!initVal) {
            llvm::errs() << "Aviso: Expressão não constante para variável global '" 
                        << node->getName() << "'. Inicializando com zero.\n";
        }
    }

    if (!initVal) {
        // Valores padrão
        if (type->isFloatTy()) {
            initVal = ConstantFP::get(*context, APFloat(0.0f));
        } else if (type->isIntegerTy(32)) {
            initVal = ConstantInt::get(*context, APInt(32, 0));
        } else if (type->isIntegerTy(1)) {
            initVal = ConstantInt::get(*context, APInt(1, 0));
        }
    }

    GlobalVariable* gVar = new GlobalVariable(
        *module,
        type,
        false,
        GlobalValue::InternalLinkage,
        initVal,
        node->getName()
    );
    
    globalValues[node->getName()] = gVar;
    globalTypes[node->getName()] = type;
    return gVar;
}


Constant* LLVMGenerator::evaluateConstantExpression(ASTNode* node) {
    // Casos básicos para números e booleanos
    if (auto num = dynamic_cast<NumberNode*>(node)) {
        if (num->isFloatingPoint()) {
            return ConstantFP::get(*context, APFloat(num->getFloatValue()));
        } else {
            return ConstantInt::get(*context, APInt(32, num->getIntValue()));
        }
    } else if (auto boolNode = dynamic_cast<BoolNode*>(node)) {
        return ConstantInt::get(*context, APInt(1, boolNode->getValue() ? 1 : 0));
    } else if (auto ident = dynamic_cast<IdentifierNode*>(node)) {
        // Tenta resolver identificadores como constantes globais
        if (globalValues.count(ident->getName())) {
            return dyn_cast<Constant>(globalValues[ident->getName()]->getInitializer());
        }
        return nullptr;
    } else if (auto binary = dynamic_cast<BinaryNode*>(node)) {
        Constant* L = evaluateConstantExpression(binary->getLeft());
        Constant* R = evaluateConstantExpression(binary->getRight());
        
        if (!L || !R) return nullptr;

        const std::string& op = binary->getOp();
        
        // Operações com floats
        if (L->getType()->isFloatTy() && R->getType()->isFloatTy()) {
            APFloat lVal = cast<ConstantFP>(L)->getValueAPF();
            APFloat rVal = cast<ConstantFP>(R)->getValueAPF();
            
            if (op == "+") {
                lVal.add(rVal, APFloat::rmNearestTiesToEven);
                return ConstantFP::get(*context, lVal);
            } else if (op == "-") {
                lVal.subtract(rVal, APFloat::rmNearestTiesToEven);
                return ConstantFP::get(*context, lVal);
            } else if (op == "*") {
                lVal.multiply(rVal, APFloat::rmNearestTiesToEven);
                return ConstantFP::get(*context, lVal);
            } else if (op == "/") {
                lVal.divide(rVal, APFloat::rmNearestTiesToEven);
                return ConstantFP::get(*context, lVal);
            }
        }
        // Operações com inteiros
        else if (L->getType()->isIntegerTy() && R->getType()->isIntegerTy()) {
            APInt lVal = cast<ConstantInt>(L)->getValue();
            APInt rVal = cast<ConstantInt>(R)->getValue();
            
            if (op == "+") return ConstantInt::get(*context, lVal + rVal);
            if (op == "-") return ConstantInt::get(*context, lVal - rVal);
            if (op == "*") return ConstantInt::get(*context, lVal * rVal);
            if (op == "/") {
                if (rVal == 0) {
                    llvm::errs() << "Erro: Divisão por zero em expressão constante\n";
                    return nullptr;
                }
                return ConstantInt::get(*context, lVal.sdiv(rVal));
            }
            if (op == ">") return ConstantInt::get(*context, APInt(1, lVal.sgt(rVal)));
            if (op == "<") return ConstantInt::get(*context, APInt(1, lVal.slt(rVal)));
            if (op == ">=") return ConstantInt::get(*context, APInt(1, lVal.sge(rVal)));
            if (op == "<=") return ConstantInt::get(*context, APInt(1, lVal.sle(rVal)));
            if (op == "==") return ConstantInt::get(*context, APInt(1, lVal.eq(rVal)));
            if (op == "!=") return ConstantInt::get(*context, APInt(1, lVal.ne(rVal)));
            if (op == "&&" || op == "||") {
                bool lBool = lVal.getBoolValue();
                bool rBool = rVal.getBoolValue();
                bool result = (op == "&&") ? (lBool && rBool) : (lBool || rBool);
                return ConstantInt::get(*context, APInt(1, result));
            }
        }
    }
    return nullptr;
}

void LLVMGenerator::handleGlobalVariables(ProgramNode* program) {
    // Primeiro passe: cria todas as globais com valores padrão
    for (const auto& decl : program->getDeclarations()) {
        if (auto varNode = dynamic_cast<VarNode*>(decl.get())) {
            Type* type = getLLVMType(varNode->getType());
            Constant* initVal = nullptr;
            
            if (type->isFloatTy()) {
                initVal = ConstantFP::get(*context, APFloat(0.0f));
            } else if (type->isIntegerTy(32)) {
                initVal = ConstantInt::get(*context, APInt(32, 0));
            } else if (type->isIntegerTy(1)) {
                initVal = ConstantInt::get(*context, APInt(1, 0));
            }
            
            GlobalVariable* gVar = new GlobalVariable(
                *module,
                type,
                false,
                GlobalValue::InternalLinkage,
                initVal,
                varNode->getName()
            );
            
            globalValues[varNode->getName()] = gVar;
            globalTypes[varNode->getName()] = type;
        }
    }
    
    // Segundo passe: avalia as expressões constantes
    for (const auto& decl : program->getDeclarations()) {
        if (auto varNode = dynamic_cast<VarNode*>(decl.get())) {
            if (varNode->getInit()) {
                Constant* initVal = evaluateConstantExpression(varNode->getInit().get());
                if (initVal) {
                    globalValues[varNode->getName()]->setInitializer(initVal);
                } else {
                    llvm::errs() << "Aviso: Expressão não constante para variável global '" 
                                << varNode->getName() << "'. Mantendo valor zero.\n";
                }
            }
        }
    }
}


Value* LLVMGenerator::generateVariable(VarNode* node) {
    if (currentFunction) {
        return generateLocalVariable(node);
    }
    return generateGlobalVariable(node);
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
    } else if (auto boolNode = dynamic_cast<BoolNode*>(node)) {
        return generateBool(boolNode);
    } else if (auto ident = dynamic_cast<IdentifierNode*>(node)) {
        return generateIdentifier(ident);
    } else if (auto var = dynamic_cast<VarNode*>(node)) {
        return generateVariable(var);
    }
    return nullptr;
}

Value* LLVMGenerator::generateBool(BoolNode* node) {
    return ConstantInt::get(*context, APInt(1, node->getValue() ? 1 : 0));
}

Value* LLVMGenerator::generateBinaryExpr(BinaryNode* node) {
    Value* L = generateExpr(node->getLeft());
    Value* R = generateExpr(node->getRight());
    
    if (!L || !R) return nullptr;

    Type* type = L->getType();
    const std::string& op = node->getOp();
    
    if (L->getType() != R->getType()) {
        if (L->getType()->isIntegerTy(32) && R->getType()->isFloatTy()) {
            L = builder->CreateSIToFP(L, Type::getFloatTy(*context), "floatconv");
            type = Type::getFloatTy(*context);
        } else if (L->getType()->isFloatTy() && R->getType()->isIntegerTy(32)) {
            R = builder->CreateSIToFP(R, Type::getFloatTy(*context), "floatconv");
            type = Type::getFloatTy(*context);
        } else {
            llvm::errs() << "Erro: Tipos incompatíveis na operação binária\n";
            return nullptr;
        }
    }

    if (op == "&&" || op == "||") {
        if (!L->getType()->isIntegerTy(1)) {
            L = builder->CreateICmpNE(L, ConstantInt::get(L->getType(), 0), "booltmp");
        }
        if (!R->getType()->isIntegerTy(1)) {
            R = builder->CreateICmpNE(R, ConstantInt::get(R->getType(), 0), "booltmp");
        }
        
        return op == "&&" ? builder->CreateAnd(L, R, "andtmp") 
                         : builder->CreateOr(L, R, "ortmp");
    }

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
    } else if (op == "<") {
        return type->isFloatTy() ? builder->CreateFCmpULT(L, R, "cmptmp")
                               : builder->CreateICmpSLT(L, R, "cmptmp");
    } else if (op == ">") {
        return type->isFloatTy() ? builder->CreateFCmpUGT(L, R, "cmptmp")
                               : builder->CreateICmpSGT(L, R, "cmptmp");
    } else if (op == "<=") {
        return type->isFloatTy() ? builder->CreateFCmpULE(L, R, "cmptmp")
                               : builder->CreateICmpSLE(L, R, "cmptmp");
    } else if (op == ">=") {
        return type->isFloatTy() ? builder->CreateFCmpUGE(L, R, "cmptmp")
                               : builder->CreateICmpSGE(L, R, "cmptmp");
    } else if (op == "==") {
        return type->isFloatTy() ? builder->CreateFCmpUEQ(L, R, "cmptmp")
                               : builder->CreateICmpEQ(L, R, "cmptmp");
    } else if (op == "!=") {
        return type->isFloatTy() ? builder->CreateFCmpUNE(L, R, "cmptmp")
                               : builder->CreateICmpNE(L, R, "cmptmp");
    }

    llvm::errs() << "Operador binário desconhecido: " << op << "\n";
    return nullptr;
}

Value* LLVMGenerator::generateIdentifier(IdentifierNode* node) {
    if (namedValues.count(node->getName())) {
        Value* val = namedValues[node->getName()];
        return builder->CreateLoad(variableTypes[node->getName()], val);
    }
    
    if (globalValues.count(node->getName())) {
        GlobalVariable* gvar = globalValues[node->getName()];
        return builder->CreateLoad(globalTypes[node->getName()], gvar);
    }
    
    llvm::errs() << "Erro: Variável '" << node->getName() << "' não declarada\n";
    return nullptr;
}

void LLVMGenerator::generateFunction(FunctionNode* node) {
    if (module->getFunction(node->getName())) {
        llvm::errs() << "Aviso: Função '" << node->getName() << "' já declarada\n";
        return;
    }

    namedValues.clear();
    variableTypes.clear();

    std::vector<Type*> paramTypes;
    for (const auto& param : node->getParams()) {
        Type* paramType = getLLVMType(param.first);
        if (!paramType) {
            llvm::errs() << "Erro: Tipo de parâmetro desconhecido\n";
            return;
        }
        paramTypes.push_back(paramType);
    }
    
    Type* returnType = getLLVMType(node->getReturnType());
    if (!returnType) {
        llvm::errs() << "Erro: Tipo de retorno desconhecido\n";
        return;
    }
    
    FunctionType* funcType = FunctionType::get(returnType, paramTypes, false);
    Function* func = Function::Create(
        funcType,
        Function::ExternalLinkage,
        node->getName(),
        *module
    );
    
    currentFunction = func;
    BasicBlock* bb = BasicBlock::Create(*context, "entry", func);
    builder->SetInsertPoint(bb);
    
    unsigned idx = 0;
    for (auto& arg : func->args()) {
        const auto& paramName = node->getParams()[idx].second;
        arg.setName(paramName);
        
        AllocaInst* alloca = builder->CreateAlloca(
            paramTypes[idx],
            nullptr,
            paramName + ".addr"
        );
        
        builder->CreateStore(&arg, alloca);
        namedValues[paramName] = alloca;
        variableTypes[paramName] = paramTypes[idx];
        idx++;
    }
    
    if (node->getBody()) {
        Value* bodyValue = generateExpr(node->getBody());
        
        if (!bodyValue) {
            func->eraseFromParent();
            currentFunction = nullptr;
            return;
        }
        
        if (!returnType->isVoidTy() && bodyValue->getType() != returnType) {
            if (returnType->isIntegerTy(1) && bodyValue->getType()->isIntegerTy(32)) {
                bodyValue = builder->CreateICmpNE(bodyValue, ConstantInt::get(*context, APInt(32, 0)), "boolconv");
            } else if (returnType->isIntegerTy(32) && bodyValue->getType()->isIntegerTy(1)) {
                bodyValue = builder->CreateZExt(bodyValue, Type::getInt32Ty(*context), "intconv");
            } else if (returnType->isFloatTy() && bodyValue->getType()->isIntegerTy(32)) {
                bodyValue = builder->CreateSIToFP(bodyValue, Type::getFloatTy(*context), "floatconv");
            } else {
                func->eraseFromParent();
                currentFunction = nullptr;
                return;
            }
        }
        
        if (returnType->isVoidTy()) {
            builder->CreateRetVoid();
        } else {
            builder->CreateRet(bodyValue);
        }
    } else {
        if (returnType->isVoidTy()) {
            builder->CreateRetVoid();
        } else {
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
        }
    }
    
    if (verifyFunction(*func, &llvm::errs())) {
        func->eraseFromParent();
    }
    currentFunction = nullptr;
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