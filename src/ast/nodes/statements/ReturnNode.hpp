#pragma once

#include "../ASTNode.hpp"
#include <memory>
#include <string>

namespace ambar {

class ReturnNode : public ASTNode {
public:
    /**
     * @brief Construct a new ReturnNode object
     * @param expr The expression to return (can be nullptr for void returns)
     * @param loc Source location information
     */
    explicit ReturnNode(ASTNodePtr expr = nullptr, SourceLocation loc = {})
        : ASTNode(NodeType::ReturnNode, loc), 
          expr_(std::move(expr)) {}
    
    /**
     * @brief Destroy the ReturnNode object
     */
    ~ReturnNode() override = default;
    
    /**
     * @brief Get the return expression
     * @return ASTNode* Pointer to the return expression (can be nullptr)
     */
    ASTNode* getExpr() { return expr_.get(); }
    
    /**
     * @brief Get the return expression (const version)
     * @return const ASTNode* Const pointer to the return expression (can be nullptr)
     */
    const ASTNode* getExpr() const { return expr_.get(); }
    
    /**
     * @brief Set the return expression
     * @param expr New expression to return
     */
    void setExpr(ASTNodePtr expr) { expr_ = std::move(expr); }
    
    /**
     * @brief Convert the node to a string representation
     * @return std::string String representation of the node
     */
    std::string toString() const override {
        if (expr_) {
            return "ReturnNode: " + expr_->toString();
        } else {
            return "ReturnNode: void";
        }
    }
    
    /**
     * @brief Accept a visitor for the Visitor pattern
     * @param visitor The visitor to accept
     */
    void accept(ASTVisitor& visitor) override {}

private:
    ASTNodePtr expr_;
};

} // namespace ambar