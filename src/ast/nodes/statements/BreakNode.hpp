#pragma once

#include "../ASTNode.hpp"
#include <memory>

namespace ambar {

class BreakNode : public ASTNode {
public:
    BreakNode() 
        : ASTNode(NodeType::BreakNode) {}
    
    std::string toString() const override {
        return "BreakNode";
    }

    void accept(ASTVisitor& visitor) override { }

};

} // namespace ambar