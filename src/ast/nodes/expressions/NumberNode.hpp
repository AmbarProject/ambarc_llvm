#pragma once

#include "../ASTNode.hpp"
#include <string>
#include <cstdio>
#include <cassert>

namespace ambar {

class NumberNode : public ASTNode {
public:
    explicit NumberNode(int value, SourceLocation loc = {})
        : ASTNode(NodeType::NumberNode, loc), intValue_(value), isFloat_(false) {}

    explicit NumberNode(double value, SourceLocation loc = {})
        : ASTNode(NodeType::NumberNode, loc), floatValue_(value), isFloat_(true) {}

    // Métodos de acesso
    bool isFloatingPoint() const { return isFloat_; }
    
    void accept(ASTVisitor& visitor) override {} 

    int getIntValue() const { 
        assert(!isFloat_ && "Not an integer value");
        return intValue_; 
    }
    
    double getFloatValue() const { 
        assert(isFloat_ && "Not a float value");
        return floatValue_; 
    }

    // Retorna o valor como string formatada
    std::string getValueAsString() const {
        if (isFloat_) {
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%.6f", floatValue_);
            
            std::string s(buffer);
            s.erase(s.find_last_not_of('0') + 1, std::string::npos);
            if (s.back() == '.') s.push_back('0');
            return s;
        }
        return std::to_string(intValue_);
    }

    std::string toString() const override {
        return "Number(" + getValueAsString() + ")";
    }

private:
    union {
        int intValue_;
        double floatValue_;
    };
    bool isFloat_;
};

} // namespace ambar