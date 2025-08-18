#pragma once

#include "../ASTNode.hpp"
#include <memory>

namespace ambar {

class ReturnNode : public ASTNode {
public:
    /**
     * @brief Construct a new ReturnNode object
     * @param expr The expression to return (can be nullptr for void returns)
     */
    explicit ReturnNode(ASTNodePtr expr = nullptr);
    
    /**
     * @brief Destroy the ReturnNode object
     */
    ~ReturnNode() override = default;
    
    /**
     * @brief Get the return expression
     * @return ASTNodePtr& Reference to the return expression
     */
    ASTNodePtr& getExpr() { return expr_; }
    
    /**
     * @brief Get the return expression (const version)
     * @return const ASTNodePtr& Const reference to the return expression
     */
    const ASTNodePtr& getExpr() const { return expr_; }
    
    /**
     * @brief Convert the node to a string representation
     * @return std::string String representation of the node
     */
    std::string toString() const override;
    
    /**
     * @brief Accept a visitor for the Visitor pattern
     * @param visitor The visitor to accept
     */
    void accept(ASTVisitor& visitor) override;

private:
    ASTNodePtr expr_;
};

} // namespace ambar