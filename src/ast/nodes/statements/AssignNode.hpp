#pragma once

#include "../ASTNode.hpp"
#include <string>
#include <memory>

namespace ambar {

/**
 * @brief Node for variable assignment statements
 * 
 * Represents an assignment operation like `x = 42`
 */
class AssignNode : public ASTNode {
public:
    /**
     * @brief Construct a new AssignNode object
     * 
     * @param identifier Name of the variable being assigned to
     * @param valueExpr Expression being assigned
     * @param loc Source location of the assignment
     */
    AssignNode(const std::string& identifier, 
               std::unique_ptr<ASTNode> valueExpr,
               SourceLocation loc = {})
        : ASTNode(ASTNode::NodeType::AssignNode, loc),
          identifier_(identifier),
          valueExpr_(std::move(valueExpr)) {}

    /**
     * @brief Destroy the AssignNode object
     */
    ~AssignNode() override = default;

    /**
     * @brief Get the identifier being assigned to
     * @return const std::string& The variable name
     */
    const std::string& getIdentifier() const { return identifier_; }

    /**
     * @brief Get the value expression
     * @return ASTNode* The expression being assigned
     */
    ASTNode* getValueExpr() const { return valueExpr_.get(); }

    void accept(ASTVisitor& visitor) override {} 

    /*
     * @brief Get the value expression as unique_ptr (for ownership transfer)
     */
    std::unique_ptr<ASTNode> takeValueExpr() { return std::move(valueExpr_); }

    // ASTNode interface implementation
    std::string toString() const override {
        return "AssignNode(to: " + identifier_ + ")";
    }

private:
    std::string identifier_;
    std::unique_ptr<ASTNode> valueExpr_;
};

} // namespace ambar