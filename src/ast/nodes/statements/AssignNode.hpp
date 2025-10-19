#pragma once

#include "../ASTNode.hpp"
#include <string>
#include <memory>

namespace ambar {

/**
 * @brief Node for variable assignment statements
 * 
 * Represents an assignment operation like `x = 42` or `arr[i] = value`
 */
class AssignNode : public ASTNode {
public:
    /**
     * @brief Construct a new AssignNode object for regular variable assignment
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
          valueExpr_(std::move(valueExpr)),
          isArrayAccess_(false) {}

    /**
     * @brief Construct a new AssignNode object for array element assignment
     * 
     * @param identifier Name of the array being accessed
     * @param valueExpr Expression being assigned
     * @param indexExpr Index expression for array access
     * @param loc Source location of the assignment
     */
    AssignNode(const std::string& identifier, 
               std::unique_ptr<ASTNode> valueExpr,
               std::unique_ptr<ASTNode> indexExpr,
               SourceLocation loc = {})
        : ASTNode(ASTNode::NodeType::AssignNode, loc),
          identifier_(identifier),
          valueExpr_(std::move(valueExpr)),
          indexExpr_(std::move(indexExpr)),
          isArrayAccess_(true) {}

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

    /**
     * @brief Get the index expression (for array assignments)
     * @return ASTNode* The index expression, or nullptr if not an array assignment
     */
    ASTNode* getIndexExpr() const { return indexExpr_.get(); }

    /**
     * @brief Check if this is an array element assignment
     * @return true if this assigns to an array element, false for regular variable
     */
    bool isArrayAccess() const { return isArrayAccess_; }

    void accept(ASTVisitor& visitor) override {
        // TODO: Implement visitor pattern for assignment nodes
    } 

    /**
     * @brief Get the value expression as unique_ptr (for ownership transfer)
     */
    std::unique_ptr<ASTNode> takeValueExpr() { return std::move(valueExpr_); }

    /**
     * @brief Get the index expression as unique_ptr (for ownership transfer)
     */
    std::unique_ptr<ASTNode> takeIndexExpr() { return std::move(indexExpr_); }

    // ASTNode interface implementation
    std::string toString() const override {
        if (isArrayAccess_) {
            return "AssignNode(array: " + identifier_ + "[index])";
        } else {
            return "AssignNode(to: " + identifier_ + ")";
        }
    }

private:
    std::string identifier_;
    std::unique_ptr<ASTNode> valueExpr_;
    std::unique_ptr<ASTNode> indexExpr_;
    bool isArrayAccess_;
};

} // namespace ambar
