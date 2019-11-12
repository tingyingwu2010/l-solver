//
// Created by hlefebvr on 12/11/19.
//

#ifndef ED_SOLVER_EXPRESSIONNODE_H
#define ED_SOLVER_EXPRESSIONNODE_H

#include <string>
#include <utility>
#include "../../structures/BinaryTreeNode.h"
#include "../Variable.h"

namespace L {
    class ExpressionNode;
    enum Type { Num, Var, Sum, Product, Cos, Sin, Tan, Acos, Asin, Atan, Cosh, Sinh, Tanh, Ln, Exp, Sqrt, Pow };
    bool is_type(const ExpressionNode& node, Type type);
    std::ostream& operator<<(std::ostream& os, const ExpressionNode& expr);
}

class L::ExpressionNode : public BinaryTreeNode {
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

static bool is_variable(const L::ExpressionNode& node) { return is_type(node, L::Var); }
static bool is_numerical(const L::ExpressionNode &node) { return is_type(node, L::Num); }
static bool is_sum(const L::ExpressionNode &node) { return is_type(node, L::Sum); }
static bool is_product(const L::ExpressionNode &node) { return is_type(node, L::Product); }
static bool is_cos(const L::ExpressionNode& node) { return is_type(node, L::Cos); }
static bool is_sin(const L::ExpressionNode& node) { return is_type(node, L::Sin); }
static bool is_tan(const L::ExpressionNode& node) { return is_type(node, L::Tan); }
static bool is_acos(const L::ExpressionNode& node) { return is_type(node, L::Acos); }
static bool is_asin(const L::ExpressionNode& node) { return is_type(node, L::Asin); }
static bool is_atan(const L::ExpressionNode& node) { return is_type(node, L::Atan); }
static bool is_cosh(const L::ExpressionNode& node) { return is_type(node, L::Cosh); }
static bool is_sinh(const L::ExpressionNode& node) { return is_type(node, L::Sinh); }
static bool is_tanh(const L::ExpressionNode& node) { return is_type(node, L::Tanh); }
static bool is_ln(const L::ExpressionNode& node) { return is_type(node, L::Ln); }
static bool is_exp(const L::ExpressionNode& node) { return is_type(node, L::Exp); }
static bool is_sqrt(const L::ExpressionNode& node) { return is_type(node, L::Sqrt); }
static bool is_pow(const L::ExpressionNode& node) { return is_type(node, L::Pow); }
static bool is_binary_operator(const L::ExpressionNode& node) { return is_sum(node) || is_product(node) || is_pow(node); }
static bool is_unitary_operator(const L::ExpressionNode& node) { return !is_variable(node) && !is_numerical(node) && !is_binary_operator(node); }


static bool is_binary_operator(L::Type type) { return type == L::Sum || type == L::Product || type == L::Pow; }
static bool is_unitary_operator(L::Type type) { return type != L::Num && type != L::Var && !is_binary_operator(type); }


#endif //ED_SOLVER_EXPRESSIONNODE_H
