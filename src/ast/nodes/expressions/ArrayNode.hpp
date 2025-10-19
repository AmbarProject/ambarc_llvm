#ifndef ARRAY_NODE_HPP
#define ARRAY_NODE_HPP

#include "../ASTNode.hpp"
#include <vector>
#include <memory>
#include <string>

namespace ambar {

class ArrayNode : public ASTNode {
public:
    // Construtor que aceita vector de ASTNode* (do parser)
    ArrayNode(std::vector<ASTNode*>& elements, const std::string& elementType, SourceLocation loc = {})
        : ASTNode(ASTNode::NodeType::ArrayNode, loc),
          elementType_(elementType) {
        // Transferir ownership dos elementos
        for (auto* elem : elements) {
            if (elem) {
                elements_.push_back(std::unique_ptr<ASTNode>(elem));
            }
        }
    }

    // Construtor para array vazio
    ArrayNode(const std::string& elementType, SourceLocation loc = {})
        : ASTNode(ASTNode::NodeType::ArrayNode, loc),
          elementType_(elementType) {}

    // Getters
    const std::vector<std::unique_ptr<ASTNode>>& getElements() const { return elements_; }
    const std::string& getElementType() const { return elementType_; }
    int getSize() const { return elements_.size(); }
    bool isInitialized() const { return !elements_.empty(); }

    void accept(ASTVisitor& visitor) override {} 

    std::string toString() const override {
        return "ArrayNode(type: " + elementType_ + ", size: " + std::to_string(elements_.size()) + ")";
    }

private:
    std::vector<std::unique_ptr<ASTNode>> elements_;
    std::string elementType_;
};

} // namespace ambar

#endif // ARRAY_NODE_HPP
