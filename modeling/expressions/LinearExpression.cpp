//
// Created by hlefebvr on 12/11/19.
//

#include "LinearExpression.h"

float LinearExpression::offset(const Expression& expr) {
    if(is_numerical(expr._tree.node())) return expr._tree.node().as_numerical();

    if (!is_sum(expr._tree.node())) return expr.evaluate();

    return Expression::evaluate(expr._tree.child(ExpressionTree::Right));
}

LinearExpression::LinearExpressionIterator::iterator::iterator(const Expression &expr, bool is_end)
        : _it(is_end || !is_sum(expr._tree.node()) || !expr._tree.has_child(ExpressionTree::Left) ? nullptr : &expr._tree.child(ExpressionTree::Left))
{}

LinearExpression::LinearExpressionIterator::iterator &
LinearExpression::LinearExpressionIterator::iterator::operator++() {
    if (!_it->has_child(ExpressionTree::Left)) _it = nullptr;
    else _it = &_it->child(ExpressionTree::Left);
    return *this;
}

LinearExpression::LinearExpressionIterator::iterator::LinearTerm
LinearExpression::LinearExpressionIterator::iterator::operator*() {
    const ExpressionTree& base = _it->child(ExpressionTree::Right);
    float coefficient = Expression::evaluate(base.child(ExpressionTree::Right), [](const Variable& var){ return false; });
    Variable& variable = base.child(ExpressionTree::Left).child(ExpressionTree::Right).node().as_variable();
    return { variable, coefficient };
}
