//
// Created by hlefebvr on 13/11/19.
//

#include <fstream>
#include "Expression.h"
#include "Variable.h"
#include "../utils/Exception.h"

unsigned long int L::Expression::_expression_id = 0;

/** CONSTRUCTORS **/

L::Expression::Expression(L::ExpressionType type) : _type(type) {}

L::Expression::Expression(const L::Variable &variable) : _type(Var), _variable(&variable._core) {}

L::Expression::Expression(float coef) : _type(Num), _numerical(coef) {}

L::Expression &L::Expression::operator=(const L::Expression &rhs) {
    _type = rhs._type;
    _numerical = rhs._numerical;
    _variable = rhs._variable;
    if (rhs._left) _left = new Expression(*rhs._left);
    if (rhs._right) _right = new Expression(*rhs._right);
    return *this;
}

L::Expression::Expression(const L::Expression &rhs)
    : _type(rhs._type),
      _numerical(rhs._numerical),
      _variable(rhs._variable),
      _left(rhs._left ? new Expression(*rhs._left) : nullptr),
      _right(rhs._right ? new Expression(*rhs._right) : nullptr)
{}

/** GETTERS **/

unsigned long int L::Expression::id() const {
    return _id;
}

L::Variable L::Expression::as_variable() {
    if (_type != Var) throw Exception("Cannot cast non-variable expression to variable");
    return Variable(*_variable);
}

L::ConstVariable L::Expression::as_variable() const {
    if (_type != Var) throw Exception("Cannot cast non-variable expression to variable");
    return ConstVariable(*_variable);
}

float &L::Expression::as_numerical() {
    if (_type != Num) throw Exception("Cannot cast non-numerical expression to numerical");
    return _numerical;
}

float L::Expression::as_numerical() const {
    if (_type != Num) throw Exception("Cannot cast non-numerical expression to numerical");
    return _numerical;
}

bool L::Expression::has_child(L::ExpressionSide side) const {
    return side == Left ? _left : _right;
}

L::Expression &L::Expression::child(L::ExpressionSide side) {
    if (!has_child(side)) throw Exception("Trying to read non-existing child");
    return side == Left ? *_left : *_right;
}

const L::Expression &L::Expression::child(L::ExpressionSide side) const {
    if (!has_child(side)) throw Exception("Trying to read non-existing child");
    return side == Left ? *_left : *_right;
}

/** SETTERS **/

void L::Expression::attach(L::ExpressionSide side, L::Expression &expr) {
    if (has_child(side)) throw Exception("Trying to attach to a non-empty side");
    if (side == Left) _left = &expr; else _right = &expr;
}

void L::Expression::attach_copy(L::ExpressionSide side, const L::Expression &expr) {
    Expression& copy = *new Expression(expr);
    attach(side, copy);
}

void L::Expression::detach_delete(L::ExpressionSide side) {
    if (!has_child(side)) throw Exception("Cannot detach nothing");
    if (side == Left) { delete _left; _left = nullptr; }
    else { delete _right; _right = nullptr; }
}

void L::Expression::insert(L::ExpressionSide side, L::Expression &expr) {
    if (has_child(side) && child(side).has_child(side)) throw Exception("Invalid insertion");
    Expression* transferred = side == Left ? _left : _right;
    if (side == Left) { _left = &expr; expr._left = transferred; } else { _right = &expr; expr._right = transferred; }
}

void L::Expression::shift(L::ExpressionSide side) {
    unsigned long int my_id = _id;
    Expression& shifted = *new Expression(_type);
    shifted._numerical = _numerical;
    shifted._variable = _variable;
    shifted._left = _left;
    shifted._right = _right;
    _id = shifted._id;
    shifted._id = my_id;
    if (side == Left) { _left = &shifted; _right = nullptr; } else { _right = &shifted; _left = nullptr; }
}

/** OPERATIONS ON CHILDREN **/

void L::transfer(L::ExpressionSide from_side, L::Expression &from_expr, L::ExpressionSide to_side, L::Expression &to_expr) {
    if (!from_expr.has_child(from_side)) throw Exception("Cannot transfer nothing");
    if (to_expr.has_child(to_side)) throw Exception("Cannot transfer to non-empty side");

    Expression* transferred;
    if (from_side == Left) { transferred = from_expr._left; from_expr._left = nullptr; }
    else { transferred = from_expr._right; from_expr._right = nullptr; }

    if (to_side == Left) to_expr._left = transferred;
    else to_expr._right = transferred;
}

void L::swap(L::ExpressionSide from_side, L::Expression &from_expr, L::ExpressionSide to_side, L::Expression &to_expr) {
    if (!from_expr.has_child(from_side) || !to_expr.has_child(to_side)) throw Exception("Cannot swap nothing");
    Expression* transferred;
    if (from_side == Left) {
        transferred = from_expr._left;
        from_expr._left = to_side == Left ? to_expr._left : to_expr._right;
    } else {
        transferred = from_expr._right;
        from_expr._right = to_side == Left ? to_expr._left : to_expr._right;
    }

    if (to_side == Left) to_expr._left = transferred;
    else to_expr._right = transferred;
}

/** OPERATORS **/

L::Expression &L::Expression::apply_operator(L::ExpressionType type, const L::Expression &rhs) {
    if (type != Sum && type != Prod) throw Exception("Call to apply_operator with invalid operator type.");

    if (_type == type && (_type == Sum || _type == Prod)) {
        Expression& added_operator = *new Expression(type);
        added_operator.attach_copy(Right, rhs);
        insert(Left, added_operator);
    } else {
        shift(Left);
        _type = type;
        attach_copy(Right, rhs);
    }

    return *this;
}

L::Expression& L::Expression::operator+=(const Expression& rhs) {
    return apply_operator(Sum, rhs);
}

L::Expression& L::Expression::operator*=(const Expression& rhs) {
    return apply_operator(Prod, rhs);
}

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

    traversal(*this);
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

    traversal(*this);
}

std::string L::Expression::to_string() const {
    switch (_type) {
        case Num: return std::to_string(_numerical);
        case Var: return _variable->user_defined_name();
        case Prod: return "*";
        case Sum: return "+";
        case Sqrt: return "sqrt";
        case Exp: return "exp";
        case Pow: return "^";
        case Ln: return "ln";
        default: throw Exception("Unknown expression type" );
    }
}

void L::Expression::export_to_dot(const std::string &filename, bool with_system_command) const {
    std::string dot_filename = filename + ".dot";
    std::string png_filename = filename + ".png";

    std::ofstream f;
    f.open(dot_filename);

    f << "graph " + filename << " {\n";
    depth_first_traversal<PostOrder>([&f](const Expression &current) {
        f << "\t" << current.id() << "[label=\"" << current.to_string() << "\"]\n";
        if (current.has_child(Left)) f << "\t " << current.id() << " -- " << current.child(Left).id() << "[color=\"red\"]\n";
        if (current.has_child(Right)) f << "\t " << current.id() << " -- " << current.child(Right).id() << "\n";
    });
    f << "}";

    f.close();

    if (with_system_command) system(("dot -Tpng " + dot_filename + " > " + png_filename).c_str());
}

L::Expression L::operator+(const L::Expression& a, const L::Expression& b) {
    return (Expression(a) += b);
}

L::Expression L::operator*(const L::Expression& a, const L::Expression& b) {
    return (Expression(a) *= b);
}

L::Expression L::operator-(const L::Expression& a, const L::Expression& b) {
    return (a + (-1.0) * b);
}

L::Expression L::operator/(const L::Expression& a, const L::Expression& b) {
    throw Exception("Not implemented yet");
}
