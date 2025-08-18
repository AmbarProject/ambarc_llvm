#include "IfNode.hpp"
#include "../../visitors/ASTVisitor.hpp"

namespace ambar {

IfNode::IfNode(std::unique_ptr<ASTNode> cond, 
               std::unique_ptr<ASTNode> thenBlock,
               std::unique_ptr<ASTNode> elseBlock)
    : ASTNode(NodeType::IfStmt),
      cond_(std::move(cond)),
      thenBlock_(std::move(thenBlock)),
      elseBlock_(std::move(elseBlock)) {}

void IfNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string IfNode::toString() const {
    return "IfNode";
}

} // namespace ambar