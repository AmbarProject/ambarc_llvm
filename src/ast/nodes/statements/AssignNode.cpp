#include "AssignNode.hpp"
#include "../../visitors/ASTVisitor.hpp"

namespace ambar {

AssignNode::AssignNode(const std::string& identifier, 
                       std::unique_ptr<ASTNode> valueExpr,
                       SourceLocation loc)
    : ASTNode(ASTNode::NodeType::AssignStmt, loc),
      identifier_(identifier),
      valueExpr_(std::move(valueExpr)) {}

AssignNode::~AssignNode() = default;

const std::string& AssignNode::getIdentifier() const {
    return identifier_;
}

ASTNode* AssignNode::getValueExpr() const {
    return valueExpr_.get();
}

void AssignNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string AssignNode::toString() const {
    return "AssignNode(to: " + identifier_ + ")";
}

} // namespace ambar