#include "LLVMContextManager.hpp"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

namespace ambar {

// ============================================================================
// CONSTRUTOR E DESTRUTOR
// ============================================================================

LLVMContextManager::LLVMContextManager(const std::string& moduleName)
    : moduleName(moduleName) {
    // Criar contexto LLVM
    context = std::make_unique<llvm::LLVMContext>();
    
    // Criar módulo com o nome especificado
    module = std::make_unique<llvm::Module>(moduleName, *context);
}

// ========================================================================
// MÉTODOS PÚBLICOS
// ========================================================================

void LLVMContextManager::createNewModule(const std::string& newName) {
    // Liberar módulo antigo se existir
    if (module) {
        module.reset();
    }
    
    // Criar novo módulo
    moduleName = newName;
    module = std::make_unique<llvm::Module>(newName, *context);
}

void LLVMContextManager::reset() {
    // Resetar completamente o contexto
    context.reset();
    module.reset();
    
    // Recriar com os mesmos parâmetros
    context = std::make_unique<llvm::LLVMContext>();
    module = std::make_unique<llvm::Module>(moduleName, *context);
}

void LLVMContextManager::setModuleName(const std::string& name) {
    if (module) {
        module->setModuleIdentifier(name);
    }
    moduleName = name;
}

} // namespace ambar