#pragma once

#include "../ASTNode.hpp"
#include <vector>
#include <string>
#include <memory>

namespace ambar {

class CallNode : public ASTNode {
public:
    CallNode(const std::string& funcName, std::vector<ASTNodePtr> args);
    
    const std::string& getFunctionName() const;
    const std::vector<ASTNodePtr>& getArguments() const;
    
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;

private:
    std::string funcName_;
    std::vector<ASTNodePtr> args_;
};

} // namespace ambar