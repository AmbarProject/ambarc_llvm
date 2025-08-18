#pragma once

#include "../ASTNode.hpp"
#include <vector>
#include <memory>

namespace ambar {

class ProgramNode : public ASTNode {
public:
    explicit ProgramNode(SourceLocation loc = {})
        : ASTNode(NodeType::ProgramNode, loc) {}
    
    void addDeclaration(std::unique_ptr<ASTNode> decl) {
        declarations_.push_back(std::move(decl));
    }

    const std::vector<std::unique_ptr<ASTNode>>& getDeclarations() const {
        return declarations_;
    }

    void accept(ASTVisitor& visitor) override {} 

    // Implementação obrigatória do método puro
    std::string toString() const override {
        std::string result = "ProgramNode (";
        result += std::to_string(declarations_.size()) + " declarations):\n";
        for (const auto& decl : declarations_) {
            result += "  " + decl->toString() + "\n";
        }
        return result;
    }

    ~ProgramNode() override = default;

private:
    std::vector<std::unique_ptr<ASTNode>> declarations_;
};

} // namespace ambar