#pragma once

#include "../ASTNode.hpp"
#include <vector>
#include <string>
#include <memory>

namespace ambar {

class CallNode : public ASTNode {
public:
    CallNode(const std::string& funcName, std::vector<ASTNodePtr> args, SourceLocation loc = {})
        : ASTNode(ASTNode::NodeType::CallNode, loc),
          funcName_(funcName),
          args_(std::move(args)) {}
    
    const std::string& getFunctionName() const { return funcName_; }
    const std::vector<ASTNodePtr>& getArguments() const { return args_; }
    
    std::string toString() const override {
        std::string argsStr;
        for (const auto& arg : args_) {
            if (!argsStr.empty()) argsStr += ", ";
            argsStr += arg->toString();
        }
        return "CallNode(func: " + funcName_ + ", args: [" + argsStr + "])";
    }

    void accept(ASTVisitor& visitor) override {} 


private:
    std::string funcName_;
    std::vector<ASTNodePtr> args_;
};

} // namespace ambar