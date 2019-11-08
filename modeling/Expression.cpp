//
// Created by hlefebvr on 04/11/19.
//

#include <fstream>
#include <cmath>
#include <functional>
#include "Expression.h"
#include "../utils/Exception.h"
#include "Variable.h"

/****  TERM  ****/

ED::Variable &ED::Term::as_variable() {
    if (_type != Var) throw Exception("Trying to convert non-variable term to variable");
    return *_variable;
}

float& ED::Term::as_numerical() {
    if (_type != Num) throw Exception("Trying to convert non-numerical term to numeric");
    return _value;
}

void ED::Term::use_variable_as_numerical() {
    if (_type != Var && _variable == nullptr) throw Exception("Trying to use a non-variable term as numeric with hidden variable");
    _type = Num;
    _value = _variable->value();
}

bool ED::Term::is_hidden_variable() {
    return _type == Num && _variable != nullptr;
}

ED::Term::Term(float num) : _type(Num), _value(num) {}

ED::Term::Term(ED::Variable &var) : _type(Var), _variable(&var) {}

ED::Term::Term(ED::Term::Type type) : _type(type) {
    if (type == Num || type == Var) throw Exception("Term(Type) should only be used to build functions");
}

ED::Term::Term(const Term& term) {
    *this = term;
}

ED::Term& ED::Term::operator=(const ED::Term &term) {
    delete _left;
    delete _right;
    if (term._left != nullptr) _left = new Term(*term._left);
    if (term._right != nullptr) _right = new Term(*term._right);
    _type = term._type;
    _variable = term._variable;
    _value = term._value;
    return *this;
}

std::string ED::Term::to_string() const {
    switch (_type) {
        case Num: return std::to_string(_value);
        case Var: return _variable->user_defined_name();
        case Sum: return "+";
        case Mul: return "*";
        case Cos: return "cos";
        case Sin: return "sin";
        case Ln: return "ln";
        case Log: return "log";
        case Exp: return "e^";
        case Pow: return "^";
        case Sqrt: return "sqrt";
        case Div: return "/";
        case Sub: return "-";
        default: throw Exception("Unknown term type");
    }
}

#include <iostream>
#include <vector>
#include <map>
#include <stack>

ED::Term::~Term() {
    if (!_expert_mode) {
        delete _left;
        delete _right;
    }
}

void ED::Term::swap() {
    Term* left = _left;
    _left = _right;
    _right = left;
}

bool ED::Term::is_unitary_operator(ED::Term::Type type) {
    return type == Cos || type == Sin || type == Log || type == Ln || type == Sqrt || type == Exp;
}

bool ED::Term::is_binary_operator(ED::Term::Type type) {
    return type == Sum || type == Sub || type == Mul || type == Div || type == Pow;
}

float ED::Term::apply(float n1, ED::Term::Type op, float n2) {
    switch (op) {
        case Sum: return n1 + n2;
        case Sub: return n1 - n2;
        case Mul: return n1 * n2;
        case Div: return n1 / n2;
        case Pow: return std::pow(n1, n2);
        default: throw Exception("Unknown binary operator");
    }
}

/****  EXPRESSION  ****/

ED::Expression::Expression(const ED::Term &term) : _root(new Term(term)) {}

ED::Expression::Expression(const ED::Expression& expr) {
    if (expr._root == nullptr) {
        delete _root;
        _root = nullptr;
        return;
    }
    _root = new Term(*expr._root);
}

ED::Expression::Expression(ED::Variable &var) : _root(new Term(var)) {}

ED::Expression::Expression(float v) : _root(new Term(v)) {}

void ED::Expression::to_dot() const {
    std::ofstream f;
    f.open("expression_tree.dot");
    f << "graph expression {\n";

    std::function<int(Term&, unsigned int)> write_term;
    write_term = [&f, &write_term](Term& current, unsigned int id) {
        f << "\t" << id << "[label=\" " << current.to_string() << " \"]\n";

        unsigned int last_used_id = id;
        if (current.has_left()) {
            f << "\t " << id << " -- " << last_used_id + 1 << "\n";
            last_used_id = write_term(current.left(), last_used_id + 1);
        }

        if (current.has_right()) {
            f << "\t " << id << " -- " << last_used_id + 1 << "\n";
            last_used_id = write_term(current.right(), last_used_id + 1);
        }

        return last_used_id;
    };

    write_term(*_root, 0);

    f << "}";
    f.close();
}

ED::Expression &ED::Expression::operator=(const Expression& expr) {
    delete _root;
    _root = new Term(*expr._root);
    return *this;
}

void ED::Expression::add_term(const Term& rhs, Term::Type operation) {
    if (_root == nullptr) {
        _root = new Term(rhs);
        return;
    }

    Term& tree = *new Term(operation);
    tree.left(*_root);
    tree.right(*new Term(rhs));
    _root = &tree;
}

ED::Expression::~Expression() {
    delete _root;
}

void ED::Expression::remove_minus() {

    if (_root != nullptr) {

        std::function<Term*(Term*)> treat_one_node;
        treat_one_node = [&treat_one_node](Term* root) {

            if (root->type() == Term::Sub) {
                Term* added_node_sum = new Term(Term::Sum);
                Term* added_node_mul = new Term(Term::Mul);
                Term* added_node_1 = new Term(-1);
                Term* left = &root->left();
                Term* right = &root->right();

                added_node_mul->left(*added_node_1);
                added_node_mul->right(*right);
                added_node_sum->right(*added_node_mul);
                added_node_sum->left(*left);

                root->expert_mode(true);
                delete root;

                return added_node_sum;
            }

            if (root->has_left()) {
                Term* left = treat_one_node(&root->left());
                root->left(*left);
            }

            if (root->has_right()) {
                Term* right = treat_one_node(&root->right());
                root->right(*right);
            }

            return root;
        };

        _root = treat_one_node(_root);
    }

}

void ED::Expression::expand() {

    if (_root != nullptr) {

        remove_minus();

        std::function<Term*(Term*)> treat_one_node;
        treat_one_node = [&treat_one_node](Term* root) {

            // expand integer power
            if (root->type() == Term::Pow && root->right().type() == Term::Num) {
                float float_value = root->right().as_numerical();
                auto int_value = static_cast<unsigned long int>(float_value);
                if (int_value == float_value) {
                    if (int_value > 1) {

                        Term* left = &root->left();

                        Term* added_node_mul_root = new Term(Term::Mul);
                        added_node_mul_root->left(*left);
                        Term* parent = added_node_mul_root;
                        for (unsigned long int i = 0, n = int_value; i < n - 2; i += 1) {
                            Term* added_copy_left = new Term(*left);
                            Term* added_node_mul = new Term(Term::Mul);

                            parent->right(*added_node_mul);
                            added_node_mul->left(*added_copy_left);

                            parent = added_node_mul;
                        }
                        Term* added_copy_left = new Term(*left);
                        parent->right(*added_copy_left);

                        root->expert_mode(true);
                        delete &root->right();
                        delete root;

                        return treat_one_node(added_node_mul_root);
                    }

                    if (int_value == 1) {
                        Term* left = &root->left();
                        root->expert_mode(true);
                        delete &root->right();
                        delete root;
                        return treat_one_node(left);
                    }

                    if (int_value == 0 && root->left().type() == Term::Num) {
                        if (root->left().as_numerical() == 0)
                            throw Exception("Could not reduce 0^0. Undefined expression.");
                        delete root;
                        return new Term(1);
                    }
                }
            }

            // expand * over + (+ on the right)
            if (root->type() == Term::Mul && root->right().type() == Term::Sum) {
                Term* A = &root->left();
                Term* B = &root->right().left();
                Term* C = &root->right().right();

                Term* added_node_sum = new Term(Term::Sum);
                added_node_sum->left(*root);
                root->right(*B);

                Term* added_node_mul = new Term(Term::Mul);
                Term* added_node_copy_A = new Term(*A);
                added_node_mul->left(*added_node_copy_A);
                added_node_mul->right(*C);
                added_node_sum->right(*added_node_mul);

                return treat_one_node(added_node_sum);
            }

            // expand * over + (+ on the left)
            if (root->type() == Term::Mul && root->left().type() == Term::Sum) {
                root->swap();
                root = treat_one_node(root);
                root->swap();
                return treat_one_node(root);
            }

            if (root->has_left()) {
                Term* left = treat_one_node(&root->left());
                root->left(*left);
            }

            if (root->has_right()) {
                Term* right = treat_one_node(&root->right());
                root->right(*right);
            }

            return root;
        };

        _root = treat_one_node(_root);
    }

}

std::string ED::Expression::to_string() const {

    std::function<std::string(Term*)> str;
    str = [&str](Term* term) {
        std::string output;
        if (Term::is_binary_operator(term->type()))
            output += "( " + str(&term->left()) + " " + term->to_string() + " " + str(&term->right()) + " )";
        else if(Term::is_unitary_operator(term->type()))
            output += term->to_string() + "( " + str(&(term->has_left() ? term->left() : term->right())) + " )";
        else if (term->type() == Term::Num)
            output += std::to_string(term->as_numerical());
        else if (term->type() == Term::Var)
            output += term->as_variable().user_defined_name();
        else throw Exception("Unknown expression while parsing");
        return output;
    };

    return (_root == nullptr) ? "" : str(_root);
}


void ED::Expression::reduce() {
    if (_root == nullptr) return;

    remove_minus();

    std::function<Term*(Term*, unsigned int)> treat_one_node;
    treat_one_node = [&treat_one_node](Term* root, unsigned int inner_counter) {

        // if comutative operation
        if (root->type() == Term::Sum || root->type() == Term::Mul) {

            // put numbers on the right
            if (root->left().type() == Term::Num && root->right().type() != Term::Num) {
                Term* new_root = new Term(root->type());
                new_root->right(root->left());
                new_root->left(root->right());
                root->expert_mode(true);
                delete root;
                return treat_one_node(new_root, 0);
            }

            // align same operations in layer
            if (root->left().type() != root->type() && root->right().type() == root->type()) {
                Term* new_root = new Term(root->type());
                new_root->right(root->left());
                new_root->left(root->right());
                root->expert_mode(true);
                delete root;
                return treat_one_node(new_root, 0);
            }

        }

        // compute binary operations
        if (Term::is_binary_operator(root->type()) && root->left().type() == Term::Num && root->right().type() == Term::Num) {

            float n1 = root->left().as_numerical();
            float n2 = root->right().as_numerical();
            Term::Type op = root->type();

            Term* new_root = new Term(Term::apply(n1, op, n2));
            delete root;
            return new_root;
        }

        // put all sums to the left
        // TODO mul as well
        if (root->type() == Term::Sum && root->left().type() == Term::Sum && root->right().type() == Term::Sum) {
            Term* right = &root->right();
            Term* right_left = &root->right().left();

            right->left(*root);
            root->right(*right_left);

            return treat_one_node(right, 0);
        }

        // put num at the highest level and reduce
        if (Term::is_binary_operator(root->type()) && root->left().type() == root->type() ) {
            Term::Type op = root->type();

            // put num at the highest level
            if (inner_counter == 0) {

                Term* uppest_non_numerical = root->right().type() != Term::Num ? root : nullptr;
                for (Term* current = &root->left() ; current->type() == op ; current = &current->left()) {

                    if (current->right().type() != Term::Num && uppest_non_numerical == nullptr) {
                        uppest_non_numerical = current;
                    }

                    if (current->right().type() == Term::Num && uppest_non_numerical != nullptr) {
                        Term* uppest_non_numerical_term = &uppest_non_numerical->right();
                        uppest_non_numerical->right(current->right());
                        current->right(*uppest_non_numerical_term);
                        uppest_non_numerical = current;
                    }

                }

                // return treat_one_node(root, 1);
            }

            if (root->right().type() == Term::Num && root->left().right().type() == Term::Num) {

                Term *left = &root->left();
                float n1 = root->left().right().as_numerical();
                float n2 = root->right().as_numerical();

                left->right().as_numerical() = Term::apply(n1, op, n2);

                root->expert_mode(true);
                delete root;

                return treat_one_node(left, 1);

            }

        }

        if (root->has_left()) {
            Term* original_left = &root->left();
            Term* left = treat_one_node(&root->left(), 0);
            root->left(*left);
            if (left != original_left) root = treat_one_node(root, 0);
        }

        if (root->has_right()) {
            Term* original_right = &root->right();
            Term* right = treat_one_node(&root->right(), 0);
            root->right(*right);
            if (right != original_right) root = treat_one_node(root, 0);
        }

        return root;
    };

    _root = treat_one_node(_root, 0);

}

// TODO WARNING, variables of interests have to be on the left
void ED::Expression::linear_group_by(const std::function<bool(const Variable&)>& indicator) {
    typename std::map<std::string, Term*> handler;

    std::function<Term*(Term*)> treat_one_node;
    treat_one_node = [&treat_one_node, &handler, &indicator](Term* root){

        if (root->type() != Term::Sum) return root;

        if (root->left().type() != Term::Sum) {

            if (root->left().type() == Term::Mul) {
                Term* potential_variable = &root->left().left();
                if (potential_variable->type() == Term::Var && indicator(potential_variable->as_variable())) {

                    std::string name = potential_variable->as_variable().user_defined_name();
                    auto found = handler.find(name);
                    if (found != handler.end()) {
                        Term* existing_term = &found->second->right();
                        Term* added_node_sum = new Term(Term::Sum);
                        added_node_sum->left(*existing_term);
                        added_node_sum->right(root->left().right());
                        found->second->right(*added_node_sum);

                        root->expert_mode(true);
                        root->left().expert_mode(true);
                        delete &root->left().left();
                        delete &root->left();
                        delete root;

                        return &root->right();
                    }

                }
            }

            if (root->left().type() == Term::Var) {
                Term* potential_variable = &root->left();
                if (potential_variable->type() == Term::Var && indicator(potential_variable->as_variable())) {

                    std::string name = potential_variable->as_variable().user_defined_name();
                    auto found = handler.find(name);
                    if (found == handler.end()) {

                        Term* added_node_mul = new Term(Term::Mul);
                        Term* added_node_one = new Term(1);
                        added_node_mul->left(*potential_variable);
                        added_node_mul->right(*added_node_one);
                        root->left(*added_node_mul);

                        return root;

                    } else {

                        Term* existing_term = &found->second->right();
                        Term* added_node_sum = new Term(Term::Sum);
                        Term* added_node_one = new Term(1);
                        added_node_sum->left(*existing_term);
                        added_node_sum->right(*added_node_one);
                        found->second->right(*added_node_sum);

                        Term* added_node_zero = new Term(0);
                        delete &root->left();
                        root->left(*added_node_zero);

                        return treat_one_node(root);
                    }

                }
            }

        }

        if (root->right().type() == Term::Mul) {
            Term* potential_variable = &root->right().left();
            if (potential_variable->type() == Term::Var && indicator(potential_variable->as_variable())) {
                std::string name = potential_variable->as_variable().user_defined_name();
                auto found = handler.find(name);
                if (found == handler.end()) {

                    handler.insert({ name, &root->right() });

                    return treat_one_node(root);

                } else if (!root->right().has_right() || found->second != &root->right()) {

                    Term* existing_term = &found->second->right();
                    Term* added_node_sum = new Term(Term::Sum);
                    added_node_sum->left(*existing_term);
                    added_node_sum->right(root->right().right());
                    found->second->right(*added_node_sum);

                    root->expert_mode(true);
                    root->right().expert_mode(true);
                    delete &root->right().left();
                    delete &root->right();
                    delete root;

                    return treat_one_node(&root->left());
                }
            }
        }

        if (root->right().type() == Term::Var) {
            if (indicator(root->right().as_variable())) {
                std::string name = root->right().as_variable().user_defined_name();
                auto found = handler.find(name);
                if (found == handler.end()) {

                    Term* added_node_mul = new Term(Term::Mul);
                    Term* added_node_one = new Term(1);
                    added_node_mul->left(root->right());
                    added_node_mul->right(*added_node_one);
                    root->right(*added_node_mul);
                    handler.insert({ name, added_node_mul });

                    return treat_one_node(root);

                } else if(!root->right().has_right() || found->second != &root->right()) {

                    Term* existing_term = &found->second->right();
                    Term* added_node_sum = new Term(Term::Sum);
                    Term* added_node_one = new Term(1);
                    added_node_sum->left(*existing_term);
                    added_node_sum->right(*added_node_one);
                    found->second->right(*added_node_sum);

                    root->expert_mode(true);
                    root->right().expert_mode(true);
                    delete &root->right().left();
                    delete &root->right();
                    delete root;

                    return treat_one_node(&root->left());
                }
            }
        }

        Term* original_left = &root->left();
        Term* left = treat_one_node(&root->left());
        root->left(*left);
        if (original_left != left) root =  treat_one_node(root);

        return root;
    };

    _root = treat_one_node(_root);

}

ED::Expression ED::Expression::linear(const std::function<bool(const Variable&)>& indicator) const {
    Expression expr = evaluate(indicator);
    expr.linear_group_by(indicator);
    expr.reduce();
    return expr;
}

void ED::Expression::linear_transform(const std::function<bool(const Variable &)> &indicator) {
    evaluate(indicator);
    linear_group_by(indicator);
    reduce();
}

ED::Expression ED::Expression::evaluate(const std::function<bool(const Variable&)>& indicator) const {
    Expression expr = *this;
    std::stack<Term*> fifo;
    fifo.push(expr._root);
    while (!fifo.empty()) {
        Term* current = fifo.top();
        fifo.pop();
        if (current->type() == Term::Var && !indicator(current->as_variable())) current->use_variable_as_numerical();
        if (current->has_left()) fifo.push(&current->left());
        if (current->has_right()) fifo.push(&current->right());
    }
    expr.reduce();
    return expr;
}
