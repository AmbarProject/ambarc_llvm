#ifndef LLVM_CONTEXT_MANAGER_HPP
#define LLVM_CONTEXT_MANAGER_HPP

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <memory>
#include <string>

namespace ambar {

/**
 * @brief Gerencia o contexto LLVM e módulo principal
 * 
 * Responsabilidade única: Gerenciar lifecycle do contexto LLVM
 * e do módulo principal.
 */
class LLVMContextManager {
public:
    /**
     * @brief Constrói um gerenciador de contexto
     * @param moduleName Nome do módulo LLVM (padrão: "ambar")
     */
    explicit LLVMContextManager(const std::string& moduleName = "ambar");
    
    ~LLVMContextManager() = default;
    
    // Não permitir cópia
    LLVMContextManager(const LLVMContextManager&) = delete;
    LLVMContextManager& operator=(const LLVMContextManager&) = delete;
    
    // Permitir movimento
    LLVMContextManager(LLVMContextManager&&) = default;
    LLVMContextManager& operator=(LLVMContextManager&&) = default;
    
    // ========================================================================
    // ACESSO AOS COMPONENTES
    // ========================================================================
    
    /**
     * @brief Obtém o contexto LLVM
     * @return Referência para o contexto LLVM
     */
    llvm::LLVMContext& getContext() { return *context; }
    
    /**
     * @brief Obtém o contexto LLVM (const)
     * @return Referência constante para o contexto LLVM
     */
    const llvm::LLVMContext& getContext() const { return *context; }
    
    /**
     * @brief Obtém o módulo LLVM
     * @return Referência para o módulo LLVM
     */
    llvm::Module& getModule() { return *module; }
    
    /**
     * @brief Obtém o módulo LLVM (const)
     * @return Referência constante para o módulo LLVM
     */
    const llvm::Module& getModule() const { return *module; }
    
    /**
     * @brief Obtém o ponteiro único para o módulo
     * @return Ponteiro único para o módulo (para transferência de ownership)
     */
    std::unique_ptr<llvm::Module> releaseModule() { return std::move(module); }
    
    /**
     * @brief Verifica se o módulo ainda é válido (não foi movido)
     * @return true se o módulo estiver disponível
     */
    bool hasModule() const { return module != nullptr; }
    
    // ========================================================================
    // UTILITÁRIOS
    // ========================================================================
    
    /**
     * @brief Cria um novo módulo com o mesmo contexto
     * @param newName Nome do novo módulo
     */
    void createNewModule(const std::string& newName = "ambar");
    
    /**
     * @brief Reseta o contexto (útil para testes)
     */
    void reset();
    
    /**
     * @brief Obtém o nome do módulo atual
     * @return Nome do módulo
     */
    std::string getModuleName() const { return moduleName; }
    
    /**
     * @brief Define o nome do módulo
     * @param name Novo nome do módulo
     */
    void setModuleName(const std::string& name);
    
private:
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::Module> module;
    std::string moduleName;
};

} // namespace ambar

#endif // LLVM_CONTEXT_MANAGER_HPP