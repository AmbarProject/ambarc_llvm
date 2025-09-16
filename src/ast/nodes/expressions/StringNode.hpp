#pragma once

#include <string>
#include "../ASTNode.hpp"

namespace ambar {

class StringNode : public ASTNode {
public:
    explicit StringNode(const std::string& value);
    
    const std::string& getValue() const;
    
    void accept(ASTVisitor& visitor) override {} 

    std::string toString() const override;

private:
    std::string value_;
};

// Implementação inline para remover a necessidade do .cpp
inline StringNode::StringNode(const std::string& value)
    : ASTNode(ASTNode::NodeType::StringNode, {}), value_(value) {}

inline const std::string& StringNode::getValue() const {
    return value_;
}

inline std::string StringNode::toString() const {
    return "StringNode(value: \"" + value_ + "\")";
}

} // namespace ambar
