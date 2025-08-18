#include "StringNode.hpp"
#include "../../visitors/ASTVisitor.hpp"

namespace ambar {

StringNode::StringNode(const std::string& value)
    : ASTNode(ASTNode::NodeType::StringExpr), value_(value) {}

const std::string& StringNode::getValue() const {
    return value_;
}

std::string StringNode::toString() const {
    return "StringNode(value: \"" + value_ + "\")";
}

void StringNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

} // namespace ambar