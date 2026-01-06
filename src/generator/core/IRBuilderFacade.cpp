#include "IRBuilderFacade.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>

namespace ambar {

// ============================================================================
// CONSTRUTOR
// ============================================================================

IRBuilderFacade::IRBuilderFacade(llvm::LLVMContext& context) {
    builder = std::make_unique<llvm::IRBuilder<>>(context);
}

// ========================================================================
// OPERAÇÕES ARITMÉTICAS
// ========================================================================

llvm::Value* IRBuilderFacade::createAdd(llvm::Value* lhs, llvm::Value* rhs, 
                                        const std::string& name) {
    if (lhs->getType()->isFloatingPointTy()) {
        return builder->CreateFAdd(lhs, rhs, name.empty() ? "addtmp" : name);
    } else {
        return builder->CreateAdd(lhs, rhs, name.empty() ? "addtmp" : name);
    }
}

llvm::Value* IRBuilderFacade::createSub(llvm::Value* lhs, llvm::Value* rhs, 
                                        const std::string& name) {
    if (lhs->getType()->isFloatingPointTy()) {
        return builder->CreateFSub(lhs, rhs, name.empty() ? "subtmp" : name);
    } else {
        return builder->CreateSub(lhs, rhs, name.empty() ? "subtmp" : name);
    }
}

llvm::Value* IRBuilderFacade::createMul(llvm::Value* lhs, llvm::Value* rhs, 
                                        const std::string& name) {
    if (lhs->getType()->isFloatingPointTy()) {
        return builder->CreateFMul(lhs, rhs, name.empty() ? "multmp" : name);
    } else {
        return builder->CreateMul(lhs, rhs, name.empty() ? "multmp" : name);
    }
}

llvm::Value* IRBuilderFacade::createDiv(llvm::Value* lhs, llvm::Value* rhs, 
                                        const std::string& name) {
    if (lhs->getType()->isFloatingPointTy()) {
        return builder->CreateFDiv(lhs, rhs, name.empty() ? "divtmp" : name);
    } else {
        return builder->CreateSDiv(lhs, rhs, name.empty() ? "divtmp" : name);
    }
}

// ========================================================================
// OPERAÇÕES DE COMPARAÇÃO
// ========================================================================

llvm::Value* IRBuilderFacade::createEqual(llvm::Value* lhs, llvm::Value* rhs, 
                                         const std::string& name) {
    if (lhs->getType()->isFloatingPointTy()) {
        return builder->CreateFCmpOEQ(lhs, rhs, name.empty() ? "eqtmp" : name);
    } else {
        return builder->CreateICmpEQ(lhs, rhs, name.empty() ? "eqtmp" : name);
    }
}

llvm::Value* IRBuilderFacade::createNotEqual(llvm::Value* lhs, llvm::Value* rhs, 
                                            const std::string& name) {
    if (lhs->getType()->isFloatingPointTy()) {
        return builder->CreateFCmpONE(lhs, rhs, name.empty() ? "netmp" : name);
    } else {
        return builder->CreateICmpNE(lhs, rhs, name.empty() ? "netmp" : name);
    }
}

llvm::Value* IRBuilderFacade::createLessThan(llvm::Value* lhs, llvm::Value* rhs, 
                                            const std::string& name) {
    if (lhs->getType()->isFloatingPointTy()) {
        return builder->CreateFCmpOLT(lhs, rhs, name.empty() ? "lttmp" : name);
    } else {
        return builder->CreateICmpSLT(lhs, rhs, name.empty() ? "lttmp" : name);
    }
}

llvm::Value* IRBuilderFacade::createGreaterThan(llvm::Value* lhs, llvm::Value* rhs, 
                                               const std::string& name) {
    if (lhs->getType()->isFloatingPointTy()) {
        return builder->CreateFCmpOGT(lhs, rhs, name.empty() ? "gttmp" : name);
    } else {
        return builder->CreateICmpSGT(lhs, rhs, name.empty() ? "gttmp" : name);
    }
}

// ========================================================================
// OPERAÇÕES LÓGICAS
// ========================================================================

llvm::Value* IRBuilderFacade::createAnd(llvm::Value* lhs, llvm::Value* rhs, 
                                       const std::string& name) {
    return builder->CreateAnd(lhs, rhs, name.empty() ? "andtmp" : name);
}

llvm::Value* IRBuilderFacade::createOr(llvm::Value* lhs, llvm::Value* rhs, 
                                      const std::string& name) {
    return builder->CreateOr(lhs, rhs, name.empty() ? "ortmp" : name);
}

llvm::Value* IRBuilderFacade::createNot(llvm::Value* value, const std::string& name) {
    return builder->CreateNot(value, name.empty() ? "nottmp" : name);
}

// ========================================================================
// OPERAÇÕES DE MEMÓRIA
// ========================================================================

llvm::Value* IRBuilderFacade::createAlloca(llvm::Type* type, const std::string& name) {
    return builder->CreateAlloca(type, nullptr, name.empty() ? "alloca" : name);
}

llvm::Value* IRBuilderFacade::createLoad(llvm::Type* type, llvm::Value* ptr, 
                                        const std::string& name) {
    return builder->CreateLoad(type, ptr, name.empty() ? "loadtmp" : name);
}

void IRBuilderFacade::createStore(llvm::Value* value, llvm::Value* ptr) {
    builder->CreateStore(value, ptr);
}

// ========================================================================
// CONVERSÃO DE TIPOS
// ========================================================================

llvm::Value* IRBuilderFacade::createIntToBool(llvm::Value* value, const std::string& name) {
    llvm::Type* intType = value->getType();
    if (!intType->isIntegerTy()) {
        // Se não for inteiro, tentar converter primeiro
        if (intType->isFloatingPointTy()) {
            // Converter float para int primeiro
            value = builder->CreateFPToSI(value, llvm::Type::getInt32Ty(builder->getContext()));
        } else {
            // Tipo não suportado
            return nullptr;
        }
    }
    
    // Comparar com zero para obter booleano
    llvm::Value* zero = llvm::ConstantInt::get(value->getType(), 0);
    return builder->CreateICmpNE(value, zero, name.empty() ? "booltmp" : name);
}

llvm::Value* IRBuilderFacade::createBoolToInt(llvm::Value* value, const std::string& name) {
    if (!value->getType()->isIntegerTy(1)) {
        // Se não for booleano, converter primeiro
        value = createIntToBool(value, "tmpbool");
    }
    
    // Zero-extend de 1 bit para 32 bits
    return builder->CreateZExt(value, llvm::Type::getInt32Ty(builder->getContext()), 
                              name.empty() ? "inttmp" : name);
}

llvm::Value* IRBuilderFacade::createIntToFloat(llvm::Value* value, const std::string& name) {
    return builder->CreateSIToFP(value, llvm::Type::getFloatTy(builder->getContext()), 
                                name.empty() ? "floattmp" : name);
}

llvm::Value* IRBuilderFacade::createFloatToInt(llvm::Value* value, const std::string& name) {
    return builder->CreateFPToSI(value, llvm::Type::getInt32Ty(builder->getContext()), 
                                name.empty() ? "inttmp" : name);
}

llvm::Value* IRBuilderFacade::createFloatToDouble(llvm::Value* value, const std::string& name) {
    return builder->CreateFPExt(value, llvm::Type::getDoubleTy(builder->getContext()), 
                               name.empty() ? "doubletmp" : name);
}

// ========================================================================
// CONTROLE DE FLUXO
// ========================================================================

void IRBuilderFacade::createCondBranch(llvm::Value* cond, llvm::BasicBlock* thenBlock, 
                                      llvm::BasicBlock* elseBlock) {
    if (elseBlock) {
        builder->CreateCondBr(cond, thenBlock, elseBlock);
    } else {
        builder->CreateCondBr(cond, thenBlock, thenBlock);
    }
}

void IRBuilderFacade::createBranch(llvm::BasicBlock* dest) {
    builder->CreateBr(dest);
}

void IRBuilderFacade::createRetVoid() {
    builder->CreateRetVoid();
}

void IRBuilderFacade::createRet(llvm::Value* value) {
    builder->CreateRet(value);
}

// ========================================================================
// CONSTANTES
// ========================================================================

llvm::ConstantInt* IRBuilderFacade::createIntConstant(int value, unsigned bits) {
    return llvm::ConstantInt::get(builder->getContext(), llvm::APInt(bits, value));
}

llvm::ConstantFP* IRBuilderFacade::createFloatConstant(float value) {
    return llvm::ConstantFP::get(builder->getContext(), llvm::APFloat(value));
}

llvm::ConstantInt* IRBuilderFacade::createBoolConstant(bool value) {
    return llvm::ConstantInt::get(builder->getContext(), llvm::APInt(1, value ? 1 : 0));
}

llvm::Constant* IRBuilderFacade::createZero(llvm::Type* type) {
    if (type->isIntegerTy()) {
        return llvm::ConstantInt::get(type, 0);
    } else if (type->isFloatingPointTy()) {
        return llvm::ConstantFP::get(type, 0.0);
    } else if (type->isPointerTy()) {
        return llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(type));
    } else if (type->isArrayTy()) {
        llvm::ArrayType* arrayType = llvm::cast<llvm::ArrayType>(type);
        llvm::Type* elemType = arrayType->getElementType();
        llvm::Constant* zeroElem = createZero(elemType);
        std::vector<llvm::Constant*> zeros(arrayType->getNumElements(), zeroElem);
        return llvm::ConstantArray::get(arrayType, zeros);
    }
    
    return nullptr;
}

} // namespace ambar