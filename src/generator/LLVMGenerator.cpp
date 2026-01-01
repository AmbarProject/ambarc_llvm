#include "LLVMGenerator.hpp"
#include <llvm/IR/Verifier.h>
#include <memory>
#include <sstream>
#include <algorithm>

using namespace ambar;
using namespace llvm;

// ============================================================================
// CONSTRUTOR E INICIALIZAÇÃO
// ============================================================================

LLVMGenerator::LLVMGenerator(OptimizationLevel level) 
    : optLevel(level) {
    // Criar os componentes principais do contexto LLVM
    context = std::make_unique<LLVMContext>();
    module = std::make_unique<Module>("ambar", *context);
    builder = std::make_unique<IRBuilder<>>(*context);
    currentFunction = nullptr;
    
    // Declarar a função printf para uso em operações de impressão
    declarePrintfFunction();
}

// ============================================================================
// DECLARAÇÃO DE FUNÇÕES BUILT-IN
// ============================================================================

void LLVMGenerator::declarePrintfFunction() {
    // Definir o tipo da função printf: int printf(const char* format, ...)
    FunctionType* printfType = FunctionType::get(
        Type::getInt32Ty(*context),
        {PointerType::get(Type::getInt8Ty(*context), 0)},
        true  // varargs: suporta número variável de argumentos
    );
    
    // Criar a declaração da função no módulo
    printfFunc = Function::Create(
        printfType,
        Function::ExternalLinkage,
        "printf",
        *module
    );
}

// ============================================================================
// ANÁLISE DE OTIMIZAÇÃO - CLASSE HELPER
// ============================================================================

std::vector<OptimizationAnalyzer::ProblematicNode> 
OptimizationAnalyzer::analyzeOptimizationProblems() {
    std::vector<ProblematicNode> problems;
    
    // Analisar todas as funções definidas no módulo (ignorar declarações externas)
    for (llvm::Function& F : module) {
        if (!F.isDeclaration() && !F.empty()) {
            analyzeFunction(F, problems);
        }
    }
    
    return problems;
}

void OptimizationAnalyzer::analyzeFunction(llvm::Function& F, 
                                         std::vector<ProblematicNode>& problems) {
    // Configurar gerenciadores de análise para obter informações sobre loops
    llvm::LoopAnalysisManager LAM;
    llvm::FunctionAnalysisManager FAM;
    llvm::ModuleAnalysisManager MAM;
    
    llvm::PassBuilder PB;
    PB.registerFunctionAnalyses(FAM);
    
    // Obter informações sobre loops na função atual
    auto& LI = FAM.getResult<llvm::LoopAnalysis>(F);
    
    // Analisar cada bloco básico da função
    for (llvm::BasicBlock& BB : F) {
        analyzeBasicBlock(BB, F, problems);
    }
}

void OptimizationAnalyzer::analyzeBasicBlock(llvm::BasicBlock& BB, 
                                           llvm::Function& F,
                                           std::vector<ProblematicNode>& problems) {
    std::vector<std::string> blockProblems;
    int totalSeverity = 0;
    
    // Verificar diferentes tipos de problemas de otimização
    
    // 1. Computações redundantes (recalcular o mesmo valor)
    if (hasRedundantComputations(BB)) {
        blockProblems.push_back("Computações redundantes detectadas");
        totalSeverity += 3;  // Severidade moderada
    }
    
    // 2. Operações computacionalmente caras (divisões, conversões)
    if (hasExpensiveOperations(BB)) {
        blockProblems.push_back("Operações computacionalmente custosas");
        totalSeverity += 4;  // Severidade alta
    }
    
    // 3. Padrões de branch ineficientes (muitos desvios condicionais)
    if (hasPoorBranchPattern(BB)) {
        blockProblems.push_back("Padrão de branch ineficiente");
        totalSeverity += 2;  // Severidade baixa
    }
    
    // 4. Acessos de memória ineficientes
    if (hasMemoryInefficiencies(BB)) {
        blockProblems.push_back("Ineficiências de acesso à memória");
        totalSeverity += 3;  // Severidade moderada
    }
    
    // 5. Problemas em estruturas de loop
    if (hasLoopInefficiencies(BB, F)) {
        blockProblems.push_back("Ineficiências em estruturas de loop");
        totalSeverity += 5;  // Severidade muito alta
    }
    
    // Se encontramos problemas significativos, adicionar ao relatório
    if (!blockProblems.empty() && totalSeverity >= 3) {
        ProblematicNode node;
        node.functionName = F.getName().str();
        node.blockName = BB.getName().str();
        node.block = &BB;
        node.severity = std::min(10, totalSeverity);  // Limitar a 10
        
        // Criar descrição detalhada dos problemas encontrados
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
    
    // Contar diferentes tipos de instruções no bloco
    for (llvm::Instruction& I : BB) {
        if (llvm::isa<llvm::LoadInst>(&I)) loadCount++;
        if (llvm::isa<llvm::StoreInst>(&I)) storeCount++;
        if (llvm::isa<llvm::BinaryOperator>(&I)) computationCount++;
        
        // Heurística simples: muitos loads podem indicar computação redundante
        if (llvm::isa<llvm::LoadInst>(&I)) {
            if (loadCount > 5) return true;
        }
    }
    
    // Se houver muitas operações de memória em relação a computações, pode ser problemático
    if ((loadCount + storeCount) > computationCount * 2) {
        return true;
    }
    
    return false;
}

bool OptimizationAnalyzer::hasExpensiveOperations(llvm::BasicBlock& BB) {
    for (llvm::Instruction& I : BB) {
        // Verificar operações de divisão e módulo (são mais caras que adição/multiplicação)
        if (auto* binOp = llvm::dyn_cast<llvm::BinaryOperator>(&I)) {
            llvm::Instruction::BinaryOps opcode = binOp->getOpcode();
            if (opcode == llvm::Instruction::SDiv || opcode == llvm::Instruction::UDiv ||
                opcode == llvm::Instruction::SRem || opcode == llvm::Instruction::URem ||
                opcode == llvm::Instruction::FDiv || opcode == llvm::Instruction::FRem) {
                return true;
            }
        }
        
        // Verificar conversões de tipo que podem ser custosas
        if (llvm::isa<llvm::FPToSIInst>(&I) || llvm::isa<llvm::SIToFPInst>(&I) ||
            llvm::isa<llvm::FPTruncInst>(&I) || llvm::isa<llvm::FPExtInst>(&I)) {
            return true;
        }
    }
    return false;
}

bool OptimizationAnalyzer::hasPoorBranchPattern(llvm::BasicBlock& BB) {
    int branchCount = 0;
    int totalInstructions = 0;
    
    // Contar instruções de branch no bloco
    for (llvm::Instruction& I : BB) {
        totalInstructions++;
        if (llvm::isa<llvm::BranchInst>(&I) || llvm::isa<llvm::SwitchInst>(&I)) {
            branchCount++;
        }
    }
    
    // Se mais de 30% das instruções são branches, pode indicar código complexo
    if (totalInstructions > 0 && (branchCount * 100 / totalInstructions) > 30) {
        return true;
    }
    
    return false;
}

bool OptimizationAnalyzer::hasMemoryInefficiencies(llvm::BasicBlock& BB) {
    int memoryOps = 0;
    int totalInstructions = 0;
    
    // Contar operações de memória (loads, stores, alocações, etc.)
    for (llvm::Instruction& I : BB) {
        totalInstructions++;
        if (llvm::isa<llvm::LoadInst>(&I) || llvm::isa<llvm::StoreInst>(&I) ||
            llvm::isa<llvm::AllocaInst>(&I) || llvm::isa<llvm::GetElementPtrInst>(&I)) {
            memoryOps++;
        }
    }
    
    // Se mais de 50% são operações de memória, pode indicar otimizações possíveis
    if (totalInstructions > 0 && (memoryOps * 100 / totalInstructions) > 50) {
        return true;
    }
    
    return false;
}

bool OptimizationAnalyzer::hasLoopInefficiencies(llvm::BasicBlock& BB, llvm::Function& F) {
    // Configurar análise de loops
    llvm::LoopAnalysisManager LAM;
    llvm::FunctionAnalysisManager FAM;
    
    llvm::PassBuilder PB;
    PB.registerFunctionAnalyses(FAM);
    
    auto& LI = FAM.getResult<llvm::LoopAnalysis>(F);
    
    // Verificar se este bloco pertence a um loop
    if (llvm::Loop* L = LI.getLoopFor(&BB)) {
        // Se for o cabeçalho do loop, verificar por invariantes de loop
        if (L->getHeader() == &BB) {
            int invariantCount = 0;
            for (llvm::Instruction& I : BB) {
                // Identificar instruções que podem ser movidas para fora do loop
                if (llvm::isa<llvm::LoadInst>(&I) || llvm::isa<llvm::BinaryOperator>(&I)) {
                    invariantCount++;
                }
            }
            
            // Muitas instruções potencialmente invariantes podem indicar otimização perdida
            if (invariantCount > 3) {
                return true;
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
    
    // Ordenar problemas por severidade (mais severos primeiro)
    auto sortedProblems = problems;
    std::sort(sortedProblems.begin(), sortedProblems.end(),
              [](const ProblematicNode& a, const ProblematicNode& b) {
                  return a.severity > b.severity;
              });
    
    // Imprimir cada problema com detalhes
    for (size_t i = 0; i < sortedProblems.size(); ++i) {
        const auto& problem = sortedProblems[i];
        llvm::errs() << "PROBLEMA #" << (i + 1) << " [Severidade: " << problem.severity << "/10]\n";
        llvm::errs() << "Função: " << problem.functionName << "\n";
        llvm::errs() << "Bloco: " << problem.blockName << "\n";
        llvm::errs() << "Tipo: " << problem.problemType << "\n";
        llvm::errs() << "Descrição:\n" << problem.description << "\n";
        
        // Mostrar as primeiras instruções do bloco problemático para contexto
        llvm::errs() << "Instruções do bloco:\n";
        int instCount = 0;
        for (const llvm::Instruction& I : *problem.block) {
            if (instCount < 5) { // Limitar a 5 instruções para não sobrecarregar a saída
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

// ============================================================================
// RELATÓRIOS DE ANÁLISE
// ============================================================================

void LLVMGenerator::analyzeAndReportOptimizationProblems() {
    llvm::errs() << "\n🔍 Iniciando análise inovadora de otimização por grafos...\n";
    
    // Executar análise de otimização e imprimir resultados
    OptimizationAnalyzer analyzer(*module);
    auto problems = analyzer.analyzeOptimizationProblems();
    analyzer.printProblematicNodes(problems);
    
    // Executar análise avançada adicional
    performAdvancedOptimizationAnalysis();
}

void LLVMGenerator::performAdvancedOptimizationAnalysis() {
    llvm::errs() << "\n=== ANÁLISE AVANÇADA DE GRAFOS ===\n";
    
    int totalBlocks = 0;
    int totalFunctions = 0;
    int problematicBlocks = 0;
    
    // Analisar cada função e bloco no módulo
    for (llvm::Function& F : *module) {
        if (F.isDeclaration() || F.empty()) continue;
        
        totalFunctions++;
        
        for (llvm::BasicBlock& BB : F) {
            totalBlocks++;
            
            // Coletar estatísticas do bloco
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
            
            // Aplicar heurísticas para identificar blocos problemáticos
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
    
    // Imprimir relatório estatístico final
    llvm::errs() << "=== RELATÓRIO FINAL ===\n";
    llvm::errs() << "Funções analisadas: " << totalFunctions << "\n";
    llvm::errs() << "Blocos totais: " << totalBlocks << "\n";
    llvm::errs() << "Blocos problemáticos: " << problematicBlocks << "\n";
    
    if (totalBlocks > 0) {
        double problematicPercentage = (problematicBlocks * 100.0) / totalBlocks;
        llvm::errs() << "Taxa de problemas: " << problematicPercentage << "%\n";
        
        // Classificar qualidade do código baseado na taxa de problemas
        if (problematicPercentage > 20.0) {
            llvm::errs() << "❌ ALTA TAXA DE PROBLEMAS: Considere reestruturar o código!\n";
        } else if (problematicPercentage > 10.0) {
            llvm::errs() << "⚠️  TAXA MODERADA DE PROBLEMAS: Algumas otimizações são recomendadas\n";
        } else {
            llvm::errs() << "✅ BAIXA TAXA DE PROBLEMAS: Código bem estruturado!\n";
        }
    }
}

// ============================================================================
// MANIPULAÇÃO DE ARRAYS
// ============================================================================

llvm::Value* LLVMGenerator::generateArrayLiteral(ArrayNode* node) {
    const auto& elements = node->getElements();
    
    // Log de depuração para entender o que está sendo processado
    llvm::errs() << "DEBUG: Gerando array literal com " << elements.size() << " elementos\n";
    
    // Obter o tipo do elemento do array
    Type* elemType = getLLVMType(node->getElementType());
    if (!elemType) {
        llvm::errs() << "Erro: Tipo de elemento desconhecido: " << node->getElementType() << "\n";
        return nullptr;
    }
    
    if (!currentFunction) {
        llvm::errs() << "Erro: Array literal fora de função\n";
        return nullptr;
    }
    
    // Criar tipo de array com tamanho exato dos elementos
    ArrayType* arrayType = ArrayType::get(elemType, elements.size());
    
    // Alocar espaço na stack para o array
    AllocaInst* arrayStorage = builder->CreateAlloca(arrayType, nullptr, "array.storage");
    
    // Inicializar cada elemento do array
    for (size_t i = 0; i < elements.size(); ++i) {
        Value* elementValue = generateExpr(elements[i].get());
        if (!elementValue) {
            llvm::errs() << "Erro: Não foi possível gerar elemento " << i << " do array\n";
            return nullptr;
        }
        
        // Log de depuração para verificação de tipos
        llvm::errs() << "DEBUG: Elemento " << i << " - tipo: " << *elementValue->getType() << "\n";
        
        // Calcular ponteiro para este elemento específico
        Value* indices[] = {
            ConstantInt::get(Type::getInt32Ty(*context), 0),  // índice base do array
            ConstantInt::get(Type::getInt32Ty(*context), i)   // índice do elemento
        };
        
        Value* elementPtr = builder->CreateGEP(arrayType, arrayStorage, indices, "elem.ptr");
        
        // Converter tipos se necessário (ex: bool para int, int para float)
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
        
        // Armazenar o valor convertido no array
        builder->CreateStore(elementValue, elementPtr);
        
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
    
    // Localizar o array nas tabelas de símbolos (locais ou globais)
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
    
    // Determinar o tipo do elemento do array
    Type* elementType = getArrayElementType(arrayType);
    if (!elementType) {
        llvm::errs() << "Erro: Não foi possível determinar o tipo do elemento do array '" 
                     << arrayName << "'\n";
        return nullptr;
    }
    
    // Calcular ponteiro para o elemento específico usando aritmética de ponteiros segura
    Value* indices[] = { 
        ConstantInt::get(Type::getInt32Ty(*context), 0),  // índice base
        indexValue                                       // índice do elemento
    };
    
    Value* elemPtr = builder->CreateInBoundsGEP(arrayType, arrayPtr, indices, "arrayelem");
    
    // Converter o valor se necessário para corresponder ao tipo do elemento
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
    
    // Armazenar o valor convertido na posição calculada
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
    
    // Localizar o array (local ou global)
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
    
    // Determinar tipo do elemento
    Type* elementType = getArrayElementType(arrayType);
    if (!elementType) {
        llvm::errs() << "Erro: Não foi possível determinar o tipo do elemento do array '" 
                     << arrayName << "'\n";
        return nullptr;
    }
    
    // Calcular ponteiro para o elemento usando aritmética de ponteiros com verificação de limites
    Value* indices[] = {
        ConstantInt::get(Type::getInt32Ty(*context), 0),  // índice base
        indexValue                                       // índice do elemento
    };
    
    Value* elemPtr = builder->CreateInBoundsGEP(arrayType, arrayPtr, indices, "arrayelem");
    
    // Carregar e retornar o valor do elemento
    return builder->CreateLoad(elementType, elemPtr, "arrayload");
}

// ============================================================================
// FUNÇÕES BUILT-IN
// ============================================================================

Value* LLVMGenerator::generatePrintCall(CallNode* node) {
    if (node->getArguments().empty()) {
        llvm::errs() << "Erro: print() requer pelo menos um argumento\n";
        return nullptr;
    }
    
    std::vector<Value*> printfArgs;
    
    // Processar cada argumento para criar string de formato apropriada
    for (size_t i = 0; i < node->getArguments().size(); i++) {
        Value* arg = generateExpr(node->getArguments()[i].get());
        if (!arg) {
            llvm::errs() << "Erro: Argumento " << i << " inválido para print\n";
            return nullptr;
        }
        
        // Determinar string de formato baseada no tipo do argumento
        std::string formatStr;
        if (arg->getType()->isIntegerTy(32)) {
            formatStr = "%d";  // inteiro
        } else if (arg->getType()->isFloatTy()) {
            formatStr = "%f";  // float
            // printf espera double, não float, então converter
            arg = builder->CreateFPExt(arg, Type::getDoubleTy(*context), "fpext");
        } else if (arg->getType()->isIntegerTy(1)) {
            formatStr = "%d";  // booleano como inteiro
            arg = builder->CreateZExt(arg, Type::getInt32Ty(*context), "boolconv");
        } else if (arg->getType()->isPointerTy()) {
            formatStr = "%s";  // string
        } else {
            llvm::errs() << "Erro: Tipo não suportado para print\n";
            return nullptr;
        }
        
        // Adicionar espaço entre argumentos (exceto o último)
        if (i < node->getArguments().size() - 1) {
            formatStr += " ";
        }
        
        // Criar string literal para o formato e chamar printf
        GlobalVariable* formatStrGlobal = createStringLiteral(formatStr);
        Constant* zero = ConstantInt::get(Type::getInt32Ty(*context), 0);
        Constant* indices[] = {zero, zero};
        Value* formatStrPtr = ConstantExpr::getGetElementPtr(
            formatStrGlobal->getValueType(),
            formatStrGlobal,
            indices,
            true
        );
        
        printfArgs.clear();
        printfArgs.push_back(formatStrPtr);
        printfArgs.push_back(arg);
        builder->CreateCall(printfFunc, printfArgs);
    }
    
    // Adicionar nova linha no final da impressão
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

// ============================================================================
// SISTEMA DE OTIMIZAÇÃO
// ============================================================================

// Converter nível de otimização do nosso sistema para nível LLVM
static llvm::OptimizationLevel toLLVMOptLevel(ambar::OptimizationLevel level) {
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
        return; // Sem otimizações no nível O0
    }
    
    // Criar gerenciador de passes para função
    legacy::FunctionPassManager FPM(module.get());
    
    // Configurar passes baseados no nível de otimização
    if (optLevel >= OptimizationLevel::O1) {
        // Otimizações básicas que melhoram desempenho sem aumentar muito o tamanho
        FPM.add(createPromoteMemoryToRegisterPass()); // Converter alocações locais para registros
        FPM.add(createInstructionCombiningPass());     // Combinar instruções simples
        FPM.add(createReassociatePass());              // Reassociar expressões
        FPM.add(createGVNPass());                      // Eliminar valores redundantes
        FPM.add(createCFGSimplificationPass());        // Simplificar fluxo de controle
    }
    
    if (optLevel >= OptimizationLevel::O2) {
        // Otimizações mais agressivas
        FPM.add(createLICMPass()); // Mover invariantes para fora de loops
    }
    
    if (optLevel >= OptimizationLevel::O3) {
        // Otimizações agressivas que podem aumentar tamanho do código
        FPM.add(createLoopUnrollPass()); // Desenrolar loops para melhorar performance
    }
    
    // Executar todos os passes configurados
    FPM.doInitialization();
    FPM.run(*func);
}

void LLVMGenerator::optimizeModule() {
    if (optLevel == OptimizationLevel::O0) return;

    // Configurar sistema moderno de passes do LLVM
    PassBuilder PB;
    LoopAnalysisManager LAM;
    FunctionAnalysisManager FAM;
    CGSCCAnalysisManager CGAM;
    ModuleAnalysisManager MAM;

    // Registrar todos os tipos de análise disponíveis
    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

    ModulePassManager MPM;

    // Selecionar pipeline de otimização baseado no nível
    if (optLevel >= OptimizationLevel::O2) {
        MPM = PB.buildPerModuleDefaultPipeline(toLLVMOptLevel(optLevel));
    } else if (optLevel >= OptimizationLevel::O1) {
        MPM = PB.buildPerModuleDefaultPipeline(toLLVMOptLevel(optLevel));
    }

    // Executar pipeline de otimização no módulo inteiro
    MPM.run(*module, MAM);
}

// ============================================================================
// MANIPULAÇÃO DE STRINGS LITERAIS
// ============================================================================

GlobalVariable* LLVMGenerator::createStringLiteral(const std::string& value) {
    // Cache de strings literais para evitar duplicação
    if (stringLiterals.count(value)) {
        return stringLiterals[value];
    }
    
    // Criar constante de array de caracteres
    Constant* strConstant = ConstantDataArray::getString(*context, value);
    
    // Criar variável global para armazenar a string
    GlobalVariable* strArray = new GlobalVariable(
        *module,
        strConstant->getType(),
        true, // constante (não pode ser modificada)
        GlobalValue::PrivateLinkage, // visibilidade interna ao módulo
        strConstant,
        ".str"  // prefixo para nomes de strings
    );
    
    // Armazenar no cache para reuso
    stringLiterals[value] = strArray;
    return strArray;
}

// ============================================================================
// GERAÇÃO DO CÓDIGO PRINCIPAL
// ============================================================================

void LLVMGenerator::generate(std::unique_ptr<ASTNode>& astRoot) {
    if (auto programNode = dynamic_cast<ProgramNode*>(astRoot.get())) {
        // Processar variáveis globais primeiro (podem ser usadas em funções)
        handleGlobalVariables(programNode);
        
        // Verificar se existe uma função main explícita
        bool hasExplicitMain = false;
        for (const auto& decl : programNode->getDeclarations()) {
            if (auto funcNode = dynamic_cast<FunctionNode*>(decl.get())) {
                if (funcNode->getName() == "main") {
                    hasExplicitMain = true;
                }
            }
        }
        
        // Gerar código para todas as funções definidas
        for (const auto& decl : programNode->getDeclarations()) {
            if (auto funcNode = dynamic_cast<FunctionNode*>(decl.get())) {
                generateFunction(funcNode);
            }
        }
        
        // Se não há função main explícita, criar uma implícita
        if (!hasExplicitMain) {
            generateMainFunction(astRoot);
        }

        // Aplicar otimizações em nível de módulo se configurado
        if (optLevel >= OptimizationLevel::O2) {
            optimizeModule();
        }
    }
}

// Interfaces simplificadas para configuração de otimização
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

// ============================================================================
// FUNÇÃO MAIN (IMPLÍCITA OU EXPLÍCITA)
// ============================================================================

void LLVMGenerator::generateMainFunction(std::unique_ptr<ASTNode>& astRoot) {
    // Definir tipo da função main: int main(void)
    FunctionType* mainFuncType = FunctionType::get(
        Type::getInt32Ty(*context),
        false
    );
    
    // Criar função main
    Function* mainFunc = Function::Create(
        mainFuncType,
        Function::ExternalLinkage,
        "main",
        *module
    );
    
    // Configurar como função atual
    currentFunction = mainFunc;
    BasicBlock* bb = BasicBlock::Create(*context, "entry", mainFunc);
    builder->SetInsertPoint(bb);
    
    if (auto programNode = dynamic_cast<ProgramNode*>(astRoot.get())) {
        // Verificar novamente se existe main explícita (segurança)
        bool hasExplicitMain = false;
        for (const auto& decl : programNode->getDeclarations()) {
            if (auto funcNode = dynamic_cast<FunctionNode*>(decl.get())) {
                if (funcNode->getName() == "main") {
                    hasExplicitMain = true;
                    break;
                }
            }
        }
        
        // Se encontrou main explícita, remover a implícita
        if (hasExplicitMain) {
            mainFunc->eraseFromParent();
            currentFunction = nullptr;
            return;
        }
        
        // Executar todos os statements que não são funções ou variáveis globais
        for (const auto& decl : programNode->getDeclarations()) {
            // Pular funções e variáveis (já processadas)
            if (dynamic_cast<FunctionNode*>(decl.get()) || 
                dynamic_cast<VarNode*>(decl.get())) {
                continue;
            }
            
            // Verificar se ainda podemos adicionar instruções
            BasicBlock* currentBB = builder->GetInsertBlock();
            if (currentBB->getTerminator()) {
                break; // Já tem retorno, parar execução
            }
            
            // Gerar código para o statement
            generateExpr(decl.get());
        }
    }
    
    // Garantir que a função termine com um retorno
    BasicBlock* currentBB = builder->GetInsertBlock();
    if (!currentBB->getTerminator()) {
        builder->CreateRet(ConstantInt::get(*context, APInt(32, 0))); // return 0
    }
    
    // Verificar validade da função gerada
    verifyFunction(*mainFunc);
    currentFunction = nullptr;
}

// ============================================================================
// GERADORES DE EXPRESSÕES E STATEMENTS
// ============================================================================

Value* LLVMGenerator::generateBlock(BlockNode* node) {
    // Salvar estado atual para restaurar após o bloco (escopo local)
    std::unordered_map<std::string, llvm::Value*> oldNamedValues = namedValues;
    std::unordered_map<std::string, llvm::Type*> oldVariableTypes = variableTypes;
    
    // Processar declarações locais primeiro
    for (const auto& decl : node->getLocalDeclarations()) {
        generateLocalVariable(dynamic_cast<VarNode*>(decl.get()));
    }
    
    // Processar statements em sequência
    Value* lastValue = nullptr;
    BasicBlock* currentBB = builder->GetInsertBlock();
    
    for (const auto& stmt : node->getStatements()) {
        // Se o bloco já terminou (return/break/continue), parar execução
        if (currentBB->getTerminator()) {
            break;
        }
        
        lastValue = generateExpr(stmt.get());
        
        // Atualizar referência para o bloco atual
        currentBB = builder->GetInsertBlock();
        
        // Se encontrou um terminador, parar processamento de statements
        if (currentBB->getTerminator()) {
            break;
        }
    }
    
    // Restaurar tabelas de símbolos (sair do escopo local)
    namedValues = oldNamedValues;
    variableTypes = oldVariableTypes;
    
    return lastValue;
}

Value* LLVMGenerator::generateLocalVariable(VarNode* node) {
    Type* type = getLLVMType(node->getType());
    AllocaInst* alloca = builder->CreateAlloca(type, nullptr, node->getName());
    
    // Registrar variável nas tabelas de símbolos locais
    namedValues[node->getName()] = alloca;
    variableTypes[node->getName()] = type;

    // Se houver inicialização, processá-la
    if (node->getInit()) {
        Value* initVal = generateExpr(node->getInit().get());
        if (initVal) {
            // Converter tipo se necessário
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
    
    // Tentar avaliar expressão de inicialização em tempo de compilação
    if (node->getInit()) {
        initVal = evaluateConstantExpression(node->getInit().get());
        
        if (!initVal) {
            // Se não for constante, tentar avaliar em função temporária
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
            
            // Limpar função temporária
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

    // Usar valor padrão se não houver inicialização
    if (!initVal) {
        if (type->isFloatTy()) {
            initVal = ConstantFP::get(*context, APFloat(0.0f));
        } else if (type->isIntegerTy(32)) {
            initVal = ConstantInt::get(*context, APInt(32, 0));
        } else if (type->isIntegerTy(1)) {
            initVal = ConstantInt::get(*context, APInt(1, 0));
        } else if (type->isPointerTy()) {
            initVal = ConstantPointerNull::get(cast<PointerType>(type));
        }
    }

    // Criar variável global
    GlobalVariable* gVar = new GlobalVariable(
        *module,
        type,
        false, // não constante (pode ser modificada)
        GlobalValue::InternalLinkage,
        initVal,
        node->getName()
    );
    
    // Registrar nas tabelas de símbolos globais
    globalValues[node->getName()] = gVar;
    globalTypes[node->getName()] = type;
    return gVar;
}

// ============================================================================
// AVALIAÇÃO DE EXPRESSÕES CONSTANTES
// ============================================================================

Constant* LLVMGenerator::evaluateConstantExpression(ASTNode* node) {
    if (!node) {
        llvm::errs() << "Erro: Nó nulo em expressão constante\n";
        return nullptr;
    }

    // Casos básicos: números, booleanos, strings
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
        // Referência a variável global constante
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
        // Avaliar operações binárias em tempo de compilação
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
        
        llvm::errs() << "Erro: Operação '" << op << "' não suportada em contexto constante\n";
        return nullptr;
    }
    else if (auto unary = dynamic_cast<UnaryNode*>(node)) {
        // Avaliar operações unárias
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
            }
        } 
        else if (op == "~") {
            if (operand->getType()->isIntegerTy(32)) {
                APInt intVal = cast<ConstantInt>(operand)->getValue();
                return ConstantInt::get(*context, ~intVal);
            }
        }

        llvm::errs() << "Erro: Operador unário '" << op << "' não suportado em contexto constante\n";
        return nullptr;
    }
    
    llvm::errs() << "Erro: Tipo de nó não suportado em expressão constante\n";
    return nullptr;
}

// ============================================================================
// MANIPULAÇÃO DE VARIÁVEIS GLOBAIS
// ============================================================================

void LLVMGenerator::handleGlobalVariables(ProgramNode* program) {
    // Primeiro passe: criar todas as globais com valores padrão
    for (const auto& decl : program->getDeclarations()) {
        if (auto varNode = dynamic_cast<VarNode*>(decl.get())) {
            Type* type = getLLVMType(varNode->getType());
            Constant* initVal = nullptr;
            
            // Definir valores padrão baseados no tipo
            if (type->isFloatTy()) {
                initVal = ConstantFP::get(*context, APFloat(0.0f));
            } else if (type->isIntegerTy(32)) {
                initVal = ConstantInt::get(*context, APInt(32, 0));
            } else if (type->isIntegerTy(1)) {
                initVal = ConstantInt::get(*context, APInt(1, 0));
            } else if (type->isPointerTy()) {
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
    
    // Segundo passe: avaliar e atribuir expressões constantes de inicialização
    for (const auto& decl : program->getDeclarations()) {
        if (auto varNode = dynamic_cast<VarNode*>(decl.get())) {
            if (varNode->getInit()) {
                Constant* initVal = evaluateConstantExpression(varNode->getInit().get());
                if (initVal) {
                    // Verificar compatibilidade e converter tipos se necessário
                    Type* expectedType = globalTypes[varNode->getName()];
                    if (initVal->getType() != expectedType) {
                        if (expectedType->isFloatTy() && initVal->getType()->isIntegerTy(32)) {
                            if (auto intVal = dyn_cast<ConstantInt>(initVal)) {
                                initVal = ConstantFP::get(*context, 
                                    APFloat(static_cast<float>(intVal->getSExtValue())));
                            }
                        } else if (expectedType->isIntegerTy(32) && initVal->getType()->isFloatTy()) {
                            if (auto floatVal = dyn_cast<ConstantFP>(initVal)) {
                                APFloat floatValue = floatVal->getValueAPF();
                                initVal = ConstantInt::get(*context, 
                                    APInt(32, static_cast<int32_t>(floatValue.convertToFloat())));
                            }
                        } else if (expectedType->isIntegerTy(1) && initVal->getType()->isIntegerTy(32)) {
                            if (auto intVal = dyn_cast<ConstantInt>(initVal)) {
                                initVal = ConstantInt::get(*context, 
                                    APInt(1, intVal->getZExtValue() != 0));
                            }
                        }
                        
                        if (initVal->getType() != expectedType) {
                            llvm::errs() << "Erro: Tipo incompatível para variável global '" 
                                        << varNode->getName() << "'. Esperado: " << *expectedType
                                        << ", obtido: " << *initVal->getType() << "\n";
                            continue;
                        }
                    }
                    
                    // Atribuir valor inicial
                    globalValues[varNode->getName()]->setInitializer(initVal);
                } else {
                    llvm::errs() << "Aviso: Expressão não constante para variável global '" 
                                << varNode->getName() << "'. Mantendo valor padrão.\n";
                }
            }
        }
    }
}

// ============================================================================
// FUNÇÕES AUXILIARES PARA TIPOS
// ============================================================================

Type* LLVMGenerator::getArrayElementType(Type* arrayType) {
    if (arrayType->isArrayTy()) {
        return arrayType->getArrayElementType();
    } else if (arrayType->isPointerTy()) {
        PointerType* ptrType = cast<PointerType>(arrayType);
        return ptrType->getArrayElementType();
    }
    return nullptr;
}

llvm::Value* LLVMGenerator::generateVariable(VarNode* node) {
    std::string typeName = node->getType();
    Type* declaredType = getLLVMType(typeName);
    
    if (!declaredType) {
        llvm::errs() << "Erro: Tipo desconhecido: " << typeName << "\n";
        return nullptr;
    }

    if (currentFunction) {
        // Variável local: considerar inicializador para determinar tipo exato
        if (node->getInit()) {
            Value* initVal = generateExpr(node->getInit().get());
            if (initVal) {
                // Para arrays, usar tipo exato do inicializador
                AllocaInst* alloca = builder->CreateAlloca(initVal->getType(), nullptr, node->getName());
                namedValues[node->getName()] = alloca;
                variableTypes[node->getName()] = initVal->getType();
                
                // Copiar array completo elemento por elemento
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
                    // Tipos não-array: armazenar diretamente
                    builder->CreateStore(initVal, alloca);
                }
                return alloca;
            }
        }
        
        // Sem inicialização: usar tipo declarado
        AllocaInst* alloca = builder->CreateAlloca(declaredType, nullptr, node->getName());
        namedValues[node->getName()] = alloca;
        variableTypes[node->getName()] = declaredType;
        return alloca;
    }
    
    // Variável global
    return generateGlobalVariable(node);
}

// ============================================================================
// GERADORES PARA DIFERENTES TIPOS DE NÓS DA AST
// ============================================================================

Value* LLVMGenerator::generateNumber(NumberNode* node) {
    if (node->isFloatingPoint()) {
        return ConstantFP::get(Type::getFloatTy(*context), 
            APFloat(static_cast<float>(node->getFloatValue())));
    } else {
        return ConstantInt::get(*context, APInt(32, node->getIntValue()));
    }
}

Value* LLVMGenerator::generateExpr(ASTNode* node) {
    // Roteador para diferentes tipos de nós da AST
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

// ============================================================================
// GERADORES PARA STATEMENTS DE CONTROLE
// ============================================================================

Value* LLVMGenerator::generateReturn(ReturnNode* node) {
    if (!currentFunction) {
        llvm::errs() << "Erro: 'return' fora de uma função\n";
        return nullptr;
    }
    
    Type* returnType = currentFunction->getReturnType();
    
    if (node->getExpr()) {
        // Return com expressão de valor
        Value* retValue = generateExpr(node->getExpr());
        if (!retValue) {
            llvm::errs() << "Erro: Expressão de return inválida\n";
            return nullptr;
        }
        
        // Converter tipo se necessário
        if (retValue->getType() != returnType) {
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
        // Return void (sem valor)
        if (!returnType->isVoidTy()) {
            llvm::errs() << "Erro: Função não-void retornando sem valor\n";
            return nullptr;
        }
        
        builder->CreateRetVoid();
        return nullptr;
    }
}

Value* LLVMGenerator::generateWhile(WhileNode* node) {
    // Salvar estado para escopo do loop
    std::unordered_map<std::string, llvm::Value*> oldNamedValues = namedValues;
    std::unordered_map<std::string, llvm::Type*> oldVariableTypes = variableTypes;
    
    Function* function = builder->GetInsertBlock()->getParent();
    BasicBlock* currentBB = builder->GetInsertBlock();
    
    // Criar estrutura de blocos para o loop while
    BasicBlock* condBB = BasicBlock::Create(*context, "while.cond", function);
    BasicBlock* bodyBB = BasicBlock::Create(*context, "while.body", function);
    BasicBlock* afterBB = BasicBlock::Create(*context, "while.end", function);
    
    // Saltar para bloco de condição
    builder->CreateBr(condBB);
    
    // Gerar condição
    builder->SetInsertPoint(condBB);
    Value* condValue = generateExpr(node->getCondition());
    if (!condValue) {
        llvm::errs() << "Erro: Condição do while inválida\n";
        return nullptr;
    }
    
    // Converter para booleano
    if (!condValue->getType()->isIntegerTy(1)) {
        condValue = builder->CreateICmpNE(
            condValue, 
            ConstantInt::get(condValue->getType(), 0), 
            "whilecond");
    }
    
    builder->CreateCondBr(condValue, bodyBB, afterBB);
    
    // Gerar corpo do loop
    builder->SetInsertPoint(bodyBB);
    pushLoopBlocks(afterBB, condBB);
    
    if (node->getBody()) {
        generateExpr(node->getBody());
    }
    
    popLoopBlocks();
    builder->CreateBr(condBB);
    
    // Continuar após o loop
    builder->SetInsertPoint(afterBB);
    
    // Restaurar tabelas de símbolos
    namedValues = oldNamedValues;
    variableTypes = oldVariableTypes;
    
    return ConstantInt::get(*context, APInt(32, 0));
}

Value* LLVMGenerator::generateIf(IfNode* node) {
    Value* condValue = generateExpr(node->getCondition());
    if (!condValue) {
        llvm::errs() << "Erro: Condição do if inválida\n";
        return nullptr;
    }
    
    // Converter condição para booleano
    if (!condValue->getType()->isIntegerTy(1)) {
        condValue = builder->CreateICmpNE(
            condValue, 
            ConstantInt::get(condValue->getType(), 0), 
            "ifcond");
    }
    
    Function* function = builder->GetInsertBlock()->getParent();
    
    // Criar blocos para estrutura if-else
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
    
    bool thenHasTerminator = thenBB->getTerminator() != nullptr;
    
    if (!thenHasTerminator) {
        builder->CreateBr(mergeBB);
    }
    
    // Gerar bloco else se existir
    Value* elseValue = nullptr;
    bool elseHasTerminator = false;
    
    if (elseBB) {
        builder->SetInsertPoint(elseBB);
        elseValue = generateExpr(node->getElseBlock());
        elseBB = builder->GetInsertBlock();
        
        elseHasTerminator = elseBB->getTerminator() != nullptr;
        
        if (!elseHasTerminator) {
            builder->CreateBr(mergeBB);
        }
    }
    
    // Determinar se o bloco merge é alcançável
    bool mergeReachable = (!thenHasTerminator || isa<BranchInst>(thenBB->getTerminator())) &&
                          (!elseBB || !elseHasTerminator || isa<BranchInst>(elseBB->getTerminator()));
    
    if (mergeReachable) {
        builder->SetInsertPoint(mergeBB);
        // Criar PHI node para valor resultante do if-else
        if (thenValue && elseValue && thenValue->getType() == elseValue->getType() && 
            !thenHasTerminator && !elseHasTerminator) {
            PHINode* phi = builder->CreatePHI(thenValue->getType(), 2, "ifresult");
            phi->addIncoming(thenValue, thenBB);
            phi->addIncoming(elseValue, elseBB);
            return phi;
        }
        return thenValue ? thenValue : elseValue;
    } else {
        mergeBB->eraseFromParent();
        return nullptr;
    }
}

// ============================================================================
// GERADORES PARA OPERADORES E EXPRESSÕES
// ============================================================================

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
        return ConstantInt::get(*context, APInt(32, 0));
    }
    
    // Saltar para bloco de break
    BasicBlock* breakBlock = breakBlocks.back();
    builder->CreateBr(breakBlock);
    
    // Criar bloco vazio para evitar instruções órfãs
    BasicBlock* afterBreak = BasicBlock::Create(*context, "afterbreak", currentFunction);
    builder->SetInsertPoint(afterBreak);
    
    return ConstantInt::get(*context, APInt(32, 0));
}

Value* LLVMGenerator::generateContinue(ContinueNode* node) {
    if (continueBlocks.empty()) {
        llvm::errs() << "Aviso: 'continue' fora de um loop - ignorando\n";
        return ConstantInt::get(*context, APInt(32, 0));
    }
    
    // Saltar para bloco de continue
    BasicBlock* continueBlock = continueBlocks.back();
    builder->CreateBr(continueBlock);
    
    BasicBlock* afterContinue = BasicBlock::Create(*context, "aftercontinue", currentFunction);
    builder->SetInsertPoint(afterContinue);
    
    return ConstantInt::get(*context, APInt(32, 0));
}

void LLVMGenerator::pushLoopBlocks(BasicBlock* breakBlock, BasicBlock* continueBlock) {
    breakBlocks.push_back(breakBlock);
    continueBlocks.push_back(continueBlock);
}

Value* LLVMGenerator::generateFor(ForNode* node) {
    // Salvar estado para escopo do loop
    std::unordered_map<std::string, llvm::Value*> oldNamedValues = namedValues;
    std::unordered_map<std::string, llvm::Type*> oldVariableTypes = variableTypes;
    
    Function* function = builder->GetInsertBlock()->getParent();
    
    // Criar estrutura de blocos para loop for
    BasicBlock* preheaderBB = builder->GetInsertBlock();
    BasicBlock* condBB = BasicBlock::Create(*context, "for.cond", function);
    BasicBlock* bodyBB = BasicBlock::Create(*context, "for.body", function);
    BasicBlock* updateBB = BasicBlock::Create(*context, "for.update", function);
    BasicBlock* afterBB = BasicBlock::Create(*context, "for.end", function);
    
    // Inicialização
    if (node->getInit()) {
        generateExpr(node->getInit());
    }
    
    builder->CreateBr(condBB);
    
    // Condição
    builder->SetInsertPoint(condBB);
    Value* condValue = nullptr;
    if (node->getCond()) {
        condValue = generateExpr(node->getCond());
        if (!condValue) {
            llvm::errs() << "Erro: Condição do for inválida\n";
            return nullptr;
        }
        
        if (!condValue->getType()->isIntegerTy(1)) {
            condValue = builder->CreateICmpNE(
                condValue, 
                ConstantInt::get(condValue->getType(), 0), 
                "forcond");
        }
    } else {
        condValue = ConstantInt::get(Type::getInt1Ty(*context), 1);
    }
    
    builder->CreateCondBr(condValue, bodyBB, afterBB);
    
    // Corpo
    builder->SetInsertPoint(bodyBB);
    pushLoopBlocks(afterBB, updateBB);
    
    if (node->getBody()) {
        generateExpr(node->getBody());
    }
    
    popLoopBlocks();
    builder->CreateBr(updateBB);
    
    // Atualização
    builder->SetInsertPoint(updateBB);
    if (node->getUpdate()) {
        generateExpr(node->getUpdate());
    }
    
    builder->CreateBr(condBB);
    
    // Após o loop
    builder->SetInsertPoint(afterBB);
    
    return ConstantInt::get(*context, APInt(32, 0));
}

void LLVMGenerator::popLoopBlocks() {
    if (!breakBlocks.empty()) breakBlocks.pop_back();
    if (!continueBlocks.empty()) continueBlocks.pop_back();
}

// ============================================================================
// GERADORES PARA ATRIBUIÇÃO E ACESSO A VARIÁVEIS
// ============================================================================

Value* LLVMGenerator::generateAssign(AssignNode* node) {
    const std::string& identifier = node->getIdentifier();
    Value* target = nullptr;
    Type* targetType = nullptr;

    if (node->isArrayAccess()) {
        return generateArrayAssignment(node);
    }

    // Buscar variável local ou global
    if (namedValues.count(identifier)) {
        target = namedValues[identifier];
        targetType = variableTypes[identifier];
    }
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

    // Converter tipos se necessário
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

    // Armazenar valor
    builder->CreateStore(value, target);
    return value;
}

Value* LLVMGenerator::generateBool(BoolNode* node) {
    return ConstantInt::get(*context, APInt(1, node->getValue() ? 1 : 0));
}

Value* LLVMGenerator::generateString(StringNode* node) {
    GlobalVariable* strArray = createStringLiteral(node->getValue());
    
    // Retornar ponteiro para primeiro caractere
    Constant* zero = ConstantInt::get(Type::getInt32Ty(*context), 0);
    Constant* indices[] = {zero, zero};
    return ConstantExpr::getGetElementPtr(
        strArray->getValueType(),
        strArray,
        indices,
        true
    );
}

// ============================================================================
// GERADORES PARA CHAMADAS DE FUNÇÃO
// ============================================================================

Value* LLVMGenerator::generateCall(CallNode* node) {
    const std::string& funcName = node->getFunctionName();
    
    // Verificar se é função built-in
    if (funcName == "print") {
        return generatePrintCall(node);
    }
    
    // Buscar função no módulo
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
    
    // Preparar argumentos
    std::vector<Value*> args;
    unsigned idx = 0;
    for (const auto& argExpr : node->getArguments()) {
        Value* argValue = generateExpr(argExpr.get());
        if (!argValue) {
            llvm::errs() << "Erro: Argumento " << idx << " inválido para função '" 
                        << funcName << "'\n";
            return nullptr;
        }
        
        // Converter tipo do argumento se necessário
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
    
    // Criar chamada
    if (callee->getReturnType()->isVoidTy()) {
        return builder->CreateCall(callee, args);
    } else {
        return builder->CreateCall(callee, args, "calltmp");
    }
}

// ============================================================================
// GERADORES PARA EXPRESSÕES BINÁRIAS
// ============================================================================

Value* LLVMGenerator::generateBinaryExpr(BinaryNode* node) {
    Value* L = generateExpr(node->getLeft());
    Value* R = generateExpr(node->getRight());
    
    if (!L || !R) return nullptr;

    // Converter tipos para compatibilidade
    if (L->getType() != R->getType()) {
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
    
    // Operadores lógicos
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

    // Operadores aritméticos e de comparação
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

// ============================================================================
// GERADORES PARA IDENTIFICADORES
// ============================================================================

Value* LLVMGenerator::generateIdentifier(IdentifierNode* node) {
    // Buscar variável local
    if (namedValues.count(node->getName())) {
        Value* ptr = namedValues[node->getName()];
        if (ptr->getType()->isPointerTy()) {
            return builder->CreateLoad(variableTypes[node->getName()], ptr, 
                node->getName() + ".load");
        }
        return ptr;
    }
    
    // Buscar variável global
    if (globalValues.count(node->getName())) {
        GlobalVariable* gvar = globalValues[node->getName()];
        return builder->CreateLoad(globalTypes[node->getName()], gvar, 
            node->getName() + ".load");
    }
    
    llvm::errs() << "Erro: Variável '" << node->getName() << "' não declarada\n";
    return nullptr;
}

// ============================================================================
// GERADORES PARA FUNÇÕES
// ============================================================================

void LLVMGenerator::generateFunction(FunctionNode* node) {
    if (module->getFunction(node->getName())) {
        llvm::errs() << "Aviso: Função '" << node->getName() << "' já declarada\n";
        return;
    }

    // Salvar estado das tabelas de símbolos
    std::unordered_map<std::string, llvm::Value*> oldNamedValues = std::move(namedValues);
    std::unordered_map<std::string, llvm::Type*> oldVariableTypes = std::move(variableTypes);
    
    namedValues.clear();
    variableTypes.clear();

    // Preparar tipos dos parâmetros
    std::vector<Type*> paramTypes;
    for (const auto& param : node->getParams()) {
        Type* paramType = getLLVMType(param.first);
        if (!paramType) {
            llvm::errs() << "Erro: Tipo de parâmetro desconhecido: " << param.first << "\n";
            namedValues = std::move(oldNamedValues);
            variableTypes = std::move(oldVariableTypes);
            return;
        }
        paramTypes.push_back(paramType);
    }
    
    // Tipo de retorno
    Type* returnType = getLLVMType(node->getReturnType());
    if (!returnType) {
        llvm::errs() << "Erro: Tipo de retorno desconhecido: " << node->getReturnType() << "\n";
        namedValues = std::move(oldNamedValues);
        variableTypes = std::move(oldVariableTypes);
        return;
    }
    
    // Criar função
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
    
    // Configurar parâmetros
    unsigned idx = 0;
    for (auto& arg : func->args()) {
        const auto& param = node->getParams()[idx];
        const std::string& paramName = param.second;
        const std::string& paramTypeStr = param.first;
        
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
    
    // Gerar corpo da função
    if (node->getBody()) {
        Value* bodyValue = nullptr;
        
        if (auto blockNode = dynamic_cast<BlockNode*>(node->getBody())) {
            bodyValue = generateBlock(blockNode);
        } else {
            bodyValue = generateExpr(node->getBody());
        }
        
        // Garantir retorno se necessário
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
        // Função sem corpo (declaração apenas)
        if (returnType->isVoidTy()) {
            builder->CreateRetVoid();
        } else {
            builder->CreateUnreachable();
        }
    }
    
    // Verificar validade da função
    if (verifyFunction(*func, &llvm::errs())) {
        llvm::errs() << "Erro: Função '" << node->getName() << "' inválida\n";
        func->eraseFromParent();
    }

    // Aplicar otimizações se configurado
    if (optLevel > OptimizationLevel::O0) {
        optimizeFunction(func);
    }
    
    // Restaurar estado
    currentFunction = nullptr;
    namedValues = std::move(oldNamedValues);
    variableTypes = std::move(oldVariableTypes);
}

// ============================================================================
// MAPEAMENTO DE TIPOS
// ============================================================================

Type* LLVMGenerator::getLLVMType(const std::string& typeName) {
    // Tipos básicos
    if (typeName == "int") return Type::getInt32Ty(*context);
    if (typeName == "float") return Type::getFloatTy(*context);
    if (typeName == "bool") return Type::getInt1Ty(*context);
    if (typeName == "string") return PointerType::get(Type::getInt8Ty(*context), 0);
    if (typeName == "void") return Type::getVoidTy(*context);

    // Arrays (sintaxe: tipo[])
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

// ============================================================================
// UTILIDADES
// ============================================================================

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

