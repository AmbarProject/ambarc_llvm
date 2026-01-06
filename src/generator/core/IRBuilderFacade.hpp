#ifndef IR_BUILDER_FACADE_HPP
#define IR_BUILDER_FACADE_HPP

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <memory>

namespace ambar {

/**
 * @brief Fachada para o IRBuilder do LLVM com métodos utilitários
 * 
 * Responsabilidade única: Fornecer uma interface simplificada
 * para operações comuns de construção de IR.
 */
class IRBuilderFacade {
public:
    /**
     * @brief Constrói a fachada com um contexto LLVM
     * @param context Referência para o contexto LLVM
     */
    explicit IRBuilderFacade(llvm::LLVMContext& context);
    
    ~IRBuilderFacade() = default;
    
    // Não permitir cópia
    IRBuilderFacade(const IRBuilderFacade&) = delete;
    IRBuilderFacade& operator=(const IRBuilderFacade&) = delete;
    
    // Permitir movimento
    IRBuilderFacade(IRBuilderFacade&&) = default;
    IRBuilderFacade& operator=(IRBuilderFacade&&) = default;
    
    // ========================================================================
    // ACESSO AO IRBUILDER
    // ========================================================================
    
    /**
     * @brief Obtém o IRBuilder interno
     * @return Referência para o IRBuilder
     */
    llvm::IRBuilder<>& getBuilder() { return *builder; }
    
    /**
     * @brief Obtém o IRBuilder interno (const)
     * @return Referência constante para o IRBuilder
     */
    const llvm::IRBuilder<>& getBuilder() const { return *builder; }
    
    // ========================================================================
    // CONTROLE DE INSERÇÃO
    // ========================================================================
    
    /**
     * @brief Define o ponto de inserção atual
     * @param block Bloco básico onde novas instruções serão inseridas
     */
    void setInsertPoint(llvm::BasicBlock* block) { builder->SetInsertPoint(block); }
    
    /**
     * @brief Obtém o bloco básico atual de inserção
     * @return Bloco básico atual ou nullptr
     */
    llvm::BasicBlock* getInsertBlock() const { return builder->GetInsertBlock(); }
    
    /**
     * @brief Limpa o ponto de inserção atual
     */
    void clearInsertionPoint() { builder->ClearInsertionPoint(); }
    
    /**
     * @brief Restaura o ponto de inserção para um bloco salvo
     * @param block Bloco básico para restaurar
     */
    void restoreInsertionPoint(llvm::BasicBlock* block) { 
        if (block) builder->SetInsertPoint(block); 
    }
    
    // ========================================================================
    // OPERAÇÕES ARITMÉTICAS
    // ========================================================================
    
    /**
     * @brief Cria operação de adição
     * @param lhs Valor à esquerda
     * @param rhs Valor à direita
     * @param name Nome da instrução (opcional)
     * @return Valor resultante da adição
     */
    llvm::Value* createAdd(llvm::Value* lhs, llvm::Value* rhs, 
                           const std::string& name = "");
    
    /**
     * @brief Cria operação de subtração
     * @param lhs Valor à esquerda
     * @param rhs Valor à direita
     * @param name Nome da instrução (opcional)
     * @return Valor resultante da subtração
     */
    llvm::Value* createSub(llvm::Value* lhs, llvm::Value* rhs, 
                           const std::string& name = "");
    
    /**
     * @brief Cria operação de multiplicação
     * @param lhs Valor à esquerda
     * @param rhs Valor à direita
     * @param name Nome da instrução (opcional)
     * @return Valor resultante da multiplicação
     */
    llvm::Value* createMul(llvm::Value* lhs, llvm::Value* rhs, 
                           const std::string& name = "");
    
    /**
     * @brief Cria operação de divisão
     * @param lhs Valor à esquerda
     * @param rhs Valor à direita
     * @param name Nome da instrução (opcional)
     * @return Valor resultante da divisão
     */
    llvm::Value* createDiv(llvm::Value* lhs, llvm::Value* rhs, 
                           const std::string& name = "");
    
    // ========================================================================
    // OPERAÇÕES DE COMPARAÇÃO
    // ========================================================================
    
    /**
     * @brief Cria comparação "igual a"
     * @param lhs Valor à esquerda
     * @param rhs Valor à direita
     * @param name Nome da instrução (opcional)
     * @return Valor booleano resultante
     */
    llvm::Value* createEqual(llvm::Value* lhs, llvm::Value* rhs, 
                             const std::string& name = "");
    
    /**
     * @brief Cria comparação "diferente de"
     * @param lhs Valor à esquerda
     * @param rhs Valor à direita
     * @param name Nome da instrução (opcional)
     * @return Valor booleano resultante
     */
    llvm::Value* createNotEqual(llvm::Value* lhs, llvm::Value* rhs, 
                                const std::string& name = "");
    
    /**
     * @brief Cria comparação "menor que"
     * @param lhs Valor à esquerda
     * @param rhs Valor à direita
     * @param name Nome da instrução (opcional)
     * @return Valor booleano resultante
     */
    llvm::Value* createLessThan(llvm::Value* lhs, llvm::Value* rhs, 
                                const std::string& name = "");
    
    /**
     * @brief Cria comparação "maior que"
     * @param lhs Valor à esquerda
     * @param rhs Valor à direita
     * @param name Nome da instrução (opcional)
     * @return Valor booleano resultante
     */
    llvm::Value* createGreaterThan(llvm::Value* lhs, llvm::Value* rhs, 
                                   const std::string& name = "");
    
    // ========================================================================
    // OPERAÇÕES LÓGICAS
    // ========================================================================
    
    /**
     * @brief Cria operação lógica AND
     * @param lhs Valor à esquerda
     * @param rhs Valor à direita
     * @param name Nome da instrução (opcional)
     * @return Valor booleano resultante
     */
    llvm::Value* createAnd(llvm::Value* lhs, llvm::Value* rhs, 
                           const std::string& name = "");
    
    /**
     * @brief Cria operação lógica OR
     * @param lhs Valor à esquerda
     * @param rhs Valor à direita
     * @param name Nome da instrução (opcional)
     * @return Valor booleano resultante
     */
    llvm::Value* createOr(llvm::Value* lhs, llvm::Value* rhs, 
                          const std::string& name = "");
    
    /**
     * @brief Cria operação lógica NOT
     * @param value Valor a ser negado
     * @param name Nome da instrução (opcional)
     * @return Valor booleano resultante
     */
    llvm::Value* createNot(llvm::Value* value, const std::string& name = "");
    
    // ========================================================================
    // OPERAÇÕES DE MEMÓRIA
    // ========================================================================
    
    /**
     * @brief Aloca memória na stack
     * @param type Tipo a ser alocado
     * @param name Nome da variável (opcional)
     * @return Ponteiro para a memória alocada
     */
    llvm::Value* createAlloca(llvm::Type* type, const std::string& name = "");
    
    /**
     * @brief Carrega valor da memória
     * @param type Tipo do valor a ser carregado
     * @param ptr Ponteiro para a memória
     * @param name Nome da instrução (opcional)
     * @return Valor carregado
     */
    llvm::Value* createLoad(llvm::Type* type, llvm::Value* ptr, 
                            const std::string& name = "");
    
    /**
     * @brief Armazena valor na memória
     * @param value Valor a ser armazenado
     * @param ptr Ponteiro para a memória
     */
    void createStore(llvm::Value* value, llvm::Value* ptr);
    
    // ========================================================================
    // CONVERSÃO DE TIPOS
    // ========================================================================
    
    /**
     * @brief Converte inteiro para booleano (zero/não-zero)
     * @param value Valor inteiro
     * @param name Nome da instrução (opcional)
     * @return Valor booleano
     */
    llvm::Value* createIntToBool(llvm::Value* value, const std::string& name = "");
    
    /**
     * @brief Converte booleano para inteiro (zero-extension)
     * @param value Valor booleano
     * @param name Nome da instrução (opcional)
     * @return Valor inteiro (32 bits)
     */
    llvm::Value* createBoolToInt(llvm::Value* value, const std::string& name = "");
    
    /**
     * @brief Converte inteiro para float
     * @param value Valor inteiro
     * @param name Nome da instrução (opcional)
     * @return Valor float
     */
    llvm::Value* createIntToFloat(llvm::Value* value, const std::string& name = "");
    
    /**
     * @brief Converte float para inteiro
     * @param value Valor float
     * @param name Nome da instrução (opcional)
     * @return Valor inteiro
     */
    llvm::Value* createFloatToInt(llvm::Value* value, const std::string& name = "");
    
    /**
     * @brief Estende float para double (para chamadas printf)
     * @param value Valor float
     * @param name Nome da instrução (opcional)
     * @return Valor double
     */
    llvm::Value* createFloatToDouble(llvm::Value* value, const std::string& name = "");
    
    // ========================================================================
    // CONTROLE DE FLUXO
    // ========================================================================
    
    /**
     * @brief Cria branch condicional
     * @param cond Condição booleana
     * @param thenBlock Bloco destino se verdadeiro
     * @param elseBlock Bloco destino se falso (opcional)
     */
    void createCondBranch(llvm::Value* cond, llvm::BasicBlock* thenBlock, 
                          llvm::BasicBlock* elseBlock = nullptr);
    
    /**
     * @brief Cria branch incondicional
     * @param dest Bloco destino
     */
    void createBranch(llvm::BasicBlock* dest);
    
    /**
     * @brief Cria retorno void
     */
    void createRetVoid();
    
    /**
     * @brief Cria retorno com valor
     * @param value Valor a ser retornado
     */
    void createRet(llvm::Value* value);
    
    // ========================================================================
    // CONSTANTES
    // ========================================================================
    
    /**
     * @brief Cria constante inteira
     * @param value Valor inteiro
     * @param bits Tamanho em bits (padrão: 32)
     * @return Valor constante
     */
    llvm::ConstantInt* createIntConstant(int value, unsigned bits = 32);
    
    /**
     * @brief Cria constante float
     * @param value Valor float
     * @return Valor constante
     */
    llvm::ConstantFP* createFloatConstant(float value);
    
    /**
     * @brief Cria constante booleana
     * @param value Valor booleano
     * @return Valor constante
     */
    llvm::ConstantInt* createBoolConstant(bool value);
    
    /**
     * @brief Cria constante zero para um tipo
     * @param type Tipo do zero
     * @return Valor constante zero
     */
    llvm::Constant* createZero(llvm::Type* type);
    
private:
    std::unique_ptr<llvm::IRBuilder<>> builder;
};

} // namespace ambar

#endif // IR_BUILDER_FACADE_HPP