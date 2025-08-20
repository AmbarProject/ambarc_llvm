// BoolNode.hpp
#pragma once

#include "../ASTNode.hpp"

namespace ambar {

/**
 * @brief Node representing a boolean literal (true/false) in the AST
 */
class BoolNode : public ASTNode {
public:
    /**
     * @brief Construct a new BoolNode object
     * @param value The boolean value (true or false)
     */
    explicit BoolNode(bool value) 
        : ASTNode(ASTNode::NodeType::BoolNode), value_(value) {}

    /**
     * @brief Get the boolean value
     * @return bool The stored boolean value
     */
    bool getValue() const { return value_; }

    void accept(ASTVisitor& visitor) override {} 

    /**
     * @brief Convert the node to a string representation
     * @return std::string String representation of the node
     */
    std::string toString() const override {
        return "BoolNode(value: " + std::string(value_ ? "true" : "false") + ")";
    }

private:
    bool value_;
};

} // namespace ambar