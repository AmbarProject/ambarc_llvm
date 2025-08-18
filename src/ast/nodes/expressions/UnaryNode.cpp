#include "UnaryNode.hpp"
#include "../../visitors/ASTVisitor.hpp"

namespace ambar {

void UnaryNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

} // namespace ambar