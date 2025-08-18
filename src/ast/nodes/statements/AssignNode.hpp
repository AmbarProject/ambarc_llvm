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
               SourceLocation loc = {});

    /**
     * @brief Destroy the AssignNode object
     */
    ~AssignNode() override;

    /**
     * @brief Get the identifier being assigned to
     * @return const std::string& The variable name
     */
    const std::string& getIdentifier() const;

    /**
     * @brief Get the value expression
     * @return ASTNode* The expression being assigned
     */
    ASTNode* getValueExpr() const;

    // ASTNode interface implementation
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;

private:
    std::string identifier_;
    std::unique_ptr<ASTNode> valueExpr_;
};

} // namespace ambar