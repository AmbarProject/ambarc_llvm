#pragma once

#include "../ASTNode.hpp"
#include <string>
#include <memory>

namespace ambar {

class VarNode : public ASTNode {
public:
    VarNode(const std::string& name, 
            const std::string& type, 
            std::unique_ptr<ASTNode> init = nullptr,
            SourceLocation loc = {})
        : ASTNode(NodeType::VarNode, loc),
          name_(name),
          type_(type),
          init_(std::move(init)) {}

    void accept(ASTVisitor& visitor) override {} 
    const std::string& getName() const { return name_; }
    const std::string& getType() const { return type_; }
    const std::unique_ptr<ASTNode>& getInit() const { return init_; }
    bool hasInit() const { return init_ != nullptr; }

    std::string toString() const override {
        return name_ + ": " + type_ + (hasInit() ? " = [init]" : "");
    }

private:
    std::string name_;
    std::string type_;
    std::unique_ptr<ASTNode> init_;
};

} // namespace ambar