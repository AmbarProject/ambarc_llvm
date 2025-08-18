// FunctionNode.hpp
#pragma once

#include "../ASTNode.hpp"
#include <vector>
#include <string>
#include <utility>
#include <memory>

namespace ambar {

/**
 * @brief Node for function declarations in the AST
 * 
 * Represents a function declaration with its name, return type,
 * parameters, and body.
 */
class FunctionNode : public ASTNode {
public:
    using ParamList = std::vector<std::pair<std::string, std::string>>;
    
    FunctionNode(std::string name,
               std::string returnType,
               ParamList params,
               ASTNodePtr body,  // Alterado para ASTNodePtr
               SourceLocation loc = {})
        : ASTNode(NodeType::FunctionNode, loc),
          name_(std::move(name)),
          returnType_(std::move(returnType)),
          params_(std::move(params)),
          body_(std::move(body)) {}
    
    ~FunctionNode() override = default;

    // Getters
    const std::string& getName() const { return name_; }
    const std::string& getReturnType() const { return returnType_; }
    const ParamList& getParams() const { return params_; }
    const ASTNode* getBody() const { return body_.get(); }  // Alterado para ASTNode
    ASTNode* getBody() { return body_.get(); }             // Alterado para ASTNode
    void accept(ASTVisitor& visitor) override {} 

    std::string toString() const override {
        std::string paramsStr;
        for (const auto& param : params_) {
            if (!paramsStr.empty()) paramsStr += ", ";
            paramsStr += param.second + ": " + param.first;
        }
        return "FunctionNode: " + name_ + "(" + paramsStr + ") -> " + returnType_;
    }

private:
    std::string name_;
    std::string returnType_;
    ParamList params_;
    ASTNodePtr body_;  // Alterado para ASTNodePtr
};

} // namespace ambar