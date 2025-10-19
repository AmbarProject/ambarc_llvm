#include "LLVMGenerator.hpp"
#include <llvm/IR/Verifier.h>
#include <memory>

using namespace ambar;
using namespace llvm;

LLVMGenerator::LLVMGenerator(OptimizationLevel level) 
    : optLevel(level) {
    context = std::make_unique<LLVMContext>();
    module = std::make_unique<Module>("ambar", *context);
    builder = std::make_unique<IRBuilder<>>(*context);
    currentFunction = nullptr;
    
    // Declarar printf
    declarePrintfFunction();
}

// Implementação do OptimizationAnalyzer
std::vector<OptimizationAnalyzer::ProblematicNode> 
OptimizationAnalyzer::analyzeOptimizationProblems() {
    std::vector<ProblematicNode> problems;
    
    for (llvm::Function& F : module) {
        if (!F.isDeclaration() && !F.empty()) {
            analyzeFunction(F, problems);
        }
    }
    
    return problems;
}

void OptimizationAnalyzer::analyzeFunction(llvm::Function& F, 
                                         std::vector<ProblematicNode>& problems) {
    // Usar AnalysisManager para obter LoopInfo
    llvm::LoopAnalysisManager LAM;
    llvm::FunctionAnalysisManager FAM;
    llvm::ModuleAnalysisManager MAM;
    
    llvm::PassBuilder PB;
    PB.registerFunctionAnalyses(FAM);
    
    auto& LI = FAM.getResult<llvm::LoopAnalysis>(F);
    
    for (llvm::BasicBlock& BB : F) {
        analyzeBasicBlock(BB, F, problems);
    }
}

void OptimizationAnalyzer::analyzeBasicBlock(llvm::BasicBlock& BB, 
                                           llvm::Function& F,
                                           std::vector<ProblematicNode>& problems) {
    std::vector<std::string> blockProblems;
    int totalSeverity = 0;
    
    // Análise de computações redundantes
    if (hasRedundantComputations(BB)) {
        blockProblems.push_back("Computações redundantes detectadas");
        totalSeverity += 3;
    }
    
    // Análise de operações custosas
    if (hasExpensiveOperations(BB)) {
        blockProblems.push_back("Operações computacionalmente custosas");
        totalSeverity += 4;
    }
    
    // Análise de padrões de branch
    if (hasPoorBranchPattern(BB)) {
        blockProblems.push_back("Padrão de branch ineficiente");
        totalSeverity += 2;
    }
    
    // Análise de ineficiências de memória
    if (hasMemoryInefficiencies(BB)) {
        blockProblems.push_back("Ineficiências de acesso à memória");
        totalSeverity += 3;
    }
    
    // Análise de loops
    if (hasLoopInefficiencies(BB, F)) {
        blockProblems.push_back("Ineficiências em estruturas de loop");
        totalSeverity += 5;
    }
    
    // Se encontramos problemas, adicionar ao relatório
    if (!blockProblems.empty() && totalSeverity >= 3) {
        ProblematicNode node;
        node.functionName = F.getName().str();
        node.blockName = BB.getName().str();
        node.block = &BB;
        node.severity = std::min(10, totalSeverity);
        
        // Criar descrição consolidada
        std::stringstream desc;
        desc << "Bloco " << BB.getName().str() << " na função " << F.getName().str() 
             << " possui " << blockProblems.size() << " problemas:\n";
        for (size_t i = 0; i < blockProblems.size(); ++i) {
            desc << "  " << (i + 1) << ". " << blockProblems[i] << "\n";
        }
        desc << "Severidade: " << totalSeverity << "/10";
        
        node.description = desc.str();
        node.problemType = "Múltiplas Ineficiências";
        
        problems.push_back(node);
    }
}

bool OptimizationAnalyzer::hasRedundantComputations(llvm::BasicBlock& BB) {
    int loadCount = 0;
    int storeCount = 0;
    int computationCount = 0;
    
    for (llvm::Instruction& I : BB) {
        if (llvm::isa<llvm::LoadInst>(&I)) loadCount++;
        if (llvm::isa<llvm::StoreInst>(&I)) storeCount++;
        if (llvm::isa<llvm::BinaryOperator>(&I)) computationCount++;
        
        // Verificar loads redundantes
        if (llvm::isa<llvm::LoadInst>(&I)) {
            // Simplificação: considerar muitos loads como potencial problema
            if (loadCount > 5) return true;
        }
    }
    
    // Muitas operações de memória em relação a computações
    if ((loadCount + storeCount) > computationCount * 2) {
        return true;
    }
    
    return false;
}

bool OptimizationAnalyzer::hasExpensiveOperations(llvm::BasicBlock& BB) {
    for (llvm::Instruction& I : BB) {
        // Verificar operações de divisão/modulo (são mais caras)
        if (auto* binOp = llvm::dyn_cast<llvm::BinaryOperator>(&I)) {
            llvm::Instruction::BinaryOps opcode = binOp->getOpcode();
            if (opcode == llvm::Instruction::SDiv || opcode == llvm::Instruction::UDiv ||
                opcode == llvm::Instruction::SRem || opcode == llvm::Instruction::URem ||
                opcode == llvm::Instruction::FDiv || opcode == llvm::Instruction::FRem) {
                return true;
            }
        }
        
        // Verificar conversões de tipo custosas
        if (llvm::isa<llvm::FPToSIInst>(&I) || llvm::isa<llvm::SIToFPInst>(&I) ||
            llvm::isa<llvm::FPTruncInst>(&I) || llvm::isa<llvm::FPExtInst>(&I)) {
            return true;
        }
    }
    return false;
}

bool OptimizationAnalyzer::hasPoorBranchPattern(llvm::BasicBlock& BB) {
    // Verificar se é um bloco com muitos branches condicionais
    int branchCount = 0;
    int totalInstructions = 0;
    
    for (llvm::Instruction& I : BB) {
        totalInstructions++;
        if (llvm::isa<llvm::BranchInst>(&I) || llvm::isa<llvm::SwitchInst>(&I)) {
            branchCount++;
        }
    }
    
    // Se mais de 30% das instruções são branches, pode ser problemático
    if (totalInstructions > 0 && (branchCount * 100 / totalInstructions) > 30) {
        return true;
    }
    
    return false;
}

// CORREÇÃO: Parâmetro correto
bool OptimizationAnalyzer::hasMemoryInefficiencies(llvm::BasicBlock& BB) {
    int memoryOps = 0;
    int totalInstructions = 0;
    
    for (llvm::Instruction& I : BB) {
        totalInstructions++;
        if (llvm::isa<llvm::LoadInst>(&I) || llvm::isa<llvm::StoreInst>(&I) ||
            llvm::isa<llvm::AllocaInst>(&I) || llvm::isa<llvm::GetElementPtrInst>(&I)) {
            memoryOps++;
        }
    }
    
    // Se mais de 50% são operações de memória
    if (totalInstructions > 0 && (memoryOps * 100 / totalInstructions) > 50) {
        return true;
    }
    
    return false;
}

bool OptimizationAnalyzer::hasLoopInefficiencies(llvm::BasicBlock& BB, llvm::Function& F) {
    // Verificação simplificada para blocos de loop
    llvm::LoopAnalysisManager LAM;
    llvm::FunctionAnalysisManager FAM;
    
    llvm::PassBuilder PB;
    PB.registerFunctionAnalyses(FAM);
    
    auto& LI = FAM.getResult<llvm::LoopAnalysis>(F);
    
    if (llvm::Loop* L = LI.getLoopFor(&BB)) {
        // Bloco pertence a um loop
        if (L->getHeader() == &BB) {
            // É o header do loop - verificar características problemáticas
            int invariantCount = 0;
            for (llvm::Instruction& I : BB) {
                // Verificação simplificada de invariantes de loop
                if (llvm::isa<llvm::LoadInst>(&I) || llvm::isa<llvm::BinaryOperator>(&I)) {
                    invariantCount++;
                }
            }
            
            if (invariantCount > 3) {
                return true; // Possível invariante de loop não movido
            }
        }
    }
    
    return false;
}

void OptimizationAnalyzer::printProblematicNodes(const std::vector<ProblematicNode>& problems) {
    if (problems.empty()) {
        llvm::errs() << "\n=== ANÁLISE DE OTIMIZAÇÃO ===\n";
        llvm::errs() << "Nenhum nó problemático detectado. O código está bem otimizado!\n";
        return;
    }
    
    llvm::errs() << "\n=== ANÁLISE DE OTIMIZAÇÃO - NÓS PROBLEMÁTICOS DETECTADOS ===\n\n";
    
    // Ordenar por severidade
    auto sortedProblems = problems;
    std::sort(sortedProblems.begin(), sortedProblems.end(),
              [](const ProblematicNode& a, const ProblematicNode& b) {
                  return a.severity > b.severity;
              });
    
    for (size_t i = 0; i < sortedProblems.size(); ++i) {
        const auto& problem = sortedProblems[i];
        llvm::errs() << "PROBLEMA #" << (i + 1) << " [Severidade: " << problem.severity << "/10]\n";
        llvm::errs() << "Função: " << problem.functionName << "\n";
        llvm::errs() << "Bloco: " << problem.blockName << "\n";
        llvm::errs() << "Tipo: " << problem.problemType << "\n";
        llvm::errs() << "Descrição:\n" << problem.description << "\n";
        
        // Mostrar algumas instruções do bloco problemático
        llvm::errs() << "Instruções do bloco:\n";
        int instCount = 0;
        for (const llvm::Instruction& I : *problem.block) {
            if (instCount < 5) { // Mostrar apenas as primeiras 5 instruções
                llvm::errs() << "  ";
                I.print(llvm::errs());
                llvm::errs() << "\n";
                instCount++;
            }
        }
        if (instCount >= 5) {
            llvm::errs() << "  ... (mais instruções)\n";
        }
        llvm::errs() << "----------------------------------------\n\n";
    }
    
    llvm::errs() << "Total de nós problemáticos detectados: " << problems.size() << "\n";
}

void LLVMGenerator::analyzeAndReportOptimizationProblems() {
    llvm::errs() << "\n🔍 Iniciando análise inovadora de otimização por grafos...\n";
    
    OptimizationAnalyzer analyzer(*module);
    auto problems = analyzer.analyzeOptimizationProblems();
    analyzer.printProblematicNodes(problems);
    
    // Também executar análise avançada
    performAdvancedOptimizationAnalysis();
}

void LLVMGenerator::performAdvancedOptimizationAnalysis() {
    llvm::errs() << "\n=== ANÁLISE AVANÇADA DE GRAFOS ===\n";
    
    int totalBlocks = 0;
    int totalFunctions = 0;
    int problematicBlocks = 0;
    
    for (llvm::Function& F : *module) {
        if (F.isDeclaration() || F.empty()) continue;
        
        totalFunctions++;
        
        for (llvm::BasicBlock& BB : F) {
            totalBlocks++;
            
            // Análise de complexidade do bloco
            int instructionCount = 0;
            int memoryOps = 0;
            int branches = 0;
            
            for (llvm::Instruction& I : BB) {
                instructionCount++;
                if (llvm::isa<llvm::LoadInst>(&I) || llvm::isa<llvm::StoreInst>(&I)) {
                    memoryOps++;
                }
                if (llvm::isa<llvm::BranchInst>(&I) || llvm::isa<llvm::SwitchInst>(&I)) {
                    branches++;
                }
            }
            
            // Heurísticas para identificar blocos problemáticos
            bool isProblematic = false;
            std::string problemReason;
            
            if (instructionCount > 20) {
                isProblematic = true;
                problemReason = "Bloco muito grande (" + std::to_string(instructionCount) + " instruções)";
            } else if (memoryOps > 5) {
                isProblematic = true;
                problemReason = "Muitas operações de memória (" + std::to_string(memoryOps) + ")";
            } else if (branches > 2) {
                isProblematic = true;
                problemReason = "Muitos branches (" + std::to_string(branches) + ")";
            }
            
            if (isProblematic) {
                problematicBlocks++;
                llvm::errs() << "⚠️  NÓ MAL OTIMIZADO DETECTADO:\n";
                llvm::errs() << "   Função: " << F.getName().str() << "\n";
                llvm::errs() << "   Bloco: " << BB.getName().str() << "\n";
                llvm::errs() << "   Problema: " << problemReason << "\n";
                llvm::errs() << "   Instruções: " << instructionCount 
                           << ", Memória: " << memoryOps 
                           << ", Branches: " << branches << "\n\n";
            }
        }
    }
    
    // Estatísticas finais
    llvm::errs() << "=== RELATÓRIO FINAL ===\n";
    llvm::errs() << "Funções analisadas: " << totalFunctions << "\n";
    llvm::errs() << "Blocos totais: " << totalBlocks << "\n";
    llvm::errs() << "Blocos problemáticos: " << problematicBlocks << "\n";
    
    if (totalBlocks > 0) {
        double problematicPercentage = (problematicBlocks * 100.0) / totalBlocks;
        llvm::errs() << "Taxa de problemas: " << problematicPercentage << "%\n";
        
        if (problematicPercentage > 20.0) {
            llvm::errs() << "❌ ALTA TAXA DE PROBLEMAS: Considere reestruturar o código!\n";
        } else if (problematicPercentage > 10.0) {
            llvm::errs() << "⚠️  TAXA MODERADA DE PROBLEMAS: Algumas otimizações são recomendadas\n";
        } else {
            llvm::errs() << "✅ BAIXA TAXA DE PROBLEMAS: Código bem estruturado!\n";
        }
    }
}

void LLVMGenerator::declarePrintfFunction() {
    // Tipo de printf: int printf(i8*, ...)
    FunctionType* printfType = FunctionType::get(
        Type::getInt32Ty(*context),
        {PointerType::get(Type::getInt8Ty(*context), 0)},
        true  // varargs
    );
    
    printfFunc = Function::Create(
        printfType,
        Function::ExternalLinkage,
        "printf",
        *module
    );
}

// CORREÇÃO COMPLETA do generateArrayLiteral
llvm::Value* LLVMGenerator::generateArrayLiteral(ArrayNode* node) {
    const auto& elements = node->getElements();
    
    // DEBUG: Verificar o que estamos recebendo
    llvm::errs() << "DEBUG: Gerando array literal com " << elements.size() << " elementos\n";
    
    // Obter tipo do elemento
    Type* elemType = getLLVMType(node->getElementType());
    if (!elemType) {
        llvm::errs() << "Erro: Tipo de elemento desconhecido: " << node->getElementType() << "\n";
        return nullptr;
    }
    
    if (!currentFunction) {
        llvm::errs() << "Erro: Array literal fora de função\n";
        return nullptr;
    }
    
    // CORREÇÃO: Criar array com tamanho exato dos elementos
    ArrayType* arrayType = ArrayType::get(elemType, elements.size());
    
    // CORREÇÃO: Alocar na stack
    AllocaInst* arrayStorage = builder->CreateAlloca(arrayType, nullptr, "array.storage");
    
    // CORREÇÃO: Inicializar cada elemento
    for (size_t i = 0; i < elements.size(); ++i) {
        Value* elementValue = generateExpr(elements[i].get());
        if (!elementValue) {
            llvm::errs() << "Erro: Não foi possível gerar elemento " << i << " do array\n";
            return nullptr;
        }
        
        // DEBUG
        llvm::errs() << "DEBUG: Elemento " << i << " - tipo: " << *elementValue->getType() << "\n";
        
        // Calcular ponteiro para este elemento
        Value* indices[] = {
            ConstantInt::get(Type::getInt32Ty(*context), 0),
            ConstantInt::get(Type::getInt32Ty(*context), i)
        };
        
        Value* elementPtr = builder->CreateGEP(arrayType, arrayStorage, indices, "elem.ptr");
        
        // Conversão de tipo se necessário
        if (elementValue->getType() != elemType) {
            if (elemType->isIntegerTy(32) && elementValue->getType()->isIntegerTy(1)) {
                elementValue = builder->CreateZExt(elementValue, elemType, "zext");
            } else if (elemType->isIntegerTy(1) && elementValue->getType()->isIntegerTy(32)) {
                elementValue = builder->CreateICmpNE(elementValue, 
                    ConstantInt::get(*context, APInt(32, 0)), "boolconv");
            } else if (elemType->isFloatTy() && elementValue->getType()->isIntegerTy(32)) {
                elementValue = builder->CreateSIToFP(elementValue, elemType, "sitofp");
            }
        }
        
        // Armazenar valor
        builder->CreateStore(elementValue, elementPtr);
        
        // DEBUG
        llvm::errs() << "DEBUG: Armazenado elemento " << i << " no array\n";
    }
    
    llvm::errs() << "DEBUG: Array literal criado com sucesso\n";
    return arrayStorage;
}


Value* LLVMGenerator::generateArrayAssignment(AssignNode* node) {
    const std::string& arrayName = node->getIdentifier();
    Value* indexValue = generateExpr(node->getIndexExpr());
    Value* value = generateExpr(node->getValueExpr());
    
    if (!indexValue || !value) {
        llvm::errs() << "Erro: Expressão inválida em assignment de array\n";
        return nullptr;
    }
    
    // Encontrar o array
    Value* arrayPtr = nullptr;
    Type* arrayType = nullptr;
    
    if (namedValues.count(arrayName)) {
        arrayPtr = namedValues[arrayName];
        arrayType = variableTypes[arrayName];
    } else if (globalValues.count(arrayName)) {
        arrayPtr = globalValues[arrayName];
        arrayType = globalTypes[arrayName];
    } else {
        llvm::errs() << "Erro: Array '" << arrayName << "' não declarado\n";
        return nullptr;
    }
    
    // CORREÇÃO: Obter tipo do elemento corretamente
    Type* elementType = getArrayElementType(arrayType);
    if (!elementType) {
        llvm::errs() << "Erro: Não foi possível determinar o tipo do elemento do array '" 
                     << arrayName << "'\n";
        return nullptr;
    }
    
    // CORREÇÃO: Cálculo correto do ponteiro para elemento
    Value* indices[] = { 
        ConstantInt::get(Type::getInt32Ty(*context), 0), // índice base
        indexValue // índice do elemento
    };
    
    // CORREÇÃO: Usar CreateInBoundsGEP para cálculo seguro de ponteiro
    Value* elemPtr = builder->CreateInBoundsGEP(arrayType, arrayPtr, indices, "arrayelem");
    
    // Fazer conversão de tipo se necessário
    if (value->getType() != elementType) {
        if (elementType->isIntegerTy(32) && value->getType()->isIntegerTy(1)) {
            value = builder->CreateZExt(value, elementType, "intconv");
        } else if (elementType->isIntegerTy(1) && value->getType()->isIntegerTy(32)) {
            value = builder->CreateICmpNE(value, ConstantInt::get(*context, APInt(32, 0)), "boolconv");
        } else if (elementType->isFloatTy() && value->getType()->isIntegerTy(32)) {
            value = builder->CreateSIToFP(value, elementType, "floatconv");
        } else if (elementType->isIntegerTy(32) && value->getType()->isFloatTy()) {
            value = builder->CreateFPToSI(value, elementType, "intconv");
        } else {
            llvm::errs() << "Erro: Tipos incompatíveis em assignment de array. Esperado: " 
                        << *elementType << ", obtido: " << *value->getType() << "\n";
            return nullptr;
        }
    }
    
    // Store do valor
    builder->CreateStore(value, elemPtr);
    return value;
}

Value* LLVMGenerator::generateArrayAccess(ArrayAccessNode* node) {
    const std::string& arrayName = node->getArrayName();
    Value* indexValue = generateExpr(node->getIndex());
    
    if (!indexValue) {
        llvm::errs() << "Erro: Índice de array inválido\n";
        return nullptr;
    }
    
    // Verificar se é variável local ou global
    Value* arrayPtr = nullptr;
    Type* arrayType = nullptr;
    
    if (namedValues.count(arrayName)) {
        arrayPtr = namedValues[arrayName];
        arrayType = variableTypes[arrayName];
    } else if (globalValues.count(arrayName)) {
        arrayPtr = globalValues[arrayName];
        arrayType = globalTypes[arrayName];
    } else {
        llvm::errs() << "Erro: Array '" << arrayName << "' não declarado\n";
        return nullptr;
    }
    
    // CORREÇÃO: Obter tipo do elemento corretamente
    Type* elementType = getArrayElementType(arrayType);
    if (!elementType) {
        llvm::errs() << "Erro: Não foi possível determinar o tipo do elemento do array '" 
                     << arrayName << "'\n";
        return nullptr;
    }
    
    // CORREÇÃO: Cálculo correto do ponteiro para elemento
    Value* indices[] = {
        ConstantInt::get(Type::getInt32Ty(*context), 0), // índice base
        indexValue // índice do elemento
    };
    
    // CORREÇÃO: Usar CreateInBoundsGEP
    Value* elemPtr = builder->CreateInBoundsGEP(arrayType, arrayPtr, indices, "arrayelem");
    
    // Load do valor
    return builder->CreateLoad(elementType, elemPtr, "arrayload");
}

Value* LLVMGenerator::generatePrintCall(CallNode* node) {
    if (node->getArguments().empty()) {
        llvm::errs() << "Erro: print() requer pelo menos um argumento\n";
        return nullptr;
    }
    
    std::vector<Value*> printfArgs;
    
    // Processar cada argumento
    for (size_t i = 0; i < node->getArguments().size(); i++) {
        Value* arg = generateExpr(node->getArguments()[i].get());
        if (!arg) {
            llvm::errs() << "Erro: Argumento " << i << " inválido para print\n";
            return nullptr;
        }
        
        // Criar format string apropriada baseada no tipo
        std::string formatStr;
        if (arg->getType()->isIntegerTy(32)) {
            formatStr = "%d";
        } else if (arg->getType()->isFloatTy()) {
            formatStr = "%f";
            // Converter float para double para printf (varargs promotion)
            arg = builder->CreateFPExt(arg, Type::getDoubleTy(*context), "fpext");
        } else if (arg->getType()->isIntegerTy(1)) {
            formatStr = "%d";
            // Converter bool para int
            arg = builder->CreateZExt(arg, Type::getInt32Ty(*context), "boolconv");
        } else if (arg->getType()->isPointerTy()) {
            // String
            formatStr = "%s";
        } else {
            llvm::errs() << "Erro: Tipo não suportado para print\n";
            return nullptr;
        }
        
        // Adicionar espaço entre argumentos (exceto o último)
        if (i < node->getArguments().size() - 1) {
            formatStr += " ";
        }
        
        // Criar string literal para o formato
        GlobalVariable* formatStrGlobal = createStringLiteral(formatStr);
        Constant* zero = ConstantInt::get(Type::getInt32Ty(*context), 0);
        Constant* indices[] = {zero, zero};
        Value* formatStrPtr = ConstantExpr::getGetElementPtr(
            formatStrGlobal->getValueType(),
            formatStrGlobal,
            indices,
            true
        );
        
        // Chamar printf para este argumento
        printfArgs.clear();
        printfArgs.push_back(formatStrPtr);
        printfArgs.push_back(arg);
        builder->CreateCall(printfFunc, printfArgs);
    }
    
    // Adicionar newline no final
    GlobalVariable* newlineGlobal = createStringLiteral("\n");
    Constant* zero = ConstantInt::get(Type::getInt32Ty(*context), 0);
    Constant* indices[] = {zero, zero};
    Value* newlinePtr = ConstantExpr::getGetElementPtr(
        newlineGlobal->getValueType(),
        newlineGlobal,
        indices,
        true
    );
    
    printfArgs.clear();
    printfArgs.push_back(newlinePtr);
    return builder->CreateCall(printfFunc, printfArgs);
}

static llvm::OptimizationLevel toLLVLOptLevel(ambar::OptimizationLevel level) {
    switch (level) {
        case ambar::OptimizationLevel::O0: return llvm::OptimizationLevel::O0;
        case ambar::OptimizationLevel::O1: return llvm::OptimizationLevel::O1;
        case ambar::OptimizationLevel::O2: return llvm::OptimizationLevel::O2;
        case ambar::OptimizationLevel::O3: return llvm::OptimizationLevel::O3;
        case ambar::OptimizationLevel::Os: return llvm::OptimizationLevel::Os;
        case ambar::OptimizationLevel::Oz: return llvm::OptimizationLevel::Oz;
    }
    return llvm::OptimizationLevel::O0; // fallback
}


void LLVMGenerator::setOptimizationLevel(OptimizationLevel level) {
    optLevel = level;
}

void LLVMGenerator::optimizeFunction(Function* func) {
    if (optLevel == OptimizationLevel::O0) {
        return; // Sem otimizações
    }
    
    // Criar pass manager para a função
    legacy::FunctionPassManager FPM(module.get());
    
    // Adicionar passes baseados no nível de otimização - SIMPLIFICADO para LLVM 18
    if (optLevel >= OptimizationLevel::O1) {
        // Otimizações básicas usando apenas as APIs disponíveis
        FPM.add(createPromoteMemoryToRegisterPass()); // Mem2Reg - disponível
        FPM.add(createInstructionCombiningPass());     // InstCombine - disponível
        FPM.add(createReassociatePass());              // Reassociate - disponível
        FPM.add(createGVNPass());                      // GVN - disponível
        FPM.add(createCFGSimplificationPass());        // SimplifyCFG - disponível
    }
    
    if (optLevel >= OptimizationLevel::O2) {
        // Otimizações médias
        // FPM.add(createSCCPPass());                     // SCCP - disponível
        // FPM.add(createDeadStoreEliminationPass());     // DSE - disponível
        FPM.add(createLICMPass());                     // LICM - disponível
    }
    
    if (optLevel >= OptimizationLevel::O3) {
        // Otimizações agressivas
        FPM.add(createLoopUnrollPass());               // Loop unroll - disponível
        // Remover passes que podem não estar disponíveis
    }
    
    // Inicializar e executar passes
    FPM.doInitialization();
    FPM.run(*func);
}

void LLVMGenerator::optimizeModule() {
    if (optLevel == OptimizationLevel::O0) return;

    PassBuilder PB;
    LoopAnalysisManager LAM;
    FunctionAnalysisManager FAM;
    CGSCCAnalysisManager CGAM;
    ModuleAnalysisManager MAM;

    // Registrar análises
    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

    ModulePassManager MPM;

    if (optLevel >= OptimizationLevel::O2) {
    // Usa pipeline padrão para O2/O3
      MPM = PB.buildPerModuleDefaultPipeline(toLLVLOptLevel(optLevel));
    } else if (optLevel >= OptimizationLevel::O1) {
      MPM = PB.buildPerModuleDefaultPipeline(toLLVLOptLevel(optLevel));
    }


    MPM.run(*module, MAM);
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
        
        // Verificar se existe função main explícita
        bool hasExplicitMain = false;
        for (const auto& decl : programNode->getDeclarations()) {
            if (auto funcNode = dynamic_cast<FunctionNode*>(decl.get())) {
                if (funcNode->getName() == "main") {
                    hasExplicitMain = true;
                }
            }
        }
        
        // Gera todas as funções
        for (const auto& decl : programNode->getDeclarations()) {
            if (auto funcNode = dynamic_cast<FunctionNode*>(decl.get())) {
                generateFunction(funcNode);
            }
        }
        
        // Se não há função main explícita, gerar main implícita
        if (!hasExplicitMain) {
            generateMainFunction(astRoot);
        }

        if (optLevel >= OptimizationLevel::O2) {
            optimizeModule();
        }
    }
}

void LLVMGenerator::enableBasicOptimizations() {
    setOptimizationLevel(OptimizationLevel::O1);
}

void LLVMGenerator::enableAggressiveOptimizations() {
    setOptimizationLevel(OptimizationLevel::O3);
}

void LLVMGenerator::enableSizeOptimizations() {
    setOptimizationLevel(OptimizationLevel::Os);
}

void LLVMGenerator::disableOptimizations() {
    setOptimizationLevel(OptimizationLevel::O0);
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
        // Primeiro: verificar se existe uma função main explícita
        bool hasExplicitMain = false;
        for (const auto& decl : programNode->getDeclarations()) {
            if (auto funcNode = dynamic_cast<FunctionNode*>(decl.get())) {
                if (funcNode->getName() == "main") {
                    hasExplicitMain = true;
                    break;
                }
            }
        }
        
        // Se há função main explícita, não gerar main implícita
        if (hasExplicitMain) {
            mainFunc->eraseFromParent();
            currentFunction = nullptr;
            return;
        }
        
        // Processar todos os statements que não são funções ou variáveis globais
        for (const auto& decl : programNode->getDeclarations()) {
            // Pular funções e variáveis (já foram processadas)
            if (dynamic_cast<FunctionNode*>(decl.get()) || 
                dynamic_cast<VarNode*>(decl.get())) {
                continue;
            }
            
            // Verificar se o bloco atual ainda é válido
            BasicBlock* currentBB = builder->GetInsertBlock();
            if (currentBB->getTerminator()) {
                break; // Parar se já há um terminador
            }
            
            // Processar outros tipos de nós (statements, blocos, etc.)
            generateExpr(decl.get());
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
        generateLocalVariable(dynamic_cast<VarNode*>(decl.get()));
    }
    
    // Processar statements
    Value* lastValue = nullptr;
    BasicBlock* currentBB = builder->GetInsertBlock();
    
    for (const auto& stmt : node->getStatements()) {
        // Verificar se o bloco atual já tem um terminador (return/break/continue)
        if (currentBB->getTerminator()) {
            break; // Não processar mais statements se já temos um terminador
        }
        
        lastValue = generateExpr(stmt.get());
        if (!lastValue) {
            // CORREÇÃO: Continuar mesmo se um statement retornar null
            // (pode ser um return void, por exemplo)
            continue;
        }
        
        // Atualizar referência do bloco atual
        currentBB = builder->GetInsertBlock();
        
        // CORREÇÃO: Se encontrou um terminador, parar a execução
        if (currentBB->getTerminator()) {
            break;
        }
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
        // CORREÇÃO: usar .get()
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
        // CORREÇÃO: usar .get()
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
            
            // CORREÇÃO: usar .get()
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
                // CORREÇÃO: usar .get()
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

Type* LLVMGenerator::getArrayElementType(Type* arrayType) {
    if (arrayType->isArrayTy()) {
        return arrayType->getArrayElementType();
    } else if (arrayType->isPointerTy()) {
        PointerType* ptrType = cast<PointerType>(arrayType);
        return ptrType->getArrayElementType();
    }
    return nullptr;
}

// CORREÇÃO: Método generateVariable para arrays
llvm::Value* LLVMGenerator::generateVariable(VarNode* node) {
    std::string typeName = node->getType();
    Type* declaredType = getLLVMType(typeName);
    
    if (!declaredType) {
        llvm::errs() << "Erro: Tipo desconhecido: " << typeName << "\n";
        return nullptr;
    }

    if (currentFunction) {
        // CORREÇÃO: Se temos inicialização, usar o tipo do inicializador
        if (node->getInit()) {
            Value* initVal = generateExpr(node->getInit().get());
            if (initVal) {
                // Para arrays, usar o tipo exato do inicializador
                AllocaInst* alloca = builder->CreateAlloca(initVal->getType(), nullptr, node->getName());
                namedValues[node->getName()] = alloca;
                variableTypes[node->getName()] = initVal->getType();
                
                // CORREÇÃO: Copiar array completo
                if (initVal->getType()->isArrayTy()) {
                    ArrayType* arrayType = cast<ArrayType>(initVal->getType());
                    Type* elemType = arrayType->getArrayElementType();
                    
                    for (unsigned i = 0; i < arrayType->getNumElements(); ++i) {
                        Value* indices[] = {
                            ConstantInt::get(Type::getInt32Ty(*context), 0),
                            ConstantInt::get(Type::getInt32Ty(*context), i)
                        };
                        
                        // Carregar do inicializador
                        Value* srcElemPtr = builder->CreateGEP(arrayType, initVal, indices, "srcelem");
                        Value* elemValue = builder->CreateLoad(elemType, srcElemPtr, "loadtmp");
                        
                        // Armazenar na variável
                        Value* dstElemPtr = builder->CreateGEP(arrayType, alloca, indices, "dstelem");
                        builder->CreateStore(elemValue, dstElemPtr);
                    }
                } else {
                    // Para tipos não-array
                    builder->CreateStore(initVal, alloca);
                }
                return alloca;
            }
        }
        
        // CORREÇÃO: Sem inicialização, usar tipo declarado
        AllocaInst* alloca = builder->CreateAlloca(declaredType, nullptr, node->getName());
        namedValues[node->getName()] = alloca;
        variableTypes[node->getName()] = declaredType;
        return alloca;
    }
    
    return generateGlobalVariable(node);
}

Value* LLVMGenerator::generateNumber(NumberNode* node) {
    if (node->isFloatingPoint()) {
        return ConstantFP::get(Type::getFloatTy(*context), APFloat(static_cast<float>(node->getFloatValue())));
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
    } else if (auto arrayAccess = dynamic_cast<ArrayAccessNode*>(node)) {
        return generateArrayAccess(arrayAccess);
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
    
    // === CORREÇÃO: Gerar bloco then ===
    builder->SetInsertPoint(thenBB);
    Value* thenValue = generateExpr(node->getThenBlock());
    thenBB = builder->GetInsertBlock(); // Atualizar referência
    
    // Verificar se o bloco then terminou com um return/break/continue
    bool thenHasTerminator = thenBB->getTerminator() != nullptr;
    
    // Só criar branch para merge se o bloco then não terminou
    if (!thenHasTerminator) {
        builder->CreateBr(mergeBB);
    }
    
    // === CORREÇÃO: Gerar bloco else se existir ===
    Value* elseValue = nullptr;
    bool elseHasTerminator = false;
    
    if (elseBB) {
        builder->SetInsertPoint(elseBB);
        elseValue = generateExpr(node->getElseBlock());
        elseBB = builder->GetInsertBlock(); // Atualizar referência
        
        elseHasTerminator = elseBB->getTerminator() != nullptr;
        
        // Só criar branch para merge se o bloco else não terminou
        if (!elseHasTerminator) {
            builder->CreateBr(mergeBB);
        }
    }
    
    // === CORREÇÃO: Só continuar no bloco merge se ele for alcançável ===
    bool mergeReachable = (!thenHasTerminator || isa<BranchInst>(thenBB->getTerminator())) &&
                          (!elseBB || !elseHasTerminator || isa<BranchInst>(elseBB->getTerminator()));
    
    if (mergeReachable) {
        builder->SetInsertPoint(mergeBB);
        // CORREÇÃO: Usar PHI node apenas se ambos os caminhos retornarem valores
        if (thenValue && elseValue && thenValue->getType() == elseValue->getType() && 
            !thenHasTerminator && !elseHasTerminator) {
            PHINode* phi = builder->CreatePHI(thenValue->getType(), 2, "ifresult");
            phi->addIncoming(thenValue, thenBB);
            phi->addIncoming(elseValue, elseBB);
            return phi;
        }
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
    // Salvar o estado atual das tabelas de símbolos
    std::unordered_map<std::string, llvm::Value*> oldNamedValues = namedValues;
    std::unordered_map<std::string, llvm::Type*> oldVariableTypes = variableTypes;
    
    Function* function = builder->GetInsertBlock()->getParent();
    
    // Criar blocos básicos para o loop
    BasicBlock* preheaderBB = builder->GetInsertBlock();
    BasicBlock* condBB = BasicBlock::Create(*context, "for.cond", function);
    BasicBlock* bodyBB = BasicBlock::Create(*context, "for.body", function);
    BasicBlock* updateBB = BasicBlock::Create(*context, "for.update", function);
    BasicBlock* afterBB = BasicBlock::Create(*context, "for.end", function);
    
    // Gerar a inicialização (se existir) - NOVO ESCOPO
    if (node->getInit()) {
        generateExpr(node->getInit());
    }
    
    // Saltar para o bloco de condição
    builder->CreateBr(condBB);
    
    // Bloco de condição
    builder->SetInsertPoint(condBB);
    
    Value* condValue = nullptr;
    if (node->getCond()) {
        condValue = generateExpr(node->getCond());
        if (!condValue) {
            llvm::errs() << "Erro: Condição do for inválida\n";
            return nullptr;
        }
        
        // Converter para booleano se necessário
        if (!condValue->getType()->isIntegerTy(1)) {
            condValue = builder->CreateICmpNE(
                condValue, 
                ConstantInt::get(condValue->getType(), 0), 
                "forcond");
        }
    } else {
        // Se não há condição, loop infinito
        condValue = ConstantInt::get(Type::getInt1Ty(*context), 1);
    }
    
    builder->CreateCondBr(condValue, bodyBB, afterBB);
    
    // Bloco do corpo
    builder->SetInsertPoint(bodyBB);
    
    // Empilhar blocos de break/continue
    pushLoopBlocks(afterBB, updateBB);
    
    // Gerar o corpo do loop - MESMO ESCOPO!
    if (node->getBody()) {
        generateExpr(node->getBody());
    }
    
    // Desempilhar blocos de break/continue
    popLoopBlocks();
    
    // Saltar para o bloco de atualização
    builder->CreateBr(updateBB);
    
    // Bloco de atualização
    builder->SetInsertPoint(updateBB);
    
    // Gerar expressão de atualização (se existir) - MESMO ESCOPO!
    if (node->getUpdate()) {
        generateExpr(node->getUpdate());
    }
    
    // Voltar para verificar a condição
    builder->CreateBr(condBB);
    
    // Bloco após o loop
    builder->SetInsertPoint(afterBB);
    
    return ConstantInt::get(*context, APInt(32, 0));
}

void LLVMGenerator::popLoopBlocks() {
    if (!breakBlocks.empty()) breakBlocks.pop_back();
    if (!continueBlocks.empty()) continueBlocks.pop_back();
}

Value* LLVMGenerator::generateAssign(AssignNode* node) {
    const std::string& identifier = node->getIdentifier();
    Value* target = nullptr;
    Type* targetType = nullptr;

    if (node->isArrayAccess()) {
        return generateArrayAssignment(node);
    }

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
    
    // Verificar se é a função built-in print
    if (funcName == "print") {
        return generatePrintCall(node);
    }
    
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

    if (L->getType() != R->getType()) {
        // Conversões necessárias
        if (L->getType()->isIntegerTy(32) && R->getType()->isIntegerTy(1)) {
            R = builder->CreateZExt(R, Type::getInt32Ty(*context), "zexttmp");
        } else if (L->getType()->isIntegerTy(1) && R->getType()->isIntegerTy(32)) {
            L = builder->CreateZExt(L, Type::getInt32Ty(*context), "zexttmp");
        } else if (L->getType()->isFloatTy() && R->getType()->isIntegerTy(32)) {
            R = builder->CreateSIToFP(R, Type::getFloatTy(*context), "sitofptmp");
        } else if (L->getType()->isIntegerTy(32) && R->getType()->isFloatTy()) {
            L = builder->CreateSIToFP(L, Type::getFloatTy(*context), "sitofptmp");
        } else {
            llvm::errs() << "Erro: Tipos incompatíveis na operação binária: " 
                        << *L->getType() << " vs " << *R->getType() << "\n";
            return nullptr;
        }
    }

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
        Value* ptr = namedValues[node->getName()];
        // Verificar se já é um valor ou ainda é um ponteiro
        if (ptr->getType()->isPointerTy()) {
            return builder->CreateLoad(variableTypes[node->getName()], ptr, node->getName() + ".load");
        }
        return ptr;
    }
    
    if (globalValues.count(node->getName())) {
        GlobalVariable* gvar = globalValues[node->getName()];
        return builder->CreateLoad(globalTypes[node->getName()], gvar, node->getName() + ".load");
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
        // CORREÇÃO: param.first é o tipo, param.second é o nome
        Type* paramType = getLLVMType(param.first); // param.first = tipo
        if (!paramType) {
            llvm::errs() << "Erro: Tipo de parâmetro desconhecido: " << param.first << "\n";
            namedValues = std::move(oldNamedValues);
            variableTypes = std::move(oldVariableTypes);
            return;
        }
        paramTypes.push_back(paramType);
    }
    
    Type* returnType = getLLVMType(node->getReturnType());
    if (!returnType) {
        llvm::errs() << "Erro: Tipo de retorno desconhecido: " << node->getReturnType() << "\n";
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
    
    // CORREÇÃO: Processar os parâmetros da função corretamente
    unsigned idx = 0;
    for (auto& arg : func->args()) {
        const auto& param = node->getParams()[idx];
        const std::string& paramName = param.second; // param.second = nome
        const std::string& paramTypeStr = param.first; // param.first = tipo
        
        arg.setName(paramName);
        
        Type* paramType = getLLVMType(paramTypeStr);
        AllocaInst* alloca = builder->CreateAlloca(
            paramType,
            nullptr,
            paramName
        );
        
        builder->CreateStore(&arg, alloca);
        namedValues[paramName] = alloca;
        variableTypes[paramName] = paramType;
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

    if (optLevel > OptimizationLevel::O0) {
        optimizeFunction(func);
    }
    
    currentFunction = nullptr;
    namedValues = std::move(oldNamedValues);
    variableTypes = std::move(oldVariableTypes);
}

Type* LLVMGenerator::getLLVMType(const std::string& typeName) {
    if (typeName == "int") return Type::getInt32Ty(*context);
    if (typeName == "float") return Type::getFloatTy(*context);
    if (typeName == "bool") return Type::getInt1Ty(*context);
    if (typeName == "string") return PointerType::get(Type::getInt8Ty(*context), 0);
    if (typeName == "void") return Type::getVoidTy(*context);

    if (typeName.find("[]") != std::string::npos) {
        std::string elemType = typeName.substr(0, typeName.find("[]"));
        Type* elementType = getLLVMType(elemType);
        if (elementType) {
            // Retornar ponteiro para o tipo do elemento
            return PointerType::get(elementType, 0);
        }
    }
    
    return nullptr;
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
