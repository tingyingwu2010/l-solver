//
// Created by hlefebvr on 13/11/19.
//

#ifndef ED_SOLVER_EXPRESSION_H
#define ED_SOLVER_EXPRESSION_H

#include <vector>
#include <string>
#include <map>
#include <functional>
#include "LSolver/utils/Exception.h"

namespace L {
    enum ExpressionType { Num, Var, Prod, Sum, Sqrt, Exp, Pow, Ln }; // cos, sin, ...
    enum ExpressionSide { Left, Right };
    enum ExpressionTraversalOrder { PostOrder, PreOrder, InOrder };
    class ConstVariable;
    class Variable;
    class CoreVariable;
    class Expression;

    // friend functions
    void transfer(ExpressionSide from_side, Expression& from_expr, ExpressionSide to_side, Expression& to_expr);
    void swap(ExpressionSide from_side, Expression& from_expr, ExpressionSide to_side, Expression& to_expr);

    // operators
    Expression operator+(const Expression& a, const Expression& b);
    Expression operator*(const Expression& a, const Expression& b);
    Expression operator/(const Expression& a, const Expression& b);
    Expression operator-(const Expression& a, const Expression& b);

    std::ostream& operator<<(std::ostream& os, ExpressionType type);
    std::ostream& operator<<(std::ostream& os, ExpressionSide type);
    std::ostream& operator<<(std::ostream& os, ExpressionTraversalOrder type);
    std::ostream& operator<<(std::ostream& os, const Expression& expr);
}

class L::Expression {
    static unsigned long int _expression_id;

    ExpressionType _type = Num;
    Expression* _right = nullptr;
    Expression* _left = nullptr;
    float _numerical = 0;
    CoreVariable* _variable = nullptr;
    unsigned long int _id = _expression_id++;

    // operators
    Expression& apply_operator(ExpressionType type, const Expression& rhs);
public:
    // constructors
    Expression() = default;
    explicit Expression(ExpressionType type);
    Expression(const Variable& variable); // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    Expression(float coef); // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    Expression(const Expression& rhs);
    Expression& operator=(const Expression& rhs);

    // getters
    Variable as_variable();
    ConstVariable as_variable() const;
    float& as_numerical();
    float as_numerical() const;
    bool has_child(ExpressionSide side) const;
    Expression& child(ExpressionSide side);
    const Expression& child(ExpressionSide side) const;
    unsigned long int id() const;
    ExpressionType type() const;

    // setters
    void attach(ExpressionSide side, Expression& expr);
    void attach_copy(ExpressionSide side, const Expression& expr);
    void insert(ExpressionSide side, Expression& expr);
    void shift(ExpressionSide side);
    void detach_delete(ExpressionSide side);

    // operations on children
    friend void transfer(ExpressionSide from_side, Expression& from_expr, ExpressionSide to_side, Expression& to_expr);
    friend void swap(ExpressionSide from_side, Expression& from_expr, ExpressionSide to_side, Expression& to_expr);

    // operators
    Expression& operator+=(const Expression& rhs);
    Expression& operator*=(const Expression& rhs);
    Expression& operator-=(const Expression& rhs);
    Expression& operator/=(const Expression& rhs);

    // traversal algorithms
    template<ExpressionTraversalOrder order> void depth_first_traversal(const std::function<void(Expression& expr)>&);
    template<ExpressionTraversalOrder order> void depth_first_traversal(const std::function<void(const Expression& expr)>&) const;

    // evaluators
    float feval(bool cast_variables = true);

    // expression search/splits
    std::map<std::string, Expression> split_by_variable(const std::map<std::string, std::function<bool(const Variable&)>>& indicators);

    // output and display
    std::string to_elementary_string() const;
    std::string to_string() const;
    void export_to_dot(const std::string& filename, bool with_system_command = true) const;
};

template<L::ExpressionTraversalOrder order>
void L::Expression::depth_first_traversal(const std::function<void(Expression & expr)>& explore) {

    std::function<void(Expression&)> traversal;
    traversal = [&traversal, &explore](Expression& current){
        if constexpr(order == PostOrder) {
            if (current.has_child(Left)) traversal(current.child(Left));
            if (current.has_child(Right)) traversal(current.child(Right));
            explore(current);
        } else if constexpr(order == InOrder) {
            if (current.has_child(Left)) traversal(current.child(Left));
            explore(current);
            if (current.has_child(Right)) traversal(current.child(Right));
        } else {
            explore(current);
            if (current.has_child(Left)) traversal(current.child(Left));
            if (current.has_child(Right)) traversal(current.child(Right));
        }
    };

    try { traversal(*this); } catch (const StopIteration& err) {}
}

template<L::ExpressionTraversalOrder order>
void L::Expression::depth_first_traversal(const std::function<void(const Expression & expr)>& explore) const {
    std::function<void(const Expression&)> traversal;
    traversal = [&traversal, &explore](const Expression& current){
        if constexpr(order == PostOrder) {
            if (current.has_child(Left)) traversal(current.child(Left));
            if (current.has_child(Right)) traversal(current.child(Right));
            explore(current);
        } else if constexpr(order == InOrder) {
            if (current.has_child(Left)) traversal(current.child(Left));
            explore(current);
            if (current.has_child(Right)) traversal(current.child(Right));
        } else {
            explore(current);
            if (current.has_child(Left)) traversal(current.child(Left));
            if (current.has_child(Right)) traversal(current.child(Right));
        }
    };

    try { traversal(*this); } catch (const StopIteration& err) {}
}


#endif //ED_SOLVER_EXPRESSION_H
