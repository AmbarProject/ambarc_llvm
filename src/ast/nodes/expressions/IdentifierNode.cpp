#include "IdentifierNode.hpp"
#include "../../visitors/ASTVisitor.hpp"

namespace ambar {

IdentifierNode::IdentifierNode(const std::string& name)
    : ASTNode(ASTNode::NodeType::IdentifierExpr), name_(name) {}

const std::string& IdentifierNode::getName() const {
    return name_;
}

std::string IdentifierNode::toString() const {
    return "IdentifierNode(name: " + name_ + ")";
}

void IdentifierNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

} // namespace ambar