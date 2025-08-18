#pragma once

#include "../ASTNode.hpp"
#include <memory>

namespace ambar {

class BreakNode : public ASTNode {
public:
    BreakNode() 
        : ASTNode(NodeType::BreakNode) {}

    void accept(ASTVisitor& visitor) override;
    
    std::string toString() const override {
        return "BreakNode";
    }
};

} // namespace ambar