#pragma once

#include "../ASTNode.hpp"
#include <vector>

namespace ambar {

class ContinueNode : public ASTNode {
public:
    ContinueNode()
        : ASTNode(ASTNode::NodeType::ContinueNode) {}
    
    ~ContinueNode() override = default;

    std::string toString() const override {
        return "ContinueNode";
    }

    void accept(ASTVisitor& visitor) override {}
private:
};

} // namespace ambar