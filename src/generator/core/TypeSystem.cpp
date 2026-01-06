#include "TypeSystem.hpp"
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <sstream>

namespace ambar {

// ============================================================================
// CONSTRUTOR
// ============================================================================

TypeSystem::TypeSystem(llvm::LLVMContext& context) 
    : context(context) {
    initializeTypeCache();
}

// ========================================================================
// MAPEAMENTO DE TIPOS BÁSICOS
// ========================================================================

llvm::Type* TypeSystem::getLLVMType(const std::string& typeName) {
    // Verificar cache primeiro
    auto it = typeCache.find(typeName);
    if (it != typeCache.end()) {
        return it->second;
    }
    
    // Tipos básicos
    if (typeName == "int") {
        typeCache[typeName] = getIntType();
        return typeCache[typeName];
    }
    else if (typeName == "float") {
        typeCache[typeName] = getFloatType();
        return typeCache[typeName];
    }
    else if (typeName == "bool") {
        typeCache[typeName] = getBoolType();
        return typeCache[typeName];
    }
    else if (typeName == "string") {
        typeCache[typeName] = getStringType();
        return typeCache[typeName];
    }
    else if (typeName == "void") {
        typeCache[typeName] = getVoidType();
        return typeCache[typeName];
    }
    
    // Arrays (sintaxe: tipo[])
    if (typeName.find("[]") != std::string::npos) {
        std::string elemTypeName = extractElementTypeName(typeName);
        llvm::Type* elementType = getLLVMType(elemTypeName);
        if (elementType) {
            // Retornar ponteiro para o tipo do elemento
            llvm::PointerType* ptrType = llvm::PointerType::get(elementType, 0);
            typeCache[typeName] = ptrType;
            return ptrType;
        }
    }
    
    // Tipo não encontrado
    return nullptr;
}

std::string TypeSystem::getAmbarTypeName(llvm::Type* type) const {
    if (type == getIntType()) return "int";
    if (type == getFloatType()) return "float";
    if (type == getBoolType()) return "bool";
    if (type == getStringType()) return "string";
    if (type == getVoidType()) return "void";
    
    // Verificar se é ponteiro para array
    if (isArrayPointerType(type)) {
        llvm::Type* elemType = getArrayElementType(type);
        if (elemType) {
            std::string elemName = getAmbarTypeName(elemType);
            return elemName + "[]";
        }
    }
    
    // Verificar se é array
    if (isArrayType(type)) {
        llvm::ArrayType* arrayType = llvm::cast<llvm::ArrayType>(type);
        std::string elemName = getAmbarTypeName(arrayType->getElementType());
        std::ostringstream oss;
        oss << elemName << "[" << arrayType->getNumElements() << "]";
        return oss.str();
    }
    
    return "unknown";
}

bool TypeSystem::isValidType(const std::string& typeName) const {
    // Tipos básicos
    if (typeName == "int" || typeName == "float" || 
        typeName == "bool" || typeName == "string" || 
        typeName == "void") {
        return true;
    }
    
    // Arrays
    if (typeName.find("[]") != std::string::npos) {
        std::string elemType = typeName.substr(0, typeName.find("[]"));
        return isValidType(elemType);
    }
    
    return false;
}

// ========================================================================
// OPERAÇÕES COM ARRAYS
// ========================================================================

llvm::ArrayType* TypeSystem::createArrayType(llvm::Type* elementType, uint64_t size) {
    if (size == 0) {
        // Array sem tamanho definido (usado em parâmetros)
        return llvm::ArrayType::get(elementType, 0);
    }
    return llvm::ArrayType::get(elementType, size);
}

llvm::PointerType* TypeSystem::createArrayPointerType(llvm::Type* elementType) {
    return llvm::PointerType::get(elementType, 0);
}

llvm::Type* TypeSystem::getArrayElementType(llvm::Type* arrayType) const {
    if (arrayType->isArrayTy()) {
        llvm::ArrayType* arrType = llvm::cast<llvm::ArrayType>(arrayType);
        return arrType->getElementType();
    } 
    else if (arrayType->isPointerTy()) {
        llvm::PointerType* ptrType = llvm::cast<llvm::PointerType>(arrayType);
        llvm::Type* pointeeType = ptrType->getArrayElementType();
        
        // Verificar se é ponteiro para array
        if (pointeeType->isArrayTy()) {
            return getArrayElementType(pointeeType);
        }
        // Se não for array, assumir que é ponteiro para elemento
        return pointeeType;
    }
    
    return nullptr;
}

bool TypeSystem::isArrayType(llvm::Type* type) const {
    return type->isArrayTy();
}

bool TypeSystem::isArrayPointerType(llvm::Type* type) const {
    if (!type->isPointerTy()) return false;
    
    llvm::PointerType* ptrType = llvm::cast<llvm::PointerType>(type);
    llvm::Type* pointeeType = ptrType->getArrayElementType();
    
    return pointeeType->isArrayTy() || 
           (pointeeType->isPointerTy() && isArrayPointerType(pointeeType));
}

// ========================================================================
// COMPATIBILIDADE E CONVERSÃO
// ========================================================================

bool TypeSystem::areTypesCompatible(llvm::Type* lhsType, llvm::Type* rhsType) const {
    // Mesmo tipo
    if (lhsType == rhsType) return true;
    
    // Ambos inteiros (pode ser diferente tamanho)
    if (lhsType->isIntegerTy() && rhsType->isIntegerTy()) return true;
    
    // Ambos floats
    if (lhsType->isFloatingPointTy() && rhsType->isFloatingPointTy()) return true;
    
    // Inteiro para float ou vice-versa (com perda de precisão)
    if ((lhsType->isIntegerTy() && rhsType->isFloatingPointTy()) ||
        (lhsType->isFloatingPointTy() && rhsType->isIntegerTy())) {
        return true;
    }
    
    // Inteiro para booleano ou vice-versa
    if ((lhsType == getBoolType() && rhsType->isIntegerTy()) ||
        (lhsType->isIntegerTy() && rhsType == getBoolType())) {
        return true;
    }
    
    // Arrays compatíveis
    if (isArrayPointerType(lhsType) && isArrayPointerType(rhsType)) {
        llvm::Type* lhsElem = getArrayElementType(lhsType);
        llvm::Type* rhsElem = getArrayElementType(rhsType);
        return areTypesCompatible(lhsElem, rhsElem);
    }
    
    return false;
}

bool TypeSystem::needsImplicitConversion(llvm::Type* fromType, llvm::Type* toType) const {
    if (fromType == toType) return false;
    
    // Conversões permitidas:
    // 1. bool -> int
    // 2. int -> float
    // 3. int -> bool
    // 4. float -> int (com perda)
    
    if (fromType == getBoolType() && toType == getIntType()) return true;
    if (fromType == getIntType() && toType == getFloatType()) return true;
    if (fromType == getIntType() && toType == getBoolType()) return true;
    if (fromType == getFloatType() && toType == getIntType()) return true;
    
    return false;
}

llvm::Type* TypeSystem::getBinaryOpResultType(llvm::Type* lhsType, llvm::Type* rhsType, 
                                             const std::string& op) const {
    // Operadores aritméticos
    if (op == "+" || op == "-" || op == "*" || op == "/") {
        if (lhsType->isFloatingPointTy() || rhsType->isFloatingPointTy()) {
            return getFloatType();
        }
        return getIntType();
    }
    
    // Operadores de comparação
    if (op == "<" || op == ">" || op == "<=" || op == ">=" || 
        op == "==" || op == "!=") {
        return getBoolType();
    }
    
    // Operadores lógicos
    if (op == "&&" || op == "||") {
        return getBoolType();
    }
    
    return nullptr;
}

// ========================================================================
// UTILITÁRIOS
// ========================================================================

unsigned TypeSystem::getTypeSizeInBits(llvm::Type* type) const {
    if (type->isIntegerTy()) {
        return type->getIntegerBitWidth();
    } else if (type->isFloatingPointTy()) {
        if (type->isFloatTy()) return 32;
        if (type->isDoubleTy()) return 64;
    } else if (type->isPointerTy()) {
        // Tamanho do ponteiro (depende da arquitetura)
        return 64; // Assumindo 64-bit
    } else if (type->isArrayTy()) {
        llvm::ArrayType* arrayType = llvm::cast<llvm::ArrayType>(type);
        return getTypeSizeInBits(arrayType->getElementType()) * arrayType->getNumElements();
    }
    
    return 0;
}

std::string TypeSystem::getTypeString(llvm::Type* type) const {
    std::string typeStr;
    llvm::raw_string_ostream rso(typeStr);
    type->print(rso);
    return rso.str();
}

// ========================================================================
// MÉTODOS PRIVADOS
// ========================================================================

std::string TypeSystem::extractElementTypeName(const std::string& typeName) const {
    size_t pos = typeName.find("[]");
    if (pos != std::string::npos) {
        return typeName.substr(0, pos);
    }
    return typeName;
}

void TypeSystem::initializeTypeCache() {
    // Inicializar cache com tipos básicos
    typeCache["int"] = getIntType();
    typeCache["float"] = getFloatType();
    typeCache["bool"] = getBoolType();
    typeCache["string"] = getStringType();
    typeCache["void"] = getVoidType();
}

} // namespace ambar