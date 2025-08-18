#pragma once

#include <string>
#include "../ASTNode.hpp"

namespace ambar {

class StringNode : public ASTNode {
public:
    explicit StringNode(const std::string& value);
    
    const std::string& getValue() const;
    
    std::string toString() const override;
    void accept(ASTVisitor& visitor) override;

private:
    std::string value_;
};

} // namespace ambar