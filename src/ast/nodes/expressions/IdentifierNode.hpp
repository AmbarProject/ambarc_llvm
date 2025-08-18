#pragma once

#include "../ASTNode.hpp"
#include <string>

namespace ambar {

class IdentifierNode : public ASTNode {
public:
    /**
     * @brief Construct a new Identifier Node object
     * @param name The name of the identifier
     */
    explicit IdentifierNode(const std::string& name);
    
    /**
     * @brief Destroy the Identifier Node object
     */
    ~IdentifierNode() override = default;
    
    /**
     * @brief Get the name of the identifier
     * @return const std::string& The identifier name
     */
    const std::string& getName() const;
    
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
    std::string name_;
};

} // namespace ambar