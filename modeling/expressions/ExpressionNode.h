//
// Created by hlefebvr on 12/11/19.
//

#ifndef ED_SOLVER_EXPRESSIONNODE_H
#define ED_SOLVER_EXPRESSIONNODE_H

#include <string>
#include <utility>
#include "../../structures/BinaryTreeNode.h"
#include "../Variable.h"

class ExpressionNode : public BinaryTreeNode {
public:
    enum Type { Num, Var, Sum, Product, Cos, Sin, Tan, Acos, Asin, Atan, Cosh, Sinh, Tanh, Ln, Exp, Sqrt, Pow };
protected:
    Type _type = Num;
    float _numerical = 0;
    Variable* _variable = nullptr;
public:
    ExpressionNode() = default;
    ExpressionNode(const ExpressionNode& rhs);
    explicit ExpressionNode(Type type);
    explicit ExpressionNode(float numerical);
    explicit ExpressionNode(Variable& variable);
    ExpressionNode& operator=(const ExpressionNode& rhs);
    Variable& as_variable() const;
    float& as_numerical();
    float as_numerical() const;
    std::string to_string() const override;
    // unsigned long int id() const { return 10; }
    Type type() const { return _type; }
    void type(Type type) { _type = type; }

    friend bool is_type(const ExpressionNode& node, Type type);
    friend std::ostream& operator<<(std::ostream& os, const ExpressionNode& expr);
};

static bool is_variable(const ExpressionNode& node) { return is_type(node, ExpressionNode::Var); }
static bool is_numerical(const ExpressionNode &node) { return is_type(node, ExpressionNode::Num); }
static bool is_sum(const ExpressionNode &node) { return is_type(node, ExpressionNode::Sum); }
static bool is_product(const ExpressionNode &node) { return is_type(node, ExpressionNode::Product); }
static bool is_cos(const ExpressionNode& node) { return is_type(node, ExpressionNode::Cos); }
static bool is_sin(const ExpressionNode& node) { return is_type(node, ExpressionNode::Sin); }
static bool is_tan(const ExpressionNode& node) { return is_type(node, ExpressionNode::Tan); }
static bool is_acos(const ExpressionNode& node) { return is_type(node, ExpressionNode::Acos); }
static bool is_asin(const ExpressionNode& node) { return is_type(node, ExpressionNode::Asin); }
static bool is_atan(const ExpressionNode& node) { return is_type(node, ExpressionNode::Atan); }
static bool is_cosh(const ExpressionNode& node) { return is_type(node, ExpressionNode::Cosh); }
static bool is_sinh(const ExpressionNode& node) { return is_type(node, ExpressionNode::Sinh); }
static bool is_tanh(const ExpressionNode& node) { return is_type(node, ExpressionNode::Tanh); }
static bool is_ln(const ExpressionNode& node) { return is_type(node, ExpressionNode::Ln); }
static bool is_exp(const ExpressionNode& node) { return is_type(node, ExpressionNode::Exp); }
static bool is_sqrt(const ExpressionNode& node) { return is_type(node, ExpressionNode::Sqrt); }
static bool is_pow(const ExpressionNode& node) { return is_type(node, ExpressionNode::Pow); }
static bool is_binary_operator(const ExpressionNode& node) { return is_sum(node) || is_product(node) || is_pow(node); }
static bool is_unitary_operator(const ExpressionNode& node) { return !is_variable(node) && !is_numerical(node) && !is_binary_operator(node); }


static bool is_binary_operator(ExpressionNode::Type type) { return type == ExpressionNode::Sum || type == ExpressionNode::Product || type == ExpressionNode::Pow; }
static bool is_unitary_operator(ExpressionNode::Type type) { return type != ExpressionNode::Num && type != ExpressionNode::Var && !is_binary_operator(type); }


#endif //ED_SOLVER_EXPRESSIONNODE_H
