#include "CallNode.hpp"
#include "../../visitors/ASTVisitor.hpp"

namespace ambar {

CallNode::CallNode(const std::string& funcName, std::vector<ASTNodePtr> args)
    : ASTNode(ASTNode::NodeType::CallExpr),
      funcName_(funcName),
      args_(std::move(args)) {}

const std::string& CallNode::getFunctionName() const {
    return funcName_;
}

const std::vector<ASTNodePtr>& CallNode::getArguments() const {
    return args_;
}

void CallNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string CallNode::toString() const {
    std::string argsStr;
    for (const auto& arg : args_) {
        if (!argsStr.empty()) argsStr += ", ";
        argsStr += arg->toString();
    }
    return "CallNode(func: " + funcName_ + ", args: [" + argsStr + "])";
}

} // namespace ambar