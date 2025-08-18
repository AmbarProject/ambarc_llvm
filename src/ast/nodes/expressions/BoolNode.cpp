#include "BoolNode.hpp"
#include "../../visitors/ASTVisitor.hpp"

namespace ambar {

BoolNode::BoolNode(bool value)
    : ASTNode(ASTNode::NodeType::BoolExpr), value_(value) {}

bool BoolNode::getValue() const {
    return value_;
}

void BoolNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string BoolNode::toString() const {
    return "BoolNode(value: " + std::string(value_ ? "true" : "false") + ")";
}

} // namespace ambar