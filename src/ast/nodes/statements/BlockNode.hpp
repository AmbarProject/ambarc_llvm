#pragma once

#include "../ASTNode.hpp"
#include "../declarations/VarNode.hpp"
#include <vector>
#include <memory>

namespace ambar {

class BlockNode : public ASTNode {
public:
    explicit BlockNode(SourceLocation loc = {});
    
    // Adiciona um nó ao bloco (gerencia automaticamente declarações vs statements)
    void add(std::unique_ptr<ASTNode> node);
    
    // Adiciona diretamente uma declaração de variável local
    void addLocalDeclaration(std::unique_ptr<VarNode> decl);
    
    // Getters
    const std::vector<std::unique_ptr<VarNode>>& getLocalDeclarations() const;
    const std::vector<std::unique_ptr<ASTNode>>& getStatements() const;
    
    // Visitor pattern
    void accept(ASTVisitor& visitor) override;
    
    // Representação em string
    std::string toString() const override;

private:
    std::vector<std::unique_ptr<VarNode>> local_declarations_;
    std::vector<std::unique_ptr<ASTNode>> statements_;
};

} // namespace ambar