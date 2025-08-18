#pragma once

#include "../ASTNode.hpp"
#include <vector>

namespace ambar {

class ContinueNode : public ASTNode {
public:
    ContinueNode();
    ~ContinueNode() override;

    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;

private:
    // Pode ser estendido no futuro se necessário
};

} // namespace ambar