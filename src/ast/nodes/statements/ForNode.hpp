#pragma once

#include "../ASTNode.hpp"
#include "BlockNode.hpp" 

namespace ambar {

class ForNode : public ASTNode {
public:
    /**
     * @brief Construct a new ForNode object
     * 
     * @param init Expressão/declaração de inicialização (pode ser nullptr)
     * @param cond Expressão de condição (pode ser nullptr)
     * @param update Expressão de atualização (pode ser nullptr)
     * @param body Bloco de código do loop
     */
    ForNode(std::unique_ptr<ASTNode> init,
            std::unique_ptr<ASTNode> cond,
            std::unique_ptr<ASTNode> update,
            std::unique_ptr<BlockNode> body);
    
    ~ForNode() override = default;

    /**
     * @brief Get the initialization expression
     * @return ASTNode* Ponteiro para a expressão de inicialização
     */
    ASTNode* getInit() const { return init_.get(); }

    /**
     * @brief Get the condition expression
     * @return ASTNode* Ponteiro para a expressão de condição
     */
    ASTNode* getCond() const { return cond_.get(); }

    /**
     * @brief Get the update expression
     * @return ASTNode* Ponteiro para a expressão de atualização
     */
    ASTNode* getUpdate() const { return update_.get(); }

    /**
     * @brief Get the body block
     * @return BlockNode* Ponteiro para o bloco de código
     */
    BlockNode* getBody() const { return body_.get(); }

    // Visitor pattern
    void accept(ASTVisitor& visitor) override;

    // Debug representation
    std::string toString() const override;

private:
    std::unique_ptr<ASTNode> init_;
    std::unique_ptr<ASTNode> cond_;
    std::unique_ptr<ASTNode> update_;
    std::unique_ptr<BlockNode> body_;
};

} // namespace ambar