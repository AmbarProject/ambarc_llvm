#pragma once

#include <vector>
#include <memory>
#include <string>
#include "../../SourceLocation.hpp"

namespace ambar {

// Forward declaration for the visitor pattern
class ASTVisitor;

/**
 * @brief Base class for all nodes in the Abstract Syntax Tree (AST)
 * 
 * This class defines the common interface for all AST nodes and implements
 * the Visitor pattern for AST traversal and transformations.
 */
class ASTNode {
public:
    enum class NodeType {
        ProgramNode,
        VarNode,
        FunctionNode,
        BinaryNode,
        NumberNode,
        StringNode,
        BoolNode,
        IdentifierNode,
        AssignNode,
        BlockNode,
        IfNode,
        WhileNode,
        ForNode,
        ReturnNode,
        CallNode,
        BreakNode,
        ContinueNode,
        UnaryNode
    };

    explicit ASTNode(NodeType type, SourceLocation loc = {})
        : nodeType_(type), location_(loc) {}

    virtual ~ASTNode() = default;

    /**
     * @brief Get the type of this node
     * @return NodeType The node type
     */
    NodeType getNodeType() const { return nodeType_; }

    /**
     * @brief Get the source location where this node originated
     * @return SourceLocation The source location
     */
    const SourceLocation& getLocation() const { return location_; }

    /**
     * @brief Accept a visitor for this node (Visitor pattern)
     * @param visitor The visitor to accept
     */
    virtual void accept(ASTVisitor& visitor) = 0;

    /**
     * @brief Convert the node to a string representation (for debugging)
     * @return std::string String representation of the node
     */
    virtual std::string toString() const = 0;

private:
    NodeType nodeType_;
    SourceLocation location_;
};

// Helper type aliases for AST node pointers
using ASTNodePtr = std::unique_ptr<ASTNode>;
using ASTNodeList = std::vector<ASTNodePtr>;

} // namespace ambar