//
// Created by hlefebvr on 12/11/19.
//

#include "ExpressionNode.h"
#include "../../utils/Exception.h"

using namespace L;

Variable &ExpressionNode::as_variable() const {
    if (!is_variable(*this)) throw Exception("Cannot return variable for non-variable nodes");
    return *_variable;
}

float ExpressionNode::as_numerical() const {
    if (!is_numerical(*this)) throw Exception("Cannot return numerical for non-numerical nodes");
    return _numerical;
}

float &ExpressionNode::as_numerical() {
    if (!is_numerical(*this)) throw Exception("Cannot return numerical for non-numerical nodes");
    return _numerical;
}

std::ostream& L::operator<<(std::ostream& os, const ExpressionNode& expr) {
    return (os << expr.to_string());
}

std::string ExpressionNode::to_string() const {
    switch (_type) {
        case Num: return std::to_string(_numerical);
        case Var: return _variable->user_defined_name();
        case Sum: return "+";
        case Product: return "*";
        case Cos: return "cos";
        case Sin: return "sin";
        case Tan: return "tan";
        case Acos: return "acos";
        case Asin: return "asin";
        case Atan: return "atan";
        case Cosh: return "cosh";
        case Sinh: return "sinh";
        case Tanh: return "tanh";
        case Ln: return "ln";
        case Exp: return "exp";
        case Sqrt: return "sqrt";
        default: throw Exception("Unknown type for expression node. This is probably due to a memory leak.");
    }
}

ExpressionNode::ExpressionNode(L::Type type) : _type(type) { }
ExpressionNode::ExpressionNode(float numerical) : _numerical(numerical), _type(Num) {}
ExpressionNode::ExpressionNode(class Variable& variable) : _variable(&variable), _type(Var) {}
ExpressionNode::ExpressionNode(const ExpressionNode &rhs) : _type(rhs._type), _numerical(rhs._numerical), _variable(rhs._variable) {}

ExpressionNode &ExpressionNode::operator=(const ExpressionNode &rhs) {
    _type = rhs._type;
    _numerical = rhs._numerical;
    _variable = rhs._variable;
    return *this;
}

bool L::is_type(const ExpressionNode& node, L::Type type) { return node._type == type; }
