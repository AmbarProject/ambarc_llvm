#pragma once

#include "../ASTNode.hpp"
#include "../declarations/VarNode.hpp"
#include <vector>
#include <memory>

namespace ambar {

class BlockNode : public ASTNode {
public:
    explicit BlockNode(SourceLocation loc = {})
        : ASTNode(ASTNode::NodeType::BlockNode, loc) {}
    
    // Adiciona um nó ao bloco (gerencia automaticamente declarações vs statements)
    void add(std::unique_ptr<ASTNode> node) {
        if (auto* decl = dynamic_cast<VarNode*>(node.get())) {
            // Transfere a posse para local_declarations_
            local_declarations_.emplace_back(static_cast<VarNode*>(node.release()));
        } else {
            // Mantém como statement geral
            statements_.push_back(std::move(node));
        }
    }
    
    // Adiciona diretamente uma declaração de variável local
    void addLocalDeclaration(std::unique_ptr<VarNode> decl) {
        local_declarations_.push_back(std::move(decl));
    }
    
    // Getters
    const std::vector<std::unique_ptr<VarNode>>& getLocalDeclarations() const {
        return local_declarations_;
    }
    
    const std::vector<std::unique_ptr<ASTNode>>& getStatements() const {
        return statements_;
    }
    
    // Visitor pattern
    void accept(ASTVisitor& visitor) override {}
    
    // Representação em string
    std::string toString() const override {
        return "BlockNode [decls=" + std::to_string(local_declarations_.size()) + 
               ", stmts=" + std::to_string(statements_.size()) + "]";
    }

private:
    std::vector<std::unique_ptr<VarNode>> local_declarations_;
    std::vector<std::unique_ptr<ASTNode>> statements_;
};

} // namespace ambar