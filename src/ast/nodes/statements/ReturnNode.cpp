#include "ReturnNode.hpp"
#include "../../visitors/ASTVisitor.hpp"

namespace ambar {

ReturnNode::ReturnNode(ASTNodePtr expr)
    : ASTNode(NodeType::ReturnStmt), expr_(std::move(expr)) {}

std::string ReturnNode::toString() const {
    return "ReturnNode" + (expr_ ? " with expression" : " (void)");
}

void ReturnNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

} // namespace ambar