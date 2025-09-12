#pragma once

#include "../ASTNode.hpp"
#include <string>

namespace ambar {

class IdentifierNode : public ASTNode {
public:
    explicit IdentifierNode(const std::string& name, SourceLocation loc = {})
        : ASTNode(ASTNode::NodeType::IdentifierNode, loc), name_(name) {}

    const std::string& getName() const { return name_; }

    void accept(ASTVisitor& visitor) override {} 
    
    std::string toString() const override {
        return "IdentifierNode(name: " + name_ + ")";
    }

private:
    std::string name_;
};

} // namespace ambar