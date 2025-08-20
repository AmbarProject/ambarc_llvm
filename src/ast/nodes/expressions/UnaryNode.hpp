// src/ast/nodes/Expressions/UnaryNode.hpp
#pragma once

#include "../ASTNode.hpp"
#include <string>
#include <memory>

namespace ambar {

class UnaryNode : public ASTNode {
public:
    /**
     * @brief Construct a new UnaryNode object
     * @param op Operator (e.g., "-", "!", "~")
     * @param operand The operand expression
     */
    UnaryNode(const std::string& op, std::unique_ptr<ASTNode> operand, SourceLocation loc = {})
        : ASTNode(NodeType::UnaryNode, loc), op_(op), operand_(std::move(operand)) {}

    /**
     * @brief Get the operator
     * @return const std::string& The operator
     */
    const std::string& getOp() const { return op_; }

    void accept(ASTVisitor& visitor) override {} 

    /**
     * @brief Get the operand
     * @return ASTNode* The operand expression
     */
    ASTNode* getOperand() const { return operand_.get(); }

    std::string toString() const override {
        return "UnaryNode: " + op_;
    }

private:
    std::string op_;
    std::unique_ptr<ASTNode> operand_;
};

} // namespace ambar