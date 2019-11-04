//
// Created by hlefebvr on 04/11/19.
//

#include <fstream>
#include <stack>
#include <functional>
#include "Expression.h"
#include "../utils/Exception.h"
#include "Variable.h"

ED::Variable &ED::Term::as_variable() {
    if (_type != Var) throw Exception("Trying to convert non-variable term to variable");
    return *_variable;
}

float ED::Term::as_numerical() {
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

ED::Term::Term(float num) {
    _type = Num;
    _value = num;
}

ED::Term::Term(ED::Variable &var) {
    _type = Var;
    _variable = &var;
}

ED::Term::Term(ED::Term::Type type) {
    if (type == Num || type == Var) throw Exception("Term(Type) should only be used to build functions");
    _type = type;
}

ED::Term::Term(const Term& term) {
    _left = term._left;
    _right = term._right;
    _type = term._type;
    _variable = term._variable;
    _value = term._value;
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

ED::Expression::Expression() {}

ED::Expression::Expression(const ED::Term &term) {
    _root = new Term(term);
}

ED::Expression::Expression(const ED::Expression& expr) {
    _root = new Term(*expr._root);
}

void ED::Expression::add_operation(const ED::Term &term, ED::Term::Type type) {
    if (_root == nullptr) { _root = new Term(term); return; }
    Term& sum = *new Term(type);
    sum.right(*new Term(term));
    sum.left(*_root);
    _root = &sum;
}

ED::Expression ED::Expression::apply_operation(const ED::Expression &lhs, const ED::Expression &rhs, Term::Type type) {
    auto root = new Term(type);
    root->left(*new Term(*lhs._root));
    root->right(*new Term(*rhs._root));
    return *root;
}

void ED::Expression::operator+=(const ED::Term &term) {
    add_operation(term, Term::Sum);
}

void ED::Expression::operator-=(const ED::Term &term) {
    add_operation(term, Term::Sub);
}

void ED::Expression::operator/=(const ED::Term &term) {
    add_operation(term, Term::Div);
}

void ED::Expression::operator*=(const ED::Term &term) {
    add_operation(term, Term::Mul);
}

ED::Expression ED::operator*(const ED::Expression& lhs, const ED::Expression& rhs) {
    return ED::Expression::apply_operation(lhs, rhs, ED::Term::Mul);
}

ED::Expression ED::operator+(const ED::Expression& lhs, const ED::Expression& rhs) {
    return ED::Expression::apply_operation(lhs, rhs, ED::Term::Sum);
}

ED::Expression ED::operator-(const ED::Expression& lhs, const ED::Expression& rhs) {
    return ED::Expression::apply_operation(lhs, rhs, ED::Term::Sub);
}

ED::Expression ED::operator/(const ED::Expression& lhs, const ED::Expression& rhs) {
    return ED::Expression::apply_operation(lhs, rhs, ED::Term::Div);
}

ED::Expression ED::Expression::apply_operation(const ED::Term &lhs, const ED::Term &rhs, ED::Term::Type type) {
    Term& root = *new Term(type);
    root.left(*new Term(lhs));
    root.right(*new Term(rhs));
    return root;
}

ED::Expression ED::operator*(const ED::Term &lhs, const ED::Term &rhs) {
    return ED::Expression::apply_operation(lhs, rhs, Term::Mul);
}

ED::Expression ED::operator/(const ED::Term &lhs, const ED::Term &rhs) {
    return ED::Expression::apply_operation(lhs, rhs, Term::Div);
}

ED::Expression ED::operator+(const ED::Term &lhs, const ED::Term &rhs) {
    return ED::Expression::apply_operation(lhs, rhs, Term::Sum);
}

ED::Expression ED::operator-(const ED::Term &lhs, const ED::Term &rhs) {
    return ED::Expression::apply_operation(lhs, rhs, Term::Sub);
}

ED::Expression ED::operator*(const ED::Expression &lhs, ED::Variable &rhs) {
    return lhs * Term(rhs);
}

ED::Expression ED::operator+(const ED::Expression &lhs, ED::Variable &rhs) {
    return lhs + Term(rhs);
}

ED::Expression ED::operator-(const ED::Expression &lhs, ED::Variable &rhs) {
    return lhs - Term(rhs);
}

ED::Expression ED::operator/(ED::Variable &lhs, const ED::Expression &rhs) {
    return Term(lhs) / rhs;
}

ED::Expression ED::operator*(ED::Variable &lhs, const ED::Expression &rhs) {
    return Term(lhs) * rhs;
}

ED::Expression ED::operator/(const ED::Expression &lhs, ED::Variable &rhs) {
    return lhs / Term(rhs);
}

ED::Expression ED::operator+(ED::Variable &lhs, const ED::Expression &rhs) {
    return Term(lhs) + rhs;
}

ED::Expression ED::operator-(ED::Variable &lhs, const ED::Expression &rhs) {
    return Term(lhs) - rhs;
}

void ED::Expression::to_dot() {
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

ED::Term ED::pow(const ED::Expression &lhs, const ED::Expression &rhs) {
    ED::Term new_root(ED::Term::Pow);
    new_root.left(*lhs._root);
    new_root.right(*rhs._root);
    return new_root;
}

ED::Term ED::pow(const ED::Expression &lhs, float rhs) {
    return pow(lhs, Term(rhs));
}

ED::Term ED::pow(float lhs, const ED::Expression &rhs) {
    return pow(Term(lhs), rhs);
}

ED::Term ED::pow(const ED::Expression &lhs, ED::Variable &rhs) {
    return pow(lhs, Term(rhs));
}

ED::Term ED::pow(ED::Variable &lhs, const ED::Expression &rhs) {
    return pow(Term(lhs), rhs);
}

ED::Term ED::Expression::apply_function(const Expression& expr, Term::Type type) {
    Term new_term(type);
    new_term.right(*expr._root);
    return new_term;
}

ED::Term ED::cos(const ED::Expression &expr) {
    return ED::Expression::apply_function(expr, ED::Term::Cos);
}

ED::Term ED::sin(const ED::Expression &expr) {
    return ED::Expression::apply_function(expr, ED::Term::Sin);
}

ED::Term ED::sqrt(const ED::Expression &expr) {
    return ED::Expression::apply_function(expr, ED::Term::Sqrt);
}

ED::Term ED::log(const ED::Expression &expr) {
    return ED::Expression::apply_function(expr, ED::Term::Log);
}

ED::Term ED::ln(const ED::Expression &expr) {
    return ED::Expression::apply_function(expr, ED::Term::Ln);
}

ED::Term ED::exp(const ED::Expression &expr) {
    return ED::Expression::apply_function(expr, ED::Term::Exp);
}

ED::Term ED::cos(ED::Variable &expr) {
    return ED::Expression::apply_function(Term(expr), ED::Term::Cos);
}

ED::Term ED::sin(ED::Variable &expr) {
    return ED::Expression::apply_function(Term(expr), ED::Term::Sin);
}

ED::Term ED::sqrt(ED::Variable &expr) {
    return ED::Expression::apply_function(Term(expr), ED::Term::Sqrt);
}

ED::Term ED::log(ED::Variable &expr) {
    return ED::Expression::apply_function(Term(expr), ED::Term::Log);
}

ED::Term ED::ln(ED::Variable &expr) {
    return ED::Expression::apply_function(Term(expr), ED::Term::Ln);
}

ED::Term ED::exp(ED::Variable &expr) {
    return ED::Expression::apply_function(Term(expr), ED::Term::Exp);
}

ED::Term ED::cos(float expr) {
    return ED::Expression::apply_function(Term(expr), ED::Term::Cos);
}

ED::Term ED::sin(float expr) {
    return ED::Expression::apply_function(Term(expr), ED::Term::Sin);
}

ED::Term ED::sqrt(float expr) {
    return ED::Expression::apply_function(Term(expr), ED::Term::Sqrt);
}

ED::Term ED::log(float expr) {
    return ED::Expression::apply_function(Term(expr), ED::Term::Log);
}

ED::Term ED::ln(float expr) {
    return ED::Expression::apply_function(Term(expr), ED::Term::Ln);
}

ED::Term ED::exp(float expr) {
    return ED::Expression::apply_function(Term(expr), ED::Term::Exp);
}
