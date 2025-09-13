#pragma once

#include "../ASTNode.hpp"
#include <memory>

namespace ambar {

class WhileNode : public ASTNode {
public:
    /**
     * @brief Construct a new WhileNode object
     * @param condition The loop condition expression
     * @param body The loop body statement/block
     */
    WhileNode(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> body)
        : ASTNode(ASTNode::NodeType::WhileNode),
          condition_(std::move(condition)),
          body_(std::move(body)) {}
    
    /**
     * @brief Destroy the WhileNode object
     */
    ~WhileNode() override = default;
    
    /**
     * @brief Accept a visitor for the Visitor pattern
     * @param visitor The visitor to accept
     */
    void accept(ASTVisitor& visitor) override {}
    
    /**
     * @brief Get the string representation of the node
     * @return std::string The string representation
     */
    std::string toString() const override {
        return "WhileNode";
    }
    
    /**
     * @brief Get the loop condition
     * @return ASTNode* The condition expression
     */
    ASTNode* getCondition() const {
        return condition_.get();
    }
    
    /**
     * @brief Get the loop body
     * @return ASTNode* The body statement/block
     */
    ASTNode* getBody() const {
        return body_.get();
    }

private:
    std::unique_ptr<ASTNode> condition_;
    std::unique_ptr<ASTNode> body_;
};

} // namespace ambar