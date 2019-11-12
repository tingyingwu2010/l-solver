//
// Created by hlefebvr on 12/11/19.
//

#include "Expression.h"

#include <vector>
#include <map>

using namespace L;

Expression::Expression(float num) : _tree(ExpressionTree(*new ExpressionNode(num))) {}
Expression::Expression(Variable& variable) : _tree(ExpressionTree(*new ExpressionNode(variable))) {}
Expression::Expression(const ExpressionNode &node) : _tree(*new ExpressionNode(node)) {}
Expression &Expression::operator=(float rhs) { *this = Expression(rhs); return *this; }
Expression &Expression::operator=(Variable& rhs) { _tree = Expression(rhs)._tree; return *this; }
Expression &Expression::operator=(const Expression &rhs) { _tree = rhs._tree; return *this; }

void Expression::export_to_dot(const std::string &filename, bool with_system_command) const {
    _tree.export_to_dot(filename, with_system_command);
}

// void Expression::normalize() {}

void Expression::expand(unsigned long depth) {

    _tree.depth_first_transform(ExpressionTree::PostOrder, [this](ExpressionTree& current){

        // correct losing normal form
        if (is_product(current.node()) && current.has_child(ExpressionTree::Left) && !is_product(current.child(ExpressionTree::Left).node())) {
            /* current.node().color(true);
            ExpressionTree& added_product = *new ExpressionTree(ExpressionNode(Product));
            current.insert(ExpressionTree::Left, added_product);
            ExpressionTree::transfer(ExpressionTree::Left, current.child(ExpressionTree::Left), ExpressionTree::Right, current.child(ExpressionTree::Left));
            return true; */
        }

        if (is_sum(current.node()) && current.has_child(ExpressionTree::Right)) {
            ExpressionTree& right = current.child(ExpressionTree::Right);
            if (is_sum(right.node())) {
                if (!right.has_child(ExpressionTree::Left)) {
                    current.insert(ExpressionTree::Left, right);
                    current.detach(ExpressionTree::Right);
                    return true;
                } else {
                    while (right.has_child(ExpressionTree::Left) && is_sum(right.child(ExpressionTree::Left).node())) {
                        ExpressionTree& left = right.child(ExpressionTree::Left);
                        right.detach(ExpressionTree::Left);
                        if(left.has_child(ExpressionTree::Left)) {
                            right.attach(ExpressionTree::Left, left.child(ExpressionTree::Left));
                            left.detach(ExpressionTree::Left);
                        }
                        current.insert(ExpressionTree::Left, left);
                    }
                    ExpressionTree& skipped = current.child(ExpressionTree::Right);
                    current.reduce_to(ExpressionTree::Right);
                    skipped.unsafe_mode(true);
                    delete &skipped;
                    return true;
                }
            }

            if (current.has_child(ExpressionTree::Left) && is_sum(current.child(ExpressionTree::Left).node())) {
                ExpressionTree& left = current.child(ExpressionTree::Left);
                if (left.has_child(ExpressionTree::Right) && is_numerical(left.child(ExpressionTree::Right).node())) {
                    ExpressionTree& left_right = left.child(ExpressionTree::Right);
                    if (is_numerical(right.node())) {
                        right.node().as_numerical() += left_right.node().as_numerical();
                        if (left.has_child(ExpressionTree::Left)) {
                            ExpressionTree &forward_left = left.child(ExpressionTree::Left);
                            left.detach(ExpressionTree::Left);
                            current.detach_delete(ExpressionTree::Left);
                            current.attach(ExpressionTree::Left, forward_left);
                        } else {
                            current.detach_delete(ExpressionTree::Left);
                        }
                    } else {
                        ExpressionTree::swap(ExpressionTree::Right, left, ExpressionTree::Right, current);
                        return true;
                    }
                }
            }
        }

        if (is_product(current.node()) && current.has_child(ExpressionTree::Left)) {

            if (
                    is_product(current.child(ExpressionTree::Left).node())
                    && current.has_child(ExpressionTree::Right)
                    && is_sum(current.child(ExpressionTree::Right).node())
                    ) {
                ExpressionTree::swap(ExpressionTree::Right, current.child(ExpressionTree::Left), ExpressionTree::Right,
                                     current);
            }

            ExpressionTree& left = current.child(ExpressionTree::Left);
            if (is_product(left.node()) && left.has_child(ExpressionTree::Right)) {
                ExpressionTree& left_right = left.child(ExpressionTree::Right);
                if (is_sum(left_right.node())) {

                    ExpressionTree& right = current.child(ExpressionTree::Right);
                    // current
                    current.node().type(Sum);

                    // right of current
                    ExpressionTree& added_right = *new ExpressionTree(ExpressionNode(Product));
                    ExpressionTree& added_product = *new ExpressionTree(ExpressionNode(Product));
                    added_right.attach(ExpressionTree::Left, added_product);
                    ExpressionTree& B = left_right.child(ExpressionTree::Left);
                    /* if (!B.has_child(ExpressionTree::Left)) */ B.reduce_to_delete(ExpressionTree::Right);
                    added_right.child(ExpressionTree::Left).attach_copy(ExpressionTree::Right, B);
                    current.insert(ExpressionTree::Right, added_right);

                    // left of current
                    left.node().type(Sum);
                    left_right.node().type(Product);
                    ExpressionTree& added_left = *new ExpressionTree(ExpressionNode(Product));
                    added_left.attach_copy(ExpressionTree::Right, right);
                    left_right.detach(ExpressionTree::Left);
                    left_right.attach(ExpressionTree::Left, added_left);

                    // On the fly evaluation
                    ExpressionTree& a = current.child(ExpressionTree::Left).child(ExpressionTree::Right).child(ExpressionTree::Right);
                    ExpressionTree& b = current.child(ExpressionTree::Left).child(ExpressionTree::Right).child(ExpressionTree::Left).child(ExpressionTree::Right);
                    if (is_numerical(a.node()) && is_numerical(b.node())) {
                        a.node().as_numerical() *= b.node().as_numerical();
                        current.child(ExpressionTree::Left).child(ExpressionTree::Right).detach_delete(ExpressionTree::Left);
                        current.child(ExpressionTree::Left).child(ExpressionTree::Right).reduce_to_delete(ExpressionTree::Right);
                    }

                    return true;
                }
            }
        }
        return false;
    });

}

float Expression::evaluate(const std::function<bool(const Variable &)> &indicator) const {
    return evaluate(_tree, indicator);
}

float Expression::evaluate(const ExpressionTree& tree, const std::function<bool(const Variable &)> &indicator) {

    std::function<float(const ExpressionTree&)> eval;
    eval = [&eval, &indicator](const ExpressionTree& current) -> float {

        if (is_numerical(current.node())) return current.node().as_numerical();

        if (is_variable(current.node())) {
            const Variable& variable = current.node().as_variable();
            if (indicator(variable)) throw Exception("Var found when evaluating.");
            return variable.value();
        }

        if (is_sum(current.node())) {
            float result = 0;
            ExpressionTree const* it = &current;
            do result += eval(it->child(ExpressionTree::Right));
            while ( it->has_child(ExpressionTree::Left) && (it = &it->child(ExpressionTree::Left)) );
            return result;
        }

        if (is_product(current.node())) {
            float result = 1;
            ExpressionTree const* it = &current;
            do result *= eval(it->child(ExpressionTree::Right));
            while ( it->has_child(ExpressionTree::Left) && (it = &it->child(ExpressionTree::Left)) );
            return result;
        }

        throw Exception("Not implemented yet");

    };

    return eval(tree);
}

void Expression::linear_factorize(const std::function<bool(const Variable &)> &indicator) {

    if (is_numerical(_tree.node()) || is_variable(_tree.node())) return;

    std::map<Variable*, ExpressionTree*> handler;
    auto create_or_edit_handler = [&handler](ExpressionTree& parent) -> bool {
        ExpressionTree& term = parent.child(ExpressionTree::Right);
        Variable& variable = term.child(ExpressionTree::Left).child(ExpressionTree::Right).node().as_variable();
        auto found = handler.find(&variable);

        if (found == handler.end()) {
            handler.insert({ &variable, &term.child(ExpressionTree::Right) });
            return false;
        }

        ExpressionTree& existing_term = *found->second;

        if (is_numerical(term.child(ExpressionTree::Right).node())) {

            float n = term.child(ExpressionTree::Right).node().as_numerical();
            if (is_numerical(existing_term.node())) {
                existing_term.node().as_numerical() += n;
            } else if (is_numerical(existing_term.child(ExpressionTree::Right).node())) {
                existing_term.child(ExpressionTree::Right).node().as_numerical() += n;
            } else if (is_sum(existing_term.node())) {
                ExpressionTree& added_sum = *new ExpressionTree(ExpressionNode(Sum));
                added_sum.attach(ExpressionTree::Right, *new ExpressionTree(ExpressionNode(n)));
                existing_term.insert(ExpressionTree::Left, added_sum);
                ExpressionTree::swap(ExpressionTree::Right, existing_term, ExpressionTree::Right, added_sum);
            } else {
                ExpressionTree& added_coefficient = *new ExpressionTree(ExpressionNode(n));
                ExpressionTree& forward_existing = *new ExpressionTree(existing_term.node());
                existing_term.insert(ExpressionTree::Left, forward_existing);
                ExpressionTree::transfer(ExpressionTree::Right, existing_term, ExpressionTree::Right, existing_term.child(ExpressionTree::Left));
                existing_term.node().type(Sum);
                existing_term.attach(ExpressionTree::Right, added_coefficient);
            }

        } else if (is_sum(term.child(ExpressionTree::Right).node())) {

            if (is_numerical(term.child(ExpressionTree::Right).child(ExpressionTree::Right).node())) {

                float n = term.child(ExpressionTree::Right).child(ExpressionTree::Right).node().as_numerical();
                if (is_numerical(existing_term.node())) {
                    existing_term.node().as_numerical() += n;
                } else if (is_numerical(existing_term.child(ExpressionTree::Right).node())) {
                    existing_term.child(ExpressionTree::Right).node().as_numerical() += n;
                } else {
                    ExpressionTree& added_sum = *new ExpressionTree(ExpressionNode(Sum));
                    added_sum.attach(ExpressionTree::Right, *new ExpressionTree(ExpressionNode(n)));
                    existing_term.insert(ExpressionTree::Left, added_sum);
                    ExpressionTree::swap(ExpressionTree::Left, existing_term, ExpressionTree::Right, existing_term);
                }

            } else {
                existing_term.insert(ExpressionTree::Left, term.child(ExpressionTree::Right));
            }

        } else {

            if (is_numerical(existing_term.node())) {
                ExpressionTree& forwarded_numerical = *new ExpressionTree(ExpressionNode(existing_term.node().as_numerical()));
                existing_term.node().as_numerical() = 0;
                existing_term.node().type(Sum);
                ExpressionTree& added_sum_term = *new ExpressionTree(ExpressionNode(Sum));
                existing_term.attach(ExpressionTree::Left, added_sum_term);
                existing_term.attach(ExpressionTree::Right, forwarded_numerical);
                added_sum_term.attach(ExpressionTree::Right, term.child(ExpressionTree::Right));
            } else {
                ExpressionTree* last;
                for (last = &existing_term ; last->has_child(ExpressionTree::Left) ; last = &last->child(ExpressionTree::Left) ){}
                ExpressionTree& added_sum_term = *new ExpressionTree(ExpressionNode(Sum));
                existing_term.attach(ExpressionTree::Left, added_sum_term);
                added_sum_term.attach(ExpressionTree::Right, term.child(ExpressionTree::Right));
            }

        }

        term.detach_delete(ExpressionTree::Right);
        if (parent.has_child(ExpressionTree::Left)) {
            ExpressionTree& skipped = parent.child(ExpressionTree::Left);
            if(skipped.has_child(ExpressionTree::Left)) skipped.detach(ExpressionTree::Left);
            parent.detach_delete(ExpressionTree::Right);
            parent.reduce_to(ExpressionTree::Left);
            skipped.unsafe_mode(true);
            delete &skipped;
            return true;
        } else {
            parent.detach_delete(ExpressionTree::Right);
        }

        return false;
    };

    ExpressionTree* current = &_tree;
    if (!is_sum(_tree.node())) {
        return;
        ExpressionTree virtual_sum((ExpressionNode(Sum)));
        virtual_sum.unsafe_mode(true);
        virtual_sum.attach(ExpressionTree::Right, _tree);
        current = &virtual_sum;
    }

    bool force_loop;
    ExpressionTree* last = nullptr;
    do {

        force_loop = false;

        if (!current->has_child(ExpressionTree::Right)) continue;

        if (is_variable(current->child(ExpressionTree::Right).node()) && indicator(current->child(ExpressionTree::Right).node().as_variable())) {

            ExpressionTree& added_product_by_one = *new ExpressionTree(ExpressionNode(Product));
            current->insert(ExpressionTree::Right, added_product_by_one);

            ExpressionTree& added_product_by_var = *new ExpressionTree(ExpressionNode(Product));
            added_product_by_var.attach(ExpressionTree::Right, *new ExpressionTree(ExpressionNode(1)));

            added_product_by_one.attach(ExpressionTree::Left, added_product_by_var);
            ExpressionTree::swap(ExpressionTree::Right, added_product_by_one, ExpressionTree::Right, added_product_by_var);

            force_loop = create_or_edit_handler(*current);

        } else if (is_product(current->child(ExpressionTree::Right).node())) {

            ExpressionTree& product = current->child(ExpressionTree::Right);

            // trivial cases with two operands
            if ( !product.child(ExpressionTree::Left).has_child(ExpressionTree::Left) ) {
                if (
                        is_variable(product.child(ExpressionTree::Right).node())
                        && indicator(product.child(ExpressionTree::Right).node().as_variable())
                        ) {
                    ExpressionTree::swap(ExpressionTree::Right, product.child(ExpressionTree::Left), ExpressionTree::Right, product);

                    force_loop = create_or_edit_handler(*current);

                } else if (
                        is_variable(product.child(ExpressionTree::Left).child(ExpressionTree::Right).node())
                        && indicator(product.child(ExpressionTree::Left).child(ExpressionTree::Right).node().as_variable())
                        ) {

                    force_loop = create_or_edit_handler(*current);

                }

            } else {

                // non trivial case with multiple operands, we have to look for the variable of interest (if any)
                ExpressionTree* term_of_interest = nullptr;
                ExpressionTree* term = &product;
                ExpressionTree* last_term = nullptr;
                do {
                    if (is_variable(term->child(ExpressionTree::Right).node()) && indicator(term->child(ExpressionTree::Right).node().as_variable())) {
                        if (term_of_interest != nullptr) throw Exception("The given indicator does not make the expression linear.");
                        term_of_interest = term;
                    }
                } while (term->has_child(ExpressionTree::Left) && (last_term = term) && (term = &term->child(ExpressionTree::Left)));

                if (term_of_interest) {

                    // turn all '*' into '+'
                    /* ExpressionTree* last_term = nullptr;
                    term = &product;
                    do term->node().type(Product);
                    while (
                            term->has_child(ExpressionTree::Left)
                            && (last_term = term)
                            && (term = &term->child(ExpressionTree::Left))
                        ); */

                    // remove the variable from the obtained sum
                    ExpressionTree& variable = term_of_interest->child(ExpressionTree::Right);
                    term_of_interest->detach(ExpressionTree::Right);
                    if (term_of_interest->has_child(ExpressionTree::Left)) term_of_interest->reduce_to(ExpressionTree::Left);
                    else last_term->reduce_to(ExpressionTree::Left);

                    // add product with variable
                    ExpressionTree& product_by_coefficient = *new ExpressionTree(ExpressionNode(Product));
                    ExpressionTree& product_by_variable = *new ExpressionTree(ExpressionNode(Product));
                    current->insert(ExpressionTree::Right, product_by_coefficient);
                    product_by_coefficient.attach(ExpressionTree::Left, product_by_variable);
                    product_by_variable.attach(ExpressionTree::Right, variable);

                    force_loop = create_or_edit_handler(*current);
                }

            }

        } else {

            // TODO handle offset !!

        }

    } while( force_loop || (current->has_child(ExpressionTree::Left) && (last = current) && (current = &current->child(ExpressionTree::Left))));

    if (last->has_child(ExpressionTree::Left) && !last->child(ExpressionTree::Left).has_child(ExpressionTree::Right)) {
        last->detach_delete(ExpressionTree::Left);
    }

}

Expression::Expression(Type type, const Expression &node) : Expression(ExpressionNode(type)) {
    if (!is_unitary_operator(type)) throw Exception("Trying to build an expression of non-unitary-operator type with only one argument. "
                                                    "If you intended to create an expression with one variable, you need not specify the type. "
                                                    "Use Expression(Var& variable) instead. Otherwise, the type you requested is a binary "
                                                    "operator with insufficient number of argument. ");
    _tree.attach_copy(ExpressionTree::Right, node._tree);
}

Expression::Expression(Type type, Variable &node) : Expression(type, Expression(node)) {}
Expression::Expression(Type type, float node) : Expression(type, Expression(node)) {}

Expression L::operator+(const Expression &a, const Expression &b) {
    Expression output;
    if (!is_sum(a._tree.node())) {
        output = Expression(ExpressionNode(Sum));
        output._tree.attach_copy(ExpressionTree::Right, a._tree);
    } else {
        output = a;
    }

    if (is_numerical(b._tree.node())){
        if (output._tree.has_child(ExpressionTree::Right) && is_numerical(output._tree.child(ExpressionTree::Right).node())) {
            output._tree.child(ExpressionTree::Right).node().as_numerical() += b._tree.node().as_numerical();
            if (!output._tree.has_child(ExpressionTree::Left)) output._tree.reduce_to_delete(ExpressionTree::Right);
            return output;
        }

        ExpressionTree added_sum((ExpressionNode(Sum)));
        added_sum.insert_copy(ExpressionTree::Right, b._tree);
        output._tree.insert_copy(ExpressionTree::Left, added_sum);
        ExpressionTree::swap(ExpressionTree::Right, output._tree, ExpressionTree::Right, output._tree.child(ExpressionTree::Left));
        return output;
    }

    if (is_sum(b._tree.node())) {

        if (b._tree.has_child(ExpressionTree::Right) && is_numerical(b._tree.child(ExpressionTree::Right).node())) {
            Expression right;
            right._tree = b._tree.child(ExpressionTree::Right); // WARNING copy
            if (b._tree.has_child(ExpressionTree::Left)) {
                Expression left;
                left._tree = b._tree.child(ExpressionTree::Left); // WARNING copy
                return a + left + right;
            }
            return a + right;
        }

        output._tree.insert_copy(ExpressionTree::Left, b._tree);
        return output;
    }

    if (output._tree.has_child(ExpressionTree::Right)) {

        ExpressionTree added_sum((ExpressionNode(Sum)));
        added_sum.attach_copy(ExpressionTree::Right, b._tree);
        output._tree.insert_copy(ExpressionTree::Left, added_sum);
        return output;
    }

    output._tree.attach_copy(ExpressionTree::Right, b._tree);
    return output;
}

Expression L::operator*(const Expression &a, const Expression &b) {
    Expression output;
    if (!is_product(a._tree.node())) {
        output = Expression(ExpressionNode(Product));
        output._tree.attach_copy(ExpressionTree::Right, a._tree);
    } else {
        output = a;
    }

    if ( is_numerical(b._tree.node())){
        if (output._tree.has_child(ExpressionTree::Right) && is_numerical(output._tree.child(ExpressionTree::Right).node())) {
            output._tree.child(ExpressionTree::Right).node().as_numerical() *= b._tree.node().as_numerical();
            if (!output._tree.has_child(ExpressionTree::Left)) output._tree.reduce_to_delete(ExpressionTree::Right);
            return output;
        }

        ExpressionTree added_sum((ExpressionNode(Product)));
        added_sum.insert_copy(ExpressionTree::Right, b._tree);
        output._tree.insert_copy(ExpressionTree::Left, added_sum);
        ExpressionTree::swap(ExpressionTree::Right, output._tree, ExpressionTree::Right, output._tree.child(ExpressionTree::Left));
        return output;
    }

    if (is_product(b._tree.node())) {

        if (b._tree.has_child(ExpressionTree::Right) && is_numerical(b._tree.child(ExpressionTree::Right).node())) {
            Expression right;
            right._tree = b._tree.child(ExpressionTree::Right); // WARNING copy
            if (b._tree.has_child(ExpressionTree::Left)) {
                Expression left;
                left._tree = b._tree.child(ExpressionTree::Left); // WARNING copy
                return a * left * right;
            }
            return a * right;
        }

        output._tree.insert_copy(ExpressionTree::Left, b._tree);
        return output;
    }

    if (output._tree.has_child(ExpressionTree::Right)) {

        ExpressionTree added_sum((ExpressionNode(Product)));
        added_sum.attach_copy(ExpressionTree::Right, b._tree);
        output._tree.insert_copy(ExpressionTree::Left, added_sum);
        return output;
    }

    output._tree.attach_copy(ExpressionTree::Right, b._tree);
    return output;
}
