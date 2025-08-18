#include "BreakNode.hpp"
#include "../../visitors/ASTVisitor.hpp"

namespace ambar {

void BreakNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

} // namespace ambar