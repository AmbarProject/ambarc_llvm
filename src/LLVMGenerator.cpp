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

GlobalVariable* LLVMGenerator::createStringLiteral(const std::string& value) {
    if (stringLiterals.count(value)) {
        return stringLiterals[value];
    }
    
    // Criar array constante
    Constant* strConstant = ConstantDataArray::getString(*context, value);
    
    // Criar variável global para o array
    GlobalVariable* strArray = new GlobalVariable(
        *module,
        strConstant->getType(),
        true, // constante
        GlobalValue::PrivateLinkage,
        strConstant,
        ".str"
    );
    
    stringLiterals[value] = strArray;
    return strArray;
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
        // Primeiro: processar apenas declarações de variáveis locais
        for (const auto& decl : programNode->getDeclarations()) {
            if (auto varNode = dynamic_cast<VarNode*>(decl.get())) {
                // Criar apenas variáveis locais (não globais)
                generateLocalVariable(varNode);
            }
        }
        
        // Segundo: processar assignments
        for (const auto& decl : programNode->getDeclarations()) {
            if (auto assignNode = dynamic_cast<AssignNode*>(decl.get())) {
                generateAssign(assignNode);
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
                } else if (type->isIntegerTy(32) && initVal->getType()->isIntegerTy(1)) {
                    initVal = builder->CreateZExt(initVal, type, "intconv");
                } else if (type->isFloatTy() && initVal->getType()->isIntegerTy(32)) {
                    initVal = builder->CreateSIToFP(initVal, type, "floatconv");
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
        } else if (type->isPointerTy()) {
            // Para strings, inicializar com null
            initVal = ConstantPointerNull::get(cast<PointerType>(type));
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
    if (!node) {
        llvm::errs() << "Erro: Nó nulo em expressão constante\n";
        return nullptr;
    }

    // Casos básicos para números e booleanos
    if (auto num = dynamic_cast<NumberNode*>(node)) {
        if (num->isFloatingPoint()) {
            float floatValue = static_cast<float>(num->getFloatValue());
            return ConstantFP::get(*context, APFloat(floatValue));
        } else {
            return ConstantInt::get(*context, APInt(32, num->getIntValue()));
        }
    } 
    else if (auto boolNode = dynamic_cast<BoolNode*>(node)) {
        return ConstantInt::get(*context, APInt(1, boolNode->getValue() ? 1 : 0));
    } 
    else if (auto strNode = dynamic_cast<StringNode*>(node)) {
        return createStringLiteral(strNode->getValue());
    } 
    else if (auto ident = dynamic_cast<IdentifierNode*>(node)) {
        if (globalValues.count(ident->getName())) {
            GlobalVariable* gvar = globalValues[ident->getName()];
            if (isa<Constant>(gvar->getInitializer())) {
                return dyn_cast<Constant>(gvar->getInitializer());
            } else {
                llvm::errs() << "Erro: Variável global '" << ident->getName() 
                            << "' não tem inicializador constante\n";
                return nullptr;
            }
        } else {
            llvm::errs() << "Erro: Variável '" << ident->getName() 
                        << "' não declarada em contexto constante\n";
            return nullptr;
        }
    } 
    else if (auto binary = dynamic_cast<BinaryNode*>(node)) {
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
                if (rVal.isZero()) {
                    llvm::errs() << "Erro: Divisão por zero em expressão constante\n";
                    return nullptr;
                }
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
            if (op == "==") return ConstantInt::get(*context, APInt(1, lVal == rVal));
            if (op == "!=") return ConstantInt::get(*context, APInt(1, lVal != rVal));
            if (op == "<") return ConstantInt::get(*context, APInt(1, lVal.slt(rVal)));
            if (op == ">") return ConstantInt::get(*context, APInt(1, lVal.sgt(rVal)));
            if (op == "<=") return ConstantInt::get(*context, APInt(1, lVal.sle(rVal)));
            if (op == ">=") return ConstantInt::get(*context, APInt(1, lVal.sge(rVal)));
        }
        // Operações com ponteiros (assumindo strings)
        else if (L->getType()->isPointerTy() && R->getType()->isPointerTy()) {
            if (op == "+") {
                // Concatenação de strings - apenas para literais conhecidos
                if (auto leftGV = dyn_cast<GlobalVariable>(L)) {
                    if (auto rightGV = dyn_cast<GlobalVariable>(R)) {
                        if (leftGV->hasInitializer() && rightGV->hasInitializer()) {
                            Constant* leftInit = leftGV->getInitializer();
                            Constant* rightInit = rightGV->getInitializer();
                            
                            // Verificar se são arrays de caracteres
                            if (leftInit->getType()->isArrayTy() && 
                                rightInit->getType()->isArrayTy()) {
                                
                                if (auto leftArray = dyn_cast<ConstantDataArray>(leftInit)) {
                                    if (auto rightArray = dyn_cast<ConstantDataArray>(rightInit)) {
                                        std::string leftVal = leftArray->getAsString().str();
                                        std::string rightVal = rightArray->getAsString().str();
                                        std::string result = leftVal + rightVal;
                                        return createStringLiteral(result);
                                    }
                                }
                            }
                        }
                    }
                }
                llvm::errs() << "Erro: Concatenação de strings não suportada em contexto constante\n";
                return nullptr;
            }
        }
        
        llvm::errs() << "Erro: Operação '" << op << "' não suportada em contexto constante\n";
        return nullptr;
    }
    else if (auto unary = dynamic_cast<UnaryNode*>(node)) {
        Constant* operand = evaluateConstantExpression(unary->getOperand());
        if (!operand) return nullptr;

        const std::string& op = unary->getOp();

        if (op == "-") {
            if (operand->getType()->isFloatTy()) {
                APFloat floatVal = cast<ConstantFP>(operand)->getValueAPF();
                floatVal.changeSign();
                return ConstantFP::get(*context, floatVal);
            } else if (operand->getType()->isIntegerTy(32)) {
                APInt intVal = cast<ConstantInt>(operand)->getValue();
                return ConstantInt::get(*context, -intVal);
            } else {
                llvm::errs() << "Erro: Operador '-' não suportado para este tipo em contexto constante\n";
                return nullptr;
            }
        } 
        else if (op == "!") {
            if (operand->getType()->isIntegerTy(1)) {
                APInt boolVal = cast<ConstantInt>(operand)->getValue();
                return ConstantInt::get(*context, APInt(1, boolVal == 0 ? 1 : 0));
            } else if (operand->getType()->isIntegerTy(32)) {
                APInt intVal = cast<ConstantInt>(operand)->getValue();
                return ConstantInt::get(*context, APInt(1, intVal == 0 ? 1 : 0));
            } else {
                llvm::errs() << "Erro: Operador '!' não suportado para este tipo em contexto constante\n";
                return nullptr;
            }
        } 
        else if (op == "~") {
            if (operand->getType()->isIntegerTy(32)) {
                APInt intVal = cast<ConstantInt>(operand)->getValue();
                return ConstantInt::get(*context, ~intVal);
            } else {
                llvm::errs() << "Erro: Operador '~' não suportado para este tipo em contexto constante\n";
                return nullptr;
            }
        }

        llvm::errs() << "Erro: Operador unário '" << op << "' não suportado em contexto constante\n";
        return nullptr;
    }
    
    llvm::errs() << "Erro: Tipo de nó não suportado em expressão constante\n";
    return nullptr;
}

void LLVMGenerator::handleGlobalVariables(ProgramNode* program) {
    // Primeiro passe: cria todas as globais com valores padrão
    for (const auto& decl : program->getDeclarations()) {
        if (auto varNode = dynamic_cast<VarNode*>(decl.get())) {
            Type* type = getLLVMType(varNode->getType());
            Constant* initVal = nullptr;
            
            // Valores padrão baseados no tipo
            if (type->isFloatTy()) {
                initVal = ConstantFP::get(*context, APFloat(0.0f));
            } else if (type->isIntegerTy(32)) {
                initVal = ConstantInt::get(*context, APInt(32, 0));
            } else if (type->isIntegerTy(1)) {
                initVal = ConstantInt::get(*context, APInt(1, 0));
            } else if (type->isPointerTy()) {
                // Para ponteiros opacos, usar null pointer
                initVal = ConstantPointerNull::get(cast<PointerType>(type));
            } else {
                llvm::errs() << "Erro: Tipo não suportado para variável global: " 
                            << varNode->getType() << "\n";
                continue;
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
                    // Verificar compatibilidade de tipos
                    Type* expectedType = globalTypes[varNode->getName()];
                    if (initVal->getType() != expectedType) {
                        // Tentar conversão para alguns casos
                        if (expectedType->isFloatTy() && initVal->getType()->isIntegerTy(32)) {
                            if (auto intVal = dyn_cast<ConstantInt>(initVal)) {
                                initVal = ConstantFP::get(*context, APFloat(static_cast<float>(intVal->getSExtValue())));
                            }
                        } else if (expectedType->isIntegerTy(32) && initVal->getType()->isFloatTy()) {
                            if (auto floatVal = dyn_cast<ConstantFP>(initVal)) {
                                APFloat floatValue = floatVal->getValueAPF();
                                initVal = ConstantInt::get(*context, APInt(32, static_cast<int32_t>(floatValue.convertToFloat())));
                            }
                        } else if (expectedType->isIntegerTy(1) && initVal->getType()->isIntegerTy(32)) {
                            if (auto intVal = dyn_cast<ConstantInt>(initVal)) {
                                initVal = ConstantInt::get(*context, APInt(1, intVal->getZExtValue() != 0));
                            }
                        }
                        
                        if (initVal->getType() != expectedType) {
                            llvm::errs() << "Erro: Tipo incompatível para variável global '" 
                                        << varNode->getName() << "'. Esperado: " << *expectedType
                                        << ", obtido: " << *initVal->getType() << "\n";
                            continue;
                        }
                    }
                    
                    globalValues[varNode->getName()]->setInitializer(initVal);
                } else {
                    llvm::errs() << "Aviso: Expressão não constante para variável global '" 
                                << varNode->getName() << "'. Mantendo valor padrão.\n";
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
    } else if (auto strNode = dynamic_cast<StringNode*>(node)) {
        return generateString(strNode);
    } else if (auto unary = dynamic_cast<UnaryNode*>(node)) {
        return generateUnaryExpr(unary);
    } else if (auto assign = dynamic_cast<AssignNode*>(node)) {
        return generateAssign(assign);
    }

    return nullptr;
}

Value* LLVMGenerator::generateUnaryExpr(UnaryNode* node) {
    Value* operand = generateExpr(node->getOperand());
    if (!operand) return nullptr;

    const std::string& op = node->getOp();
    Type* type = operand->getType();

    if (op == "-") {
        if (type->isFloatTy()) {
            return builder->CreateFNeg(operand, "negtmp");
        } else if (type->isIntegerTy(32)) {
            return builder->CreateNeg(operand, "negtmp");
        } else {
            llvm::errs() << "Erro: Operador '-' não suportado para este tipo\n";
            return nullptr;
        }
    } else if (op == "!") {
        if (type->isIntegerTy(1)) {
            return builder->CreateNot(operand, "nottmp");
        } else if (type->isIntegerTy(32)) {
            // Converta para booleano primeiro
            Value* boolVal = builder->CreateICmpNE(
                operand, 
                ConstantInt::get(*context, APInt(32, 0)), 
                "booltmp");
            return builder->CreateNot(boolVal, "nottmp");
        } else {
            llvm::errs() << "Erro: Operador '!' não suportado para este tipo\n";
            return nullptr;
        }
    } else if (op == "~") {
        if (type->isIntegerTy(32)) {
            return builder->CreateNot(operand, "bwnottmp");
        } else {
            llvm::errs() << "Erro: Operador '~' não suportado para este tipo\n";
            return nullptr;
        }
    }

    llvm::errs() << "Operador unário desconhecido: " << op << "\n";
    return nullptr;
}

Value* LLVMGenerator::generateAssign(AssignNode* node) {
    const std::string& identifier = node->getIdentifier();
    Value* target = nullptr;
    Type* targetType = nullptr;

    // Primeiro verificar se é variável local
    if (namedValues.count(identifier)) {
        target = namedValues[identifier];
        targetType = variableTypes[identifier];
    }
    // Depois verificar se é variável global
    else if (globalValues.count(identifier)) {
        target = globalValues[identifier];
        targetType = globalTypes[identifier];
    }
    else {
        llvm::errs() << "Erro: Variável '" << identifier << "' não declarada\n";
        return nullptr;
    }

    // Gerar valor da expressão
    Value* value = generateExpr(node->getValueExpr());
    if (!value) {
        llvm::errs() << "Erro: Expressão de atribuição inválida\n";
        return nullptr;
    }

    // Verificar compatibilidade de tipos e fazer conversões necessárias
    if (value->getType() != targetType) {
        if (targetType->isIntegerTy(32) && value->getType()->isIntegerTy(1)) {
            value = builder->CreateZExt(value, targetType, "intconv");
        } else if (targetType->isIntegerTy(1) && value->getType()->isIntegerTy(32)) {
            value = builder->CreateICmpNE(value, ConstantInt::get(*context, APInt(32, 0)), "boolconv");
        } else if (targetType->isFloatTy() && value->getType()->isIntegerTy(32)) {
            value = builder->CreateSIToFP(value, targetType, "floatconv");
        } else if (targetType->isIntegerTy(32) && value->getType()->isFloatTy()) {
            value = builder->CreateFPToSI(value, targetType, "intconv");
        } else {
            llvm::errs() << "Erro: Tipos incompatíveis na atribuição para '" 
                        << identifier << "'. Esperado: " << *targetType
                        << ", obtido: " << *value->getType() << "\n";
            return nullptr;
        }
    }

    // Fazer o store
    builder->CreateStore(value, target);
    return value;
}


Value* LLVMGenerator::generateBool(BoolNode* node) {
    return ConstantInt::get(*context, APInt(1, node->getValue() ? 1 : 0));
}

Value* LLVMGenerator::generateString(StringNode* node) {
    GlobalVariable* strArray = createStringLiteral(node->getValue());
    
    // Obter ponteiro para o primeiro caractere
    Constant* zero = ConstantInt::get(Type::getInt32Ty(*context), 0);
    Constant* indices[] = {zero, zero};
    return ConstantExpr::getGetElementPtr(
        strArray->getValueType(),
        strArray,
        indices,
        true
    );
}

Value* LLVMGenerator::generateBinaryExpr(BinaryNode* node) {
    Value* L = generateExpr(node->getLeft());
    Value* R = generateExpr(node->getRight());
    
    if (!L || !R) return nullptr;

    Type* type = L->getType();
    const std::string& op = node->getOp();
    
    // Verificar se estamos lidando com strings
    if (L->getType()->isPointerTy() && R->getType()->isPointerTy()) {
        if (op == "+") {
            // Concatenação de strings - precisaria de uma função de runtime
            llvm::errs() << "Aviso: Concatenação de strings não implementada completamente\n";
            return L; // Retorna a primeira string como placeholder
        } else if (op == "==") {
            // Comparação de strings - precisaria de strcmp
            llvm::errs() << "Aviso: Comparação de strings não implementada completamente\n";
            return ConstantInt::get(*context, APInt(1, 0)); // Retorna false como placeholder
        }
    }
    
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
            } else if (returnType->isPointerTy()) {
                retVal = ConstantPointerNull::get(cast<PointerType>(returnType));
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
    if (typeName == "string") return Type::getInt8PtrTy(*context);
    return Type::getVoidTy(*context);
}

void LLVMGenerator::dumpIR() const {
    module->print(llvm::errs(), nullptr);
}