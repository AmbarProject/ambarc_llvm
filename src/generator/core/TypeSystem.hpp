#ifndef TYPE_SYSTEM_HPP
#define TYPE_SYSTEM_HPP

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <string>
#include <unordered_map>
#include <memory>

namespace ambar {

/**
 * @brief Sistema de tipos para mapeamento entre tipos Ambar e LLVM
 * 
 * Responsabilidade única: Gerenciar tipos e conversões entre a
 * linguagem Ambar e o sistema de tipos do LLVM.
 */
class TypeSystem {
public:
    /**
     * @brief Constrói o sistema de tipos com um contexto LLVM
     * @param context Referência para o contexto LLVM
     */
    explicit TypeSystem(llvm::LLVMContext& context);
    
    ~TypeSystem() = default;
    
    // Não permitir cópia
    TypeSystem(const TypeSystem&) = delete;
    TypeSystem& operator=(const TypeSystem&) = delete;
    
    // Permitir movimento
    TypeSystem(TypeSystem&&) = default;
    TypeSystem& operator=(TypeSystem&&) = default;
    
    // ========================================================================
    // MAPEAMENTO DE TIPOS BÁSICOS
    // ========================================================================
    
    /**
     * @brief Obtém tipo LLVM para nome de tipo Ambar
     * @param typeName Nome do tipo na linguagem Ambar
     * @return Tipo LLVM correspondente (ou nullptr se desconhecido)
     */
    llvm::Type* getLLVMType(const std::string& typeName);
    
    /**
     * @brief Obtém nome Ambar para tipo LLVM
     * @param type Tipo LLVM
     * @return Nome correspondente na linguagem Ambar
     */
    std::string getAmbarTypeName(llvm::Type* type) const;
    
    /**
     * @brief Verifica se um tipo é válido na linguagem Ambar
     * @param typeName Nome do tipo a verificar
     * @return true se o tipo for suportado
     */
    bool isValidType(const std::string& typeName) const;
    
    // ========================================================================
    // TIPOS BÁSICOS PRÉ-DEFINIDOS
    // ========================================================================
    
    /** @brief Obtém tipo inteiro (32 bits) */
    llvm::Type* getIntType() const { return llvm::Type::getInt32Ty(context); }
    
    /** @brief Obtém tipo float (32 bits) */
    llvm::Type* getFloatType() const { return llvm::Type::getFloatTy(context); }
    
    /** @brief Obtém tipo booleano (1 bit) */
    llvm::Type* getBoolType() const { return llvm::Type::getInt1Ty(context); }
    
    /** @brief Obtém tipo void */
    llvm::Type* getVoidType() const { return llvm::Type::getVoidTy(context); }
    
    /** @brief Obtém tipo string (ponteiro para char) */
    llvm::PointerType* getStringType() const { 
        return llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0); 
    }
    
    // ========================================================================
    // OPERAÇÕES COM ARRAYS
    // ========================================================================
    
    /**
     * @brief Cria tipo de array LLVM
     * @param elementType Tipo dos elementos
     * @param size Tamanho do array (0 = array sem tamanho definido)
     * @return Tipo de array LLVM
     */
    llvm::ArrayType* createArrayType(llvm::Type* elementType, uint64_t size = 0);
    
    /**
     * @brief Obtém tipo de ponteiro para array
     * @param elementType Tipo dos elementos
     * @return Ponteiro para array
     */
    llvm::PointerType* createArrayPointerType(llvm::Type* elementType);
    
    /**
     * @brief Extrai tipo do elemento de um tipo de array/ponteiro
     * @param arrayType Tipo de array ou ponteiro para array
     * @return Tipo do elemento (ou nullptr se não for array)
     */
    llvm::Type* getArrayElementType(llvm::Type* arrayType) const;
    
    /**
     * @brief Verifica se um tipo é array
     * @param type Tipo a verificar
     * @return true se for tipo array
     */
    bool isArrayType(llvm::Type* type) const;
    
    /**
     * @brief Verifica se um tipo é ponteiro para array
     * @param type Tipo a verificar
     * @return true se for ponteiro para array
     */
    bool isArrayPointerType(llvm::Type* type) const;
    
    // ========================================================================
    // COMPATIBILIDADE E CONVERSÃO
    // ========================================================================
    
    /**
     * @brief Verifica se dois tipos são compatíveis para atribuição
     * @param lhsType Tipo destino
     * @param rhsType Tipo origem
     * @return true se os tipos são compatíveis
     */
    bool areTypesCompatible(llvm::Type* lhsType, llvm::Type* rhsType) const;
    
    /**
     * @brief Determina se conversão implícita é necessária
     * @param fromType Tipo origem
     * @param toType Tipo destino
     * @return true se conversão implícita é possível
     */
    bool needsImplicitConversion(llvm::Type* fromType, llvm::Type* toType) const;
    
    /**
     * @brief Obtém tipo resultante de operação binária
     * @param lhsType Tipo do operando esquerdo
     * @param rhsType Tipo do operando direito
     * @param op Operador (ex: "+", "*", "<")
     * @return Tipo resultante (ou nullptr se operação inválida)
     */
    llvm::Type* getBinaryOpResultType(llvm::Type* lhsType, llvm::Type* rhsType, 
                                      const std::string& op) const;
    
    // ========================================================================
    // UTILITÁRIOS
    // ========================================================================
    
    /**
     * @brief Obtém o contexto LLVM
     * @return Referência para o contexto LLVM
     */
    llvm::LLVMContext& getContext() { return context; }
    
    /**
     * @brief Obtém o contexto LLVM (const)
     * @return Referência constante para o contexto LLVM
     */
    const llvm::LLVMContext& getContext() const { return context; }
    
    /**
     * @brief Obtém tamanho em bits de um tipo
     * @param type Tipo LLVM
     * @return Tamanho em bits (0 para tipos sem tamanho definido)
     */
    unsigned getTypeSizeInBits(llvm::Type* type) const;
    
    /**
     * @brief Obtém string de representação do tipo
     * @param type Tipo LLVM
     * @return String representando o tipo
     */
    std::string getTypeString(llvm::Type* type) const;
    
private:
    llvm::LLVMContext& context;
    
    // Cache para tipos criados dinamicamente
    mutable std::unordered_map<std::string, llvm::Type*> typeCache;
    
    /**
     * @brief Mapeia nome de tipo array para tipo elemento
     * @param typeName Nome do tipo array (ex: "int[]")
     * @return Nome do tipo elemento (ex: "int")
     */
    std::string extractElementTypeName(const std::string& typeName) const;
    
    /**
     * @brief Inicializa cache de tipos básicos
     */
    void initializeTypeCache();
};

} // namespace ambar

#endif // TYPE_SYSTEM_HPP