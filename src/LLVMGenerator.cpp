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
        
        // Gera todas as funções EXCETO main
        for (const auto& decl : programNode->getDeclarations()) {
            if (auto funcNode = dynamic_cast<FunctionNode*>(decl.get())) {
                if (funcNode->getName() != "main") {
                    generateFunction(funcNode);
                }
            }
        }
        
        // Gera a função main apenas se não existir ainda
        if (!module->getFunction("main")) {
            generateMainFunction(astRoot);
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
        // Primeiro: processar variáveis globais (se houver)
        for (const auto& decl : programNode->getDeclarations()) {
            if (auto varNode = dynamic_cast<VarNode*>(decl.get())) {
                // Variável global - já foi processada em handleGlobalVariables
                // Mas precisamos garantir que existe e pode ser usada
                if (globalValues.count(varNode->getName())) {
                    // Variável global existe, pode ser usada
                }
            }
        }
        
        // Segundo: processar o código do programa principal
        for (const auto& decl : programNode->getDeclarations()) {
            if (auto blockNode = dynamic_cast<BlockNode*>(decl.get())) {
                generateBlock(blockNode);
            }
        }
    }
    
    // Se a função não terminou com um return, adicionar return 0
    BasicBlock* currentBB = builder->GetInsertBlock();
    if (!currentBB->getTerminator()) {
        builder->CreateRet(ConstantInt::get(*context, APInt(32, 0)));
    }
    
    verifyFunction(*mainFunc);
    currentFunction = nullptr;
}

Value* LLVMGenerator::generateBlock(BlockNode* node) {
    // Salvar o estado atual das tabelas de símbolos
    std::unordered_map<std::string, llvm::Value*> oldNamedValues = namedValues;
    std::unordered_map<std::string, llvm::Type*> oldVariableTypes = variableTypes;
    
    // Processar declarações locais primeiro
    for (const auto& decl : node->getLocalDeclarations()) {
        generateLocalVariable(decl.get());
    }
    
    // Processar statements
    Value* lastValue = nullptr;
    BasicBlock* currentBB = builder->GetInsertBlock();
    
    for (const auto& stmt : node->getStatements()) {
        // Verificar se o bloco atual já tem um terminador
        if (currentBB->getTerminator()) {
            break; // Não processar mais statements se já temos um terminador
        }
        
        lastValue = generateExpr(stmt.get());
        if (!lastValue) {
            break;
        }
        
        // Atualizar referência do bloco atual
        currentBB = builder->GetInsertBlock();
    }
    
    // Restaurar tabelas de símbolos (escopo local termina)
    namedValues = oldNamedValues;
    variableTypes = oldVariableTypes;
    
    return lastValue;
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
    } else if (auto block = dynamic_cast<BlockNode*>(node)) {
	return generateBlock(block);
    } else if (auto breakNode = dynamic_cast<BreakNode*>(node)) {
        return generateBreak(breakNode);
    } else if (auto continueNode = dynamic_cast<ContinueNode*>(node)) {
        return generateContinue(continueNode);
    } else if (auto forNode = dynamic_cast<ForNode*>(node)) {
        return generateFor(forNode);
    } else if (auto ifNode = dynamic_cast<IfNode*>(node)) {
        return generateIf(ifNode);
    } else if (auto returnNode = dynamic_cast<ReturnNode*>(node)) {
        return generateReturn(returnNode);
    } else if (auto whileNode = dynamic_cast<WhileNode*>(node)) {
        return generateWhile(whileNode);
    } else if (auto call = dynamic_cast<CallNode*>(node)) {
        return generateCall(call);
    } else if (auto funcNode = dynamic_cast<FunctionNode*>(node)) {
        generateFunction(funcNode);
        return nullptr;
    }

    return nullptr;
}

Value* LLVMGenerator::generateReturn(ReturnNode* node) {
    if (!currentFunction) {
        llvm::errs() << "Erro: 'return' fora de uma função\n";
        return nullptr;
    }
    
    Type* returnType = currentFunction->getReturnType();
    
    if (node->getExpr()) {
        // Return com expressão
        Value* retValue = generateExpr(node->getExpr()); // CORRIGIDO: removido .get()
        if (!retValue) {
            llvm::errs() << "Erro: Expressão de return inválida\n";
            return nullptr;
        }
        
        // Verificar compatibilidade de tipos
        if (retValue->getType() != returnType) {
            // Conversões de tipo necessárias
            if (returnType->isIntegerTy(32) && retValue->getType()->isIntegerTy(1)) {
                retValue = builder->CreateZExt(retValue, returnType, "retconv");
            } else if (returnType->isIntegerTy(1) && retValue->getType()->isIntegerTy(32)) {
                retValue = builder->CreateICmpNE(retValue, ConstantInt::get(*context, APInt(32, 0)), "retconv");
            } else if (returnType->isFloatTy() && retValue->getType()->isIntegerTy(32)) {
                retValue = builder->CreateSIToFP(retValue, returnType, "retconv");
            } else if (returnType->isIntegerTy(32) && retValue->getType()->isFloatTy()) {
                retValue = builder->CreateFPToSI(retValue, returnType, "retconv");
            } else {
                llvm::errs() << "Erro: Tipo de retorno incompatível. Esperado: " 
                            << *returnType << ", obtido: " << *retValue->getType() << "\n";
                return nullptr;
            }
        }
        
        builder->CreateRet(retValue);
        return retValue;
    } else {
        // Return void
        if (!returnType->isVoidTy()) {
            llvm::errs() << "Erro: Função não-void retornando sem valor\n";
            return nullptr;
        }
        
        builder->CreateRetVoid();
        return nullptr;
    }
}

Value* LLVMGenerator::generateWhile(WhileNode* node) {
    // Salvar o estado atual das tabelas de símbolos para o escopo do loop
    std::unordered_map<std::string, llvm::Value*> oldNamedValues = namedValues;
    std::unordered_map<std::string, llvm::Type*> oldVariableTypes = variableTypes;
    
    Function* function = builder->GetInsertBlock()->getParent();
    BasicBlock* currentBB = builder->GetInsertBlock();
    
    // Criar blocos básicos para o loop
    BasicBlock* condBB = BasicBlock::Create(*context, "while.cond", function);
    BasicBlock* bodyBB = BasicBlock::Create(*context, "while.body", function);
    BasicBlock* afterBB = BasicBlock::Create(*context, "while.end", function);
    
    // Saltar para o bloco de condição
    builder->CreateBr(condBB);
    
    // Bloco de condição
    builder->SetInsertPoint(condBB);
    
    // Gerar a condição
    Value* condValue = generateExpr(node->getCondition());
    if (!condValue) {
        llvm::errs() << "Erro: Condição do while inválida\n";
        return nullptr;
    }
    
    // Converter para booleano se necessário
    if (!condValue->getType()->isIntegerTy(1)) {
        condValue = builder->CreateICmpNE(
            condValue, 
            ConstantInt::get(condValue->getType(), 0), 
            "whilecond");
    }
    
    builder->CreateCondBr(condValue, bodyBB, afterBB);
    
    // Bloco do corpo
    builder->SetInsertPoint(bodyBB);
    
    // Empilhar blocos de break/continue
    pushLoopBlocks(afterBB, condBB);
    
    // Gerar o corpo do loop
    if (node->getBody()) {
        generateExpr(node->getBody());
    }
    
    // Desempilhar blocos de break/continue
    popLoopBlocks();
    
    // Saltar de volta para verificar a condição
    builder->CreateBr(condBB);
    
    // Bloco após o loop
    builder->SetInsertPoint(afterBB);
    
    // Restaurar tabelas de símbolos (escopo do loop termina)
    namedValues = oldNamedValues;
    variableTypes = oldVariableTypes;
    
    return ConstantInt::get(*context, APInt(32, 0)); // Retorno dummy
}

Value* LLVMGenerator::generateIf(IfNode* node) {
    // Gerar a condição
    Value* condValue = generateExpr(node->getCondition());
    if (!condValue) {
        llvm::errs() << "Erro: Condição do if inválida\n";
        return nullptr;
    }
    
    // Converter para booleano se necessário
    if (!condValue->getType()->isIntegerTy(1)) {
        condValue = builder->CreateICmpNE(
            condValue, 
            ConstantInt::get(condValue->getType(), 0), 
            "ifcond");
    }
    
    Function* function = builder->GetInsertBlock()->getParent();
    BasicBlock* currentBB = builder->GetInsertBlock();
    
    // Criar blocos básicos
    BasicBlock* thenBB = BasicBlock::Create(*context, "then", function);
    BasicBlock* elseBB = node->getElseBlock() ? 
                         BasicBlock::Create(*context, "else", function) : nullptr;
    BasicBlock* mergeBB = BasicBlock::Create(*context, "ifcont", function);
    
    // Criar branch condicional
    if (elseBB) {
        builder->CreateCondBr(condValue, thenBB, elseBB);
    } else {
        builder->CreateCondBr(condValue, thenBB, mergeBB);
    }
    
    // Gerar bloco then
    builder->SetInsertPoint(thenBB);
    Value* thenValue = generateExpr(node->getThenBlock());
    thenBB = builder->GetInsertBlock();
    
    // Só criar branch para merge se o bloco then não terminou
    if (!thenBB->getTerminator()) {
        builder->CreateBr(mergeBB);
    }
    
    // Gerar bloco else se existir
    Value* elseValue = nullptr;
    if (elseBB) {
        builder->SetInsertPoint(elseBB);
        elseValue = generateExpr(node->getElseBlock());
        elseBB = builder->GetInsertBlock();
        
        // Só criar branch para merge se o bloco else não terminou
        if (!elseBB->getTerminator()) {
            builder->CreateBr(mergeBB);
        }
    }
    
    // Só continuar no bloco merge se ele for alcançável
    bool mergeReachable = (!thenBB->getTerminator() || isa<BranchInst>(thenBB->getTerminator())) &&
                          (!elseBB || !elseBB->getTerminator() || isa<BranchInst>(elseBB->getTerminator()));
    
    if (mergeReachable) {
        builder->SetInsertPoint(mergeBB);
        return thenValue ? thenValue : elseValue;
    } else {
        // Se o merge não é alcançável, remover e retornar null
        mergeBB->eraseFromParent();
        return nullptr;
    }
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



Value* LLVMGenerator::generateBreak(BreakNode* node) {
    if (breakBlocks.empty()) {
        llvm::errs() << "Aviso: 'break' fora de um loop - ignorando\n";
        return ConstantInt::get(*context, APInt(32, 0)); // Retorna dummy
    }
    
    // Criar salto para o bloco de break
    BasicBlock* breakBlock = breakBlocks.back();
    builder->CreateBr(breakBlock);
    
    // Criar um bloco vazio após o break para evitar instruções órfãs
    BasicBlock* afterBreak = BasicBlock::Create(*context, "afterbreak", currentFunction);
    builder->SetInsertPoint(afterBreak);
    
    return ConstantInt::get(*context, APInt(32, 0)); // Retorno dummy
}

Value* LLVMGenerator::generateContinue(ContinueNode* node) {
    if (continueBlocks.empty()) {
        llvm::errs() << "Aviso: 'continue' fora de um loop - ignorando\n";
        return ConstantInt::get(*context, APInt(32, 0)); // Retorna dummy
    }
    
    // Criar salto para o bloco de continue
    BasicBlock* continueBlock = continueBlocks.back();
    builder->CreateBr(continueBlock);
    
    // Criar um bloco vazio após o continue para evitar instruções órfãs
    BasicBlock* afterContinue = BasicBlock::Create(*context, "aftercontinue", currentFunction);
    builder->SetInsertPoint(afterContinue);
    
    return ConstantInt::get(*context, APInt(32, 0)); // Retorno dummy
}

void LLVMGenerator::pushLoopBlocks(BasicBlock* breakBlock, BasicBlock* continueBlock) {
    breakBlocks.push_back(breakBlock);
    continueBlocks.push_back(continueBlock);
}

Value* LLVMGenerator::generateFor(ForNode* node) {
    // Salvar o estado atual das tabelas de símbolos para o escopo do loop
    std::unordered_map<std::string, llvm::Value*> oldNamedValues = namedValues;
    std::unordered_map<std::string, llvm::Type*> oldVariableTypes = variableTypes;
    
    // Criar blocos básicos para o loop
    BasicBlock* preheaderBB = builder->GetInsertBlock();
    BasicBlock* loopBB = BasicBlock::Create(*context, "loop", currentFunction);
    BasicBlock* bodyBB = BasicBlock::Create(*context, "body", currentFunction);
    BasicBlock* updateBB = BasicBlock::Create(*context, "update", currentFunction);
    BasicBlock* afterLoopBB = BasicBlock::Create(*context, "afterloop", currentFunction);
    
    // Gerar a expressão de inicialização (se existir)
    if (node->getInit()) {
        generateExpr(node->getInit());
    }
    
    // Saltar para o bloco do loop
    builder->CreateBr(loopBB);
    
    // Bloco do loop: verificar condição
    builder->SetInsertPoint(loopBB);
    
    Value* condValue = nullptr;
    if (node->getCond()) {
        condValue = generateExpr(node->getCond());
        if (!condValue) {
            llvm::errs() << "Erro: Condição do loop for inválida\n";
            return nullptr;
        }
        
        // Converter para booleano se necessário
        if (!condValue->getType()->isIntegerTy(1)) {
            condValue = builder->CreateICmpNE(
                condValue, 
                ConstantInt::get(condValue->getType(), 0), 
                "loopcond");
        }
    } else {
        // Se não há condição, loop infinito (condição sempre verdadeira)
        condValue = ConstantInt::get(Type::getInt1Ty(*context), 1);
    }
    
    builder->CreateCondBr(condValue, bodyBB, afterLoopBB);
    
    // Bloco do corpo
    builder->SetInsertPoint(bodyBB);
    
    // Empilhar blocos de break/continue
    pushLoopBlocks(afterLoopBB, updateBB);
    
    // Gerar o corpo do loop
    if (node->getBody()) {
        generateBlock(node->getBody());
    }
    
    // Desempilhar blocos de break/continue
    popLoopBlocks();
    
    // Saltar para o bloco de atualização
    builder->CreateBr(updateBB);
    
    // Bloco de atualização
    builder->SetInsertPoint(updateBB);
    
    // Gerar expressão de atualização (se existir)
    if (node->getUpdate()) {
        generateExpr(node->getUpdate());
    }
    
    // Voltar para verificar a condição
    builder->CreateBr(loopBB);
    
    // Bloco após o loop
    builder->SetInsertPoint(afterLoopBB);
    
    // Restaurar tabelas de símbolos (escopo do loop termina)
    namedValues = oldNamedValues;
    variableTypes = oldVariableTypes;
    
    return ConstantInt::get(*context, APInt(32, 0)); // Retorno dummy
}

void LLVMGenerator::popLoopBlocks() {
    if (!breakBlocks.empty()) breakBlocks.pop_back();
    if (!continueBlocks.empty()) continueBlocks.pop_back();
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

Value* LLVMGenerator::generateCall(CallNode* node) {
    const std::string& funcName = node->getFunctionName();
    
    // Verificar se a função existe
    Function* callee = module->getFunction(funcName);
    if (!callee) {
        llvm::errs() << "Erro: Função '" << funcName << "' não declarada\n";
        return nullptr;
    }
    
    // Verificar número de argumentos
    if (callee->arg_size() != node->getArguments().size()) {
        llvm::errs() << "Erro: Número de argumentos incorreto para função '" 
                    << funcName << "'. Esperado: " << callee->arg_size()
                    << ", obtido: " << node->getArguments().size() << "\n";
        return nullptr;
    }
    
    // Gerar valores dos argumentos
    std::vector<Value*> args;
    unsigned idx = 0;
    for (const auto& argExpr : node->getArguments()) {
        Value* argValue = generateExpr(argExpr.get());
        if (!argValue) {
            llvm::errs() << "Erro: Argumento " << idx << " inválido para função '" 
                        << funcName << "'\n";
            return nullptr;
        }
        
        // Verificar compatibilidade de tipos e fazer conversões
        Type* expectedType = callee->getArg(idx)->getType();
        if (argValue->getType() != expectedType) {
            if (expectedType->isIntegerTy(32) && argValue->getType()->isIntegerTy(1)) {
                argValue = builder->CreateZExt(argValue, expectedType, "argconv");
            } else if (expectedType->isIntegerTy(1) && argValue->getType()->isIntegerTy(32)) {
                argValue = builder->CreateICmpNE(argValue, ConstantInt::get(*context, APInt(32, 0)), "argconv");
            } else if (expectedType->isFloatTy() && argValue->getType()->isIntegerTy(32)) {
                argValue = builder->CreateSIToFP(argValue, expectedType, "argconv");
            } else if (expectedType->isIntegerTy(32) && argValue->getType()->isFloatTy()) {
                argValue = builder->CreateFPToSI(argValue, expectedType, "argconv");
            } else {
                llvm::errs() << "Erro: Tipo de argumento " << idx << " incompatível para função '" 
                            << funcName << "'. Esperado: " << *expectedType
                            << ", obtido: " << *argValue->getType() << "\n";
                return nullptr;
            }
        }
        
        args.push_back(argValue);
        idx++;
    }
    
    // Criar a chamada
    if (callee->getReturnType()->isVoidTy()) {
        return builder->CreateCall(callee, args);
    } else {
        return builder->CreateCall(callee, args, "calltmp");
    }
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

    // Salvar o estado atual das tabelas de símbolos
    std::unordered_map<std::string, llvm::Value*> oldNamedValues = std::move(namedValues);
    std::unordered_map<std::string, llvm::Type*> oldVariableTypes = std::move(variableTypes);
    
    namedValues.clear();
    variableTypes.clear();

    std::vector<Type*> paramTypes;
    for (const auto& param : node->getParams()) {
        Type* paramType = getLLVMType(param.first);
        if (!paramType) {
            llvm::errs() << "Erro: Tipo de parâmetro desconhecido\n";
            namedValues = std::move(oldNamedValues);
            variableTypes = std::move(oldVariableTypes);
            return;
        }
        paramTypes.push_back(paramType);
    }
    
    Type* returnType = getLLVMType(node->getReturnType());
    if (!returnType) {
        llvm::errs() << "Erro: Tipo de retorno desconhecido\n";
        namedValues = std::move(oldNamedValues);
        variableTypes = std::move(oldVariableTypes);
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
        Value* bodyValue = nullptr;
        
        if (auto blockNode = dynamic_cast<BlockNode*>(node->getBody())) {
            bodyValue = generateBlock(blockNode);
        } else {
            bodyValue = generateExpr(node->getBody());
        }
        
        // Se a função não terminou com um return, adicionar return apropriado
        BasicBlock* currentBB = builder->GetInsertBlock();
        if (!currentBB->getTerminator()) {
            if (returnType->isVoidTy()) {
                builder->CreateRetVoid();
            } else {
                // Retornar valor padrão baseado no tipo
                Value* defaultRet = nullptr;
                if (returnType->isIntegerTy(32)) {
                    defaultRet = ConstantInt::get(*context, APInt(32, 0));
                } else if (returnType->isFloatTy()) {
                    defaultRet = ConstantFP::get(*context, APFloat(0.0f));
                } else if (returnType->isIntegerTy(1)) {
                    defaultRet = ConstantInt::get(*context, APInt(1, 0));
                } else if (returnType->isPointerTy()) {
                    defaultRet = ConstantPointerNull::get(cast<PointerType>(returnType));
                }
                
                if (defaultRet) {
                    builder->CreateRet(defaultRet);
                } else {
                    builder->CreateUnreachable();
                }
            }
        }
    } else {
        // Função sem corpo
        if (returnType->isVoidTy()) {
            builder->CreateRetVoid();
        } else {
            builder->CreateUnreachable();
        }
    }
    
    if (verifyFunction(*func, &llvm::errs())) {
        llvm::errs() << "Erro: Função '" << node->getName() << "' inválida\n";
        func->eraseFromParent();
    }
    
    currentFunction = nullptr;
    namedValues = std::move(oldNamedValues);
    variableTypes = std::move(oldVariableTypes);
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

void LLVMGenerator::dumpIRToFile(const std::string& filename) const {
    std::error_code EC;
    llvm::raw_fd_ostream out(filename, EC);
    if (EC) {
        llvm::errs() << "Erro ao abrir arquivo: " << EC.message() << "\n";
        return;
    }
    module->print(out, nullptr);
    out.close();
}
