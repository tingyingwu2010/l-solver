//
// Created by hlefebvr on 12/11/19.
//

#ifndef ED_SOLVER_EXPRESSION_H
#define ED_SOLVER_EXPRESSION_H

#include "../../structures/BinaryTree.h"
#include "ExpressionNode.h"

typedef BinaryTree<ExpressionNode> ExpressionTree;

class Expression {
    ExpressionTree _tree;
    friend class LinearExpression;
public:
    // Constructors
    Expression() = default;
    explicit Expression(float num);
    explicit Expression(Variable& variable);
    explicit Expression(const ExpressionNode& node);
    Expression(const Expression& rhs) = default;
    Expression(ExpressionNode::Type type, const Expression& node);
    Expression(ExpressionNode::Type type, Variable& node);
    Expression(ExpressionNode::Type type, float node);
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

static Expression pow(const Expression& node, const Expression& exponent) { throw Exception("Not implemented yet"); }
static Expression pow(const Expression& node, Variable& exponent) { throw Exception("Not implemented yet"); }
static Expression pow(const Expression& node, float exponent) { throw Exception("Not implemented yet"); }
static Expression pow(Variable& node, const Expression& exponent) { throw Exception("Not implemented yet"); }
static Expression pow(float node, const Expression& exponent) { throw Exception("Not implemented yet"); }
static Expression pow(Variable& node, Variable& exponent) { throw Exception("Not implemented yet"); }
static Expression pow(float node, Variable& exponent) { throw Exception("Not implemented yet"); }
static Expression pow(Variable& node, float exponent) { throw Exception("Not implemented yet"); }
static Expression pow(float node, float exponent) { throw Exception("Not implemented yet"); }

static Expression operator+(const Expression& a, Variable& b) { return a + Expression(b); }
static Expression operator+(const Expression& a, float b) { return a + Expression(b); }
static Expression operator+(Variable& a, const Expression& b) { return Expression(a) + b; }
static Expression operator+(Variable& a, Variable& b) { return Expression(a) + Expression(b); }
static Expression operator+(Variable& a, float b) { return Expression(a) + Expression(b); }
static Expression operator+(float a, const Expression& b) { return Expression(a) + b; }
static Expression operator+(float a, Variable& b) { return Expression(a) + Expression(b); }

static Expression operator*(const Expression& a, Variable& b) { return a * Expression(b); }
static Expression operator*(const Expression& a, float b) { return a * Expression(b); }
static Expression operator*(Variable& a, const Expression& b) { return Expression(a) * b; }
static Expression operator*(Variable& a, Variable& b) { return Expression(a) * Expression(b); }
static Expression operator*(Variable& a, float b) { return Expression(a) * b; }
static Expression operator*(float a, const Expression& b) { return Expression(a) * b; }
static Expression operator*(float a, Variable& b) { return Expression(a) * Expression(b); }

static Expression operator-(const Expression& a, const Expression& b) { return a + (-1) * b; }
static Expression operator-(const Expression& a, Variable& b) { return a + (-1) * b; }
static Expression operator-(const Expression& a, float b) { return a + ((-1) * b); }
static Expression operator-(Variable& a, const Expression& b) { return a + (-1) * b; }
static Expression operator-(Variable& a, Variable& b) { return a + (-1) * b; }
static Expression operator-(Variable& a, float b) { return a * (-1) * b; }
static Expression operator-(float a, const Expression& b) { return a * (-1) * b; }
static Expression operator-(float a, Variable& b) { return a * (-1) * b; }

static Expression operator/(const Expression& a, const Expression& b) { return a * pow(b, -1); }
static Expression operator/(const Expression& a, Variable& b) { return a * pow(b, -1); }
static Expression operator/(const Expression& a, float b) { return a * pow(b, -1); }
static Expression operator/(Variable& a, const Expression& b) { return a * pow(b, -1); }
static Expression operator/(Variable& a, Variable& b) { return a * pow(b, -1); }
static Expression operator/(Variable& a, float b) { return a * pow(b, -1); }
static Expression operator/(float a, const Expression& b) { return a * pow(b, -1); }
static Expression operator/(float a, Variable& b) { return a * pow(b, -1); }

// Functions dedicated to expressions creation
static Expression cos(const Expression& node) { return Expression(ExpressionNode::Cos, node); }
static Expression sin(const Expression& node) { return Expression(ExpressionNode::Sin, node); }
static Expression tan(const Expression& node) { return Expression(ExpressionNode::Tan, node); }
static Expression acos(const Expression& node) { return Expression(ExpressionNode::Acos, node); }
static Expression asin(const Expression& node) { return Expression(ExpressionNode::Asin, node); }
static Expression atan(const Expression& node) { return Expression(ExpressionNode::Atan, node); }
static Expression cosh(const Expression& node) { return Expression(ExpressionNode::Cosh, node); }
static Expression sinh(const Expression& node) { return Expression(ExpressionNode::Sinh, node); }
static Expression tanh(const Expression& node) { return Expression(ExpressionNode::Tanh, node); }
static Expression ln(const Expression& node) { return Expression(ExpressionNode::Ln, node); }
static Expression exp(const Expression& node) { return Expression(ExpressionNode::Exp, node); }
static Expression sqrt(const Expression& node) { return Expression(ExpressionNode::Sqrt, node); }
static Expression cos(Variable& node) { return Expression(ExpressionNode::Cos, node); }
static Expression sin(Variable& node) { return Expression(ExpressionNode::Sin, node); }
static Expression tan(Variable& node) { return Expression(ExpressionNode::Tan, node); }
static Expression acos(Variable& node) { return Expression(ExpressionNode::Acos, node); }
static Expression asin(Variable& node) { return Expression(ExpressionNode::Asin, node); }
static Expression atan(Variable& node) { return Expression(ExpressionNode::Atan, node); }
static Expression cosh(Variable& node) { return Expression(ExpressionNode::Cosh, node); }
static Expression sinh(Variable& node) { return Expression(ExpressionNode::Sinh, node); }
static Expression tanh(Variable& node) { return Expression(ExpressionNode::Tanh, node); }
static Expression ln(Variable& node) { return Expression(ExpressionNode::Ln, node); }
static Expression exp(Variable& node) { return Expression(ExpressionNode::Exp, node); }
static Expression sqrt(Variable& node) { return Expression(ExpressionNode::Sqrt, node); }
static Expression cos(float node) { return Expression(ExpressionNode::Cos, node); }
static Expression sin(float node) { return Expression(ExpressionNode::Sin, node); }
static Expression tan(float node) { return Expression(ExpressionNode::Tan, node); }
static Expression acos(float node) { return Expression(ExpressionNode::Acos, node); }
static Expression asin(float node) { return Expression(ExpressionNode::Asin, node); }
static Expression atan(float node) { return Expression(ExpressionNode::Atan, node); }
static Expression cosh(float node) { return Expression(ExpressionNode::Cosh, node); }
static Expression sinh(float node) { return Expression(ExpressionNode::Sinh, node); }
static Expression tanh(float node) { return Expression(ExpressionNode::Tanh, node); }
static Expression ln(float node) { return Expression(ExpressionNode::Ln, node); }
static Expression exp(float node) { return Expression(ExpressionNode::Exp, node); }
static Expression sqrt(float node) { return Expression(ExpressionNode::Sqrt, node); }


#endif //ED_SOLVER_EXPRESSION_H
