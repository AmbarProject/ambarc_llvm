#include "BinaryNode.hpp"
#include "../../visitors/ASTVisitor.hpp"

namespace ambar {

BinaryNode::BinaryNode(std::string op, 
                       ASTNodePtr left, 
                       ASTNodePtr right,
                       SourceLocation loc)
    : ASTNode(NodeType::BinaryExpr, std::move(loc)),
      op_(std::move(op)),
      left_(std::move(left)),
      right_(std::move(right)) {}

void BinaryNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string BinaryNode::toString() const {
    return "BinaryNode(op: " + op_ + 
           ", left: " + left_->toString() + 
           ", right: " + right_->toString() + ")";
}

} // namespace ambar