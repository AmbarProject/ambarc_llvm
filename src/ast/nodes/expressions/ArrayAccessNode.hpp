#ifndef ARRAY_ACCESS_NODE_HPP
#define ARRAY_ACCESS_NODE_HPP

#include "../ASTNode.hpp"
#include <memory>
#include <string>

namespace ambar {

class ArrayAccessNode : public ASTNode {
public:
    ArrayAccessNode(const std::string& arrayName, std::unique_ptr<ASTNode> index, SourceLocation loc = {})
        : ASTNode(ASTNode::NodeType::ArrayAccessNode, loc),
          arrayName_(arrayName), index_(std::move(index)) {}

    const std::string& getArrayName() const { return arrayName_; }
    ASTNode* getIndex() const { return index_.get(); }

    void accept(ASTVisitor& visitor) override {} 

    std::string toString() const override {
        return "ArrayAccessNode(array: " + arrayName_ + ")";
    }

private:
    std::string arrayName_;
    std::unique_ptr<ASTNode> index_;
};

} // namespace ambar

#endif // ARRAY_ACCESS_NODE_HPP
