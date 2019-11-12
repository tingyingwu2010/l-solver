//
// Created by hlefebvr on 12/11/19.
//

#ifndef ED_SOLVER_EXPRESSION_H
#define ED_SOLVER_EXPRESSION_H

#include "../../structures/BinaryTree.h"
#include "ExpressionNode.h"

namespace L {

    typedef BinaryTree<ExpressionNode> ExpressionTree;
    class Expression;

    Expression operator+(const Expression& a, const Expression& b);
    Expression operator*(const Expression& a, const Expression& b);
}

class L::Expression {
    ExpressionTree _tree;
    friend class LinearExpression;
public:
    // Constructors
    Expression() = default;
    explicit Expression(float num);
    explicit Expression(Variable& variable);
    explicit Expression(const ExpressionNode& node);
    Expression(const Expression& rhs) = default;
    Expression(L::Type type, const Expression& node);
    Expression(L::Type type, Variable& node);
    Expression(L::Type type, float node);
    Expression& operator=(const Expression& rhs);
    Expression& operator=(Variable& rhs); // TODO, we can use const if env is used
    Expression& operator=(float rhs);

    // Math operations
    Expression& operator+=(const Expression& term);
    Expression& operator*=(const Expression& term);

    Expression& operator-=(const Expression& term);
    Expression& operator/=(const Expression& term);
    Expression& operator+=(const Variable& term);
    Expression& operator-=(const Variable& term);
    Expression& operator*=(const Variable& term);
    Expression& operator/=(const Variable& term);
    Expression& operator+=(float term);
    Expression& operator-=(float term);
    Expression& operator*=(float term);
    Expression& operator/=(float term);

    // Tree manipulation
    // void normalize();
    void expand(unsigned long int depth = 0);
    static float evaluate(const ExpressionTree& tree, const std::function<bool(const Variable&)>& indicator = [](const Variable&){ return true; });
    float evaluate(const std::function<bool(const Variable&)>& indicator = [](const Variable&){ return true; }) const;
    void linear_factorize(const std::function<bool(const Variable&)>& indicator  = [](const Variable&){ return true; }); //!< expects an expanded normal form

    // Display and output-related functions
    void export_to_dot(const std::string& filename = "expression_tree", bool with_system_command = false) const;

    // Operators
    friend Expression operator+(const Expression& a, const Expression& b);
    friend Expression operator*(const Expression& a, const Expression& b);
};

namespace L {
    static L::Expression pow(const L::Expression &node, const L::Expression &exponent) {
        throw L::Exception("Not implemented yet");
    }

    static L::Expression pow(const L::Expression &node, L::Variable &exponent) {
        throw L::Exception("Not implemented yet");
    }

    static L::Expression pow(const L::Expression &node, float exponent) { throw L::Exception("Not implemented yet"); }

    static L::Expression pow(L::Variable &node, const L::Expression &exponent) {
        throw L::Exception("Not implemented yet");
    }

    static L::Expression pow(float node, const L::Expression &exponent) { throw L::Exception("Not implemented yet"); }

    static L::Expression pow(L::Variable &node, L::Variable &exponent) { throw L::Exception("Not implemented yet"); }

    static L::Expression pow(float node, L::Variable &exponent) { throw L::Exception("Not implemented yet"); }

    static L::Expression pow(L::Variable &node, float exponent) { throw L::Exception("Not implemented yet"); }

    static L::Expression pow(float node, float exponent) { throw L::Exception("Not implemented yet"); }

    static L::Expression operator+(const L::Expression &a, L::Variable &b) { return a + L::Expression(b); }

    static L::Expression operator+(const L::Expression &a, float b) { return a + L::Expression(b); }

    static L::Expression operator+(L::Variable &a, const L::Expression &b) { return L::Expression(a) + b; }

    static L::Expression operator+(L::Variable &a, L::Variable &b) { return L::Expression(a) + L::Expression(b); }

    static L::Expression operator+(L::Variable &a, float b) { return L::Expression(a) + L::Expression(b); }

    static L::Expression operator+(float a, const L::Expression &b) { return L::Expression(a) + b; }

    static L::Expression operator+(float a, L::Variable &b) { return L::Expression(a) + L::Expression(b); }

    static L::Expression operator*(const L::Expression &a, L::Variable &b) { return a * L::Expression(b); }

    static L::Expression operator*(const L::Expression &a, float b) { return a * L::Expression(b); }

    static L::Expression operator*(L::Variable &a, const L::Expression &b) { return L::Expression(a) * b; }

    static L::Expression operator*(L::Variable &a, L::Variable &b) { return L::Expression(a) * L::Expression(b); }

    static L::Expression operator*(L::Variable &a, float b) { return L::Expression(a) * b; }

    static L::Expression operator*(float a, const L::Expression &b) { return L::Expression(a) * b; }

    static L::Expression operator*(float a, L::Variable &b) { return L::Expression(a) * L::Expression(b); }

    static L::Expression operator-(const L::Expression &a, const L::Expression &b) { return a + (-1) * b; }

    static L::Expression operator-(const L::Expression &a, L::Variable &b) { return a + (-1) * b; }

    static L::Expression operator-(const L::Expression &a, float b) { return a + ((-1) * b); }

    static L::Expression operator-(L::Variable &a, const L::Expression &b) { return a + (-1) * b; }

    static L::Expression operator-(L::Variable &a, L::Variable &b) { return a + (-1) * b; }

    static L::Expression operator-(L::Variable &a, float b) { return a * (-1) * b; }

    static L::Expression operator-(float a, const L::Expression &b) { return a * (-1) * b; }

    static L::Expression operator-(float a, L::Variable &b) { return a * (-1) * b; }

    static L::Expression operator/(const L::Expression &a, const L::Expression &b) { return a * pow(b, -1); }

    static L::Expression operator/(const L::Expression &a, L::Variable &b) { return a * pow(b, -1); }

    static L::Expression operator/(const L::Expression &a, float b) { return a * pow(b, -1); }

    static L::Expression operator/(L::Variable &a, const L::Expression &b) { return a * pow(b, -1); }

    static L::Expression operator/(L::Variable &a, L::Variable &b) { return a * pow(b, -1); }

    static L::Expression operator/(L::Variable &a, float b) { return a * pow(b, -1); }

    static L::Expression operator/(float a, const L::Expression &b) { return a * pow(b, -1); }

    static L::Expression operator/(float a, L::Variable &b) { return a * pow(b, -1); }

// Functions dedicated to L::Expressions creation
    static L::Expression cos(const L::Expression &node) { return L::Expression(L::Cos, node); }

    static L::Expression sin(const L::Expression &node) { return L::Expression(L::Sin, node); }

    static L::Expression tan(const L::Expression &node) { return L::Expression(L::Tan, node); }

    static L::Expression acos(const L::Expression &node) { return L::Expression(L::Acos, node); }

    static L::Expression asin(const L::Expression &node) { return L::Expression(L::Asin, node); }

    static L::Expression atan(const L::Expression &node) { return L::Expression(L::Atan, node); }

    static L::Expression cosh(const L::Expression &node) { return L::Expression(L::Cosh, node); }

    static L::Expression sinh(const L::Expression &node) { return L::Expression(L::Sinh, node); }

    static L::Expression tanh(const L::Expression &node) { return L::Expression(L::Tanh, node); }

    static L::Expression ln(const L::Expression &node) { return L::Expression(L::Ln, node); }

    static L::Expression exp(const L::Expression &node) { return L::Expression(L::Exp, node); }

    static L::Expression sqrt(const L::Expression &node) { return L::Expression(L::Sqrt, node); }

    static L::Expression cos(L::Variable &node) { return L::Expression(L::Cos, node); }

    static L::Expression sin(L::Variable &node) { return L::Expression(L::Sin, node); }

    static L::Expression tan(L::Variable &node) { return L::Expression(L::Tan, node); }

    static L::Expression acos(L::Variable &node) { return L::Expression(L::Acos, node); }

    static L::Expression asin(L::Variable &node) { return L::Expression(L::Asin, node); }

    static L::Expression atan(L::Variable &node) { return L::Expression(L::Atan, node); }

    static L::Expression cosh(L::Variable &node) { return L::Expression(L::Cosh, node); }

    static L::Expression sinh(L::Variable &node) { return L::Expression(L::Sinh, node); }

    static L::Expression tanh(L::Variable &node) { return L::Expression(L::Tanh, node); }

    static L::Expression ln(L::Variable &node) { return L::Expression(L::Ln, node); }

    static L::Expression exp(L::Variable &node) { return L::Expression(L::Exp, node); }

    static L::Expression sqrt(L::Variable &node) { return L::Expression(L::Sqrt, node); }

    static L::Expression cos(float node) { return L::Expression(L::Cos, node); }

    static L::Expression sin(float node) { return L::Expression(L::Sin, node); }

    static L::Expression tan(float node) { return L::Expression(L::Tan, node); }

    static L::Expression acos(float node) { return L::Expression(L::Acos, node); }

    static L::Expression asin(float node) { return L::Expression(L::Asin, node); }

    static L::Expression atan(float node) { return L::Expression(L::Atan, node); }

    static L::Expression cosh(float node) { return L::Expression(L::Cosh, node); }

    static L::Expression sinh(float node) { return L::Expression(L::Sinh, node); }

    static L::Expression tanh(float node) { return L::Expression(L::Tanh, node); }

    static L::Expression ln(float node) { return L::Expression(L::Ln, node); }

    static L::Expression exp(float node) { return L::Expression(L::Exp, node); }

    static L::Expression sqrt(float node) { return L::Expression(L::Sqrt, node); }
}

#endif //ED_SOLVER_EXPRESSION_H
