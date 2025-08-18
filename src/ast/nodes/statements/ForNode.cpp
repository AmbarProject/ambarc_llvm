#include "ForNode.hpp"
#include "../../visitors/ASTVisitor.hpp"

namespace ambar {

ForNode::ForNode(std::unique_ptr<ASTNode> init,
                 std::unique_ptr<ASTNode> cond,
                 std::unique_ptr<ASTNode> update,
                 std::unique_ptr<BlockNode> body)
    : ASTNode(ASTNode::NodeType::ForNode),
      init_(std::move(init)),
      cond_(std::move(cond)),
      update_(std::move(update)),
      body_(std::move(body)) {}

void ForNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string ForNode::toString() const {
    std::string initStr = init_ ? init_->toString() : "null";
    std::string condStr = cond_ ? cond_->toString() : "null";
    std::string updateStr = update_ ? update_->toString() : "null";
    
    return "ForNode(init=" + initStr + 
           ", cond=" + condStr + 
           ", update=" + updateStr + ")";
}

} // namespace ambar