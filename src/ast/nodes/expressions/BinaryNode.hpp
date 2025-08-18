#pragma once

#include "../ASTNode.hpp"
#include <memory>
#include <string>

namespace ambar {

/**
 * @brief Represents a binary operation in the AST
 * 
 * Handles operations like +, -, *, /, &&, ||, etc.
 */
class BinaryNode : public ASTNode {
public:
    /**
     * @brief Construct a new BinaryNode object
     * 
     * @param op The operator as string (e.g., "+", "&&")
     * @param left The left operand
     * @param right The right operand
     * @param loc Source location information
     */
    BinaryNode(std::string op, 
               ASTNodePtr left, 
               ASTNodePtr right,
               SourceLocation loc = {});

    /**
     * @brief Get the operator
     * @return const std::string& The operator
     */
    const std::string& getOp() const { return op_; }

    /**
     * @brief Get the left operand
     * @return ASTNode* The left operand
     */
    ASTNode* getLeft() const { return left_.get(); }

    /**
     * @brief Get the right operand
     * @return ASTNode* The right operand
     */
    ASTNode* getRight() const { return right_.get(); }

    // ASTNode interface implementation
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;

private:
    std::string op_;
    ASTNodePtr left_;
    ASTNodePtr right_;
};

} // namespace ambar