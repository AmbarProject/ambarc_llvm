#include "ContinueNode.hpp"
#include "../../visitors/ASTVisitor.hpp"

namespace ambar {

ContinueNode::ContinueNode()
    : ASTNode(ASTNode::NodeType::ContinueStmt) {}

ContinueNode::~ContinueNode() = default;

void ContinueNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string ContinueNode::toString() const {
    return "ContinueNode";
}

} // namespace ambar