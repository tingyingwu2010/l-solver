//
// Created by hlefebvr on 12/11/19.
//

#ifndef ED_SOLVER_LINEAREXPRESSION_H
#define ED_SOLVER_LINEAREXPRESSION_H

#include "Expression.h"
#include "../Variable.h"

namespace L {
    struct LinearExpression;
}

struct L::LinearExpression {
    class LinearExpressionIterator{
        const Expression& _expr;
    public:
        class iterator {
            ExpressionTree const* _it;
        public:
            struct LinearTerm {
                Variable& variable;
                float coefficient;
                LinearTerm(Variable& var, float coef) : variable(var), coefficient(coef) {}
            };
            iterator(const Expression& expr, bool is_end);
            bool operator==(const iterator& rhs) const { return _it == rhs._it; }
            bool operator!=(const iterator& rhs) const { return !(rhs == *this); }
            iterator& operator++();
            LinearTerm operator*();

        };
        explicit LinearExpressionIterator(const Expression& expr) : _expr(expr) {}
        iterator begin() { return {_expr, false}; }
        iterator end() { return {_expr, true}; }
    };
    static LinearExpressionIterator terms(const Expression& expr) { return LinearExpressionIterator(expr); }
    static float offset(const Expression& expr);
};


#endif //ED_SOLVER_LINEAREXPRESSION_H
