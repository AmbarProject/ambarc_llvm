#include "WhileNode.hpp"
#include "../../visitors/ASTVisitor.hpp"

namespace ambar {

WhileNode::WhileNode(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> body)
    : ASTNode(ASTNode::NodeType::WhileStmt),
      condition_(std::move(condition)),
      body_(std::move(body)) {}

void WhileNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string WhileNode::toString() const {
    return "WhileNode";
}

ASTNode* WhileNode::getCondition() const {
    return condition_.get();
}

ASTNode* WhileNode::getBody() const {
    return body_.get();
}

} // namespace ambar