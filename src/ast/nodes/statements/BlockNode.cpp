#include "BlockNode.hpp"
#include "../../visitors/ASTVisitor.hpp"

namespace ambar {

BlockNode::BlockNode(SourceLocation loc)
    : ASTNode(ASTNode::NodeType::Block, loc) {}

void BlockNode::add(std::unique_ptr<ASTNode> node) {
    if (auto* decl = dynamic_cast<VarDeclNode*>(node.get())) {
        // Transfere a posse para local_declarations_
        local_declarations_.emplace_back(static_cast<VarDeclNode*>(node.release()));
    } else {
        // Mantém como statement geral
        statements_.push_back(std::move(node));
    }
}

void BlockNode::addLocalDeclaration(std::unique_ptr<VarDeclNode> decl) {
    local_declarations_.push_back(std::move(decl));
}

const std::vector<std::unique_ptr<VarDeclNode>>& BlockNode::getLocalDeclarations() const {
    return local_declarations_;
}

const std::vector<std::unique_ptr<ASTNode>>& BlockNode::getStatements() const {
    return statements_;
}

void BlockNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string BlockNode::toString() const {
    return "BlockNode [decls=" + std::to_string(local_declarations_.size()) + 
           ", stmts=" + std::to_string(statements_.size()) + "]";
}

} // namespace ambar