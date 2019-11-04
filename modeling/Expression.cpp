//
// Created by hlefebvr on 04/11/19.
//

#include <fstream>
#include <stack>
#include <functional>
#include "Expression.h"
#include "../utils/Exception.h"
#include "Variable.h"

/****  TERM  ****/

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
ED::Term::~Term() {
    delete _left;
    delete _right;
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
