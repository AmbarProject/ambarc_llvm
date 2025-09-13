#pragma once

#include "../ASTNode.hpp"
#include <memory>
#include <string>

namespace ambar {

class IfNode : public ASTNode {
public:
    IfNode(std::unique_ptr<ASTNode> cond, 
           std::unique_ptr<ASTNode> thenBlock,
           std::unique_ptr<ASTNode> elseBlock = nullptr,
           SourceLocation loc = SourceLocation())
        : ASTNode(NodeType::IfNode, loc),
          cond_(std::move(cond)),
          thenBlock_(std::move(thenBlock)),
          elseBlock_(std::move(elseBlock)) {}
    
    ~IfNode() override = default;

    // Getters
    ASTNode* getCondition() const { return cond_.get(); }
    ASTNode* getThenBlock() const { return thenBlock_.get(); }
    ASTNode* getElseBlock() const { return elseBlock_.get(); }

    // String representation
    std::string toString() const override {
        return "IfNode";
    }

    void accept(class ASTVisitor& visitor) override { }


private:
    std::unique_ptr<ASTNode> cond_;
    std::unique_ptr<ASTNode> thenBlock_;
    std::unique_ptr<ASTNode> elseBlock_;
};

} // namespace ambar