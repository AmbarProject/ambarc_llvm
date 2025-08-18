#pragma once

#include "../ASTNode.hpp"
#include <memory>
#include <vector>

namespace ambar {

class IfNode : public ASTNode {
public:
    IfNode(std::unique_ptr<ASTNode> cond, 
           std::unique_ptr<ASTNode> thenBlock,
           std::unique_ptr<ASTNode> elseBlock = nullptr);
    
    ~IfNode() override = default;

    // Getters
    ASTNode* getCondition() const { return cond_.get(); }
    ASTNode* getThenBlock() const { return thenBlock_.get(); }
    ASTNode* getElseBlock() const { return elseBlock_.get(); }

    // Visitor pattern
    void accept(ASTVisitor& visitor) override;

    // String representation
    std::string toString() const override;

private:
    std::unique_ptr<ASTNode> cond_;
    std::unique_ptr<ASTNode> thenBlock_;
    std::unique_ptr<ASTNode> elseBlock_;
};

} // namespace ambar