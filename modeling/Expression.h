//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_EXPRESSION_H
#define ED_SOLVER_EXPRESSION_H

namespace ED {
    class Variable;
    class Term;
    class Expression;

    static Expression operator+(Variable& lhs, Variable& rhs);
    static Expression operator-(Variable& lhs, Variable& rhs);
    static Expression operator*(Variable& lhs, Variable& rhs);
    static Expression operator/(Variable& lhs, Variable& rhs);
    static Expression operator+(Variable& lhs, float rhs);
    static Expression operator-(Variable& lhs, float rhs);
    static Expression operator*(Variable& lhs, float rhs);
    static Expression operator/(Variable& lhs, float rhs);
    static Expression operator+(float lhs, Variable& rhs);
    static Expression operator-(float lhs, Variable& rhs);
    static Expression operator*(float lhs, Variable& rhs);
    static Expression operator/(float lhs, Variable& rhs);
    static Expression operator+(const Expression& lhs, const Expression& rhs);
    static Expression operator-(const Expression& lhs, const Expression& rhs);
    static Expression operator*(const Expression& lhs, const Expression& rhs);
    static Expression operator/(const Expression& lhs, const Expression& rhs);
    static Expression operator+(const Expression& lhs, Variable& rhs);
    static Expression operator-(const Expression& lhs, Variable& rhs);
    static Expression operator*(const Expression& lhs, Variable& rhs);
    static Expression operator/(const Expression& lhs, Variable& rhs);
    static Expression operator+(Variable& lhs, const Expression& rhs);
    static Expression operator-(Variable& lhs, const Expression& rhs);
    static Expression operator*(Variable& lhs, const Expression& rhs);
    static Expression operator/(Variable& lhs, const Expression& rhs);
    static Expression operator+(const Expression& lhs, float rhs);
    static Expression operator-(const Expression& lhs, float rhs);
    static Expression operator*(const Expression& lhs, float rhs);
    static Expression operator/(const Expression& lhs, float rhs);
    static Expression operator+(float lhs, const Expression& rhs);
    static Expression operator-(float lhs, const Expression& rhs);
    static Expression operator*(float lhs, const Expression& rhs);
    static Expression operator/(float lhs, const Expression& rhs);

    static Expression sin(const Expression& expr);
    static Expression sin(Variable& var);
    static Expression sin(float v);
    static Expression cos(const Expression& expr);
    static Expression cos(Variable& var);
    static Expression cos(float v);
    static Expression sqrt(const Expression& expr);
    static Expression sqrt(Variable& var);
    static Expression sqrt(float v);
    static Expression log(const Expression& expr);
    static Expression log(Variable& var);
    static Expression log(float v);
    static Expression ln(const Expression& expr);
    static Expression ln(Variable& var);
    static Expression ln(float v);
    static Expression exp(const Expression& expr);
    static Expression exp(Variable& var);
    static Expression exp(float v);
    static Expression pow(const Expression& expr1, const Expression& expr2);
    static Expression pow(const Expression& expr, Variable& var);
    static Expression pow(const Expression& expr, float v);
    static Expression pow(float v, const Expression& expr);
    static Expression pow(float v, Variable& var);
    static Expression pow(float v1, float v2);
    static Expression pow(Variable& var, const Expression& expr);
    static Expression pow(Variable& var1, Variable& var2);
    static Expression pow(Variable& var, float v);
}

class ED::Term {
public:
    enum Type { Num, Var, Sum, Mul, Div, Sub, Pow, Cos, Sin, Ln, Log, Exp, Sqrt };
protected:
    Type _type;
    Variable* _variable = nullptr;
    float _value = 0.0;
    Term* _left = nullptr;
    Term* _right = nullptr;
    bool _expert_mode = false;
public:
    explicit Term(float num); //!< builds a numerical term
    explicit Term(Variable& var); //!< builds a variable term
    explicit Term(Type type); //!< builds a functional term
    Term(const Term& term); //!< copies a term and its left and right operands
    Term& operator=(const Term& term); //!< assigns by copy a term and its left and right operands
    virtual ~Term();

    Type type() const { return _type; }
    Variable& as_variable();
    float& as_numerical();
    void use_variable_as_numerical();
    bool is_hidden_variable();

    void left(Term& l) { _left = &l; }
    void right(Term& r) { _right = &r; }
    bool has_left() const { return _left != nullptr; }
    bool has_right() const { return _right != nullptr; }
    // todo add has_left/right check ?
    Term& left() const { return *_left; }
    Term& right() const { return *_right; }

    std::string to_string() const;
    void expert_mode(bool b) { _expert_mode = b; } //!< the expert mode is used when the term does not free the memory
    bool expert_mode() { return _expert_mode; }
    void swap();

    static bool is_binary_operator(Type type);
    static bool is_unitary_operator(Type type);
    static float apply(float n1, Type op, float n2);
};

/***
 * \details the smaller expressions on the right the better
 */
class ED::Expression {
    Term* _root = nullptr;

    void add_term(const Term& rhs, Term::Type operation);
    static inline Expression unitary_function(const Expression& expr, Term::Type type);
    explicit Expression(const Term& term);
public:
    Expression() = default;
    virtual ~Expression();
    Expression(const Expression& expr);
    explicit Expression(Variable& var);
    explicit Expression(float v);
    Expression& operator=(const Expression& expr);
    Expression& operator=(Variable& var) { return (*this = Expression(var)); }
    Expression& operator=(float v) { return (*this = Expression(v)); }

    void remove_minus();
    void expand(); //!< Turns the expression in expanded summation form
    void reduce();

    // operators that do not copy the left term
    void operator+=(float x) { add_term(Term(x), Term::Sum); }
    void operator-=(float x) { add_term(Term(x), Term::Sub); }
    void operator*=(float x) { add_term(Term(x), Term::Mul); }
    void operator/=(float x) { add_term(Term(x), Term::Div); }
    void operator+=(Variable& var) { add_term(Term(var), Term::Sum); }
    void operator-=(Variable& var) { add_term(Term(var), Term::Sub); }
    void operator*=(Variable& var) { add_term(Term(var), Term::Mul); }
    void operator/=(Variable& var) { add_term(Term(var), Term::Div); }
    void operator+=(const Expression& rhs) { add_term(*rhs._root, Term::Sum); }
    void operator-=(const Expression& rhs) { add_term(*rhs._root, Term::Sub); }
    void operator*=(const Expression& rhs) { add_term(*rhs._root, Term::Mul); }
    void operator/=(const Expression& rhs) { add_term(*rhs._root, Term::Div); }

    // operators that copy both terms
    friend Expression operator+(Variable& lhs, Variable& rhs) { return Expression(lhs) + Expression(rhs); }
    friend Expression operator-(Variable& lhs, Variable& rhs) { return Expression(lhs) - Expression(rhs); }
    friend Expression operator*(Variable& lhs, Variable& rhs) { return Expression(lhs) * Expression(rhs); }
    friend Expression operator/(Variable& lhs, Variable& rhs) { return Expression(lhs) / Expression(rhs); }
    friend Expression operator+(Variable& lhs, float rhs) { return Expression(lhs) + Expression(rhs); }
    friend Expression operator-(Variable& lhs, float rhs) { return Expression(lhs) - Expression(rhs); }
    friend Expression operator*(Variable& lhs, float rhs) { return Expression(lhs) * Expression(rhs); }
    friend Expression operator/(Variable& lhs, float rhs) { return Expression(lhs) / Expression(rhs); }
    friend Expression operator+(float lhs, Variable& rhs) { return Expression(lhs) + Expression(rhs); }
    friend Expression operator-(float lhs, Variable& rhs) { return Expression(lhs) - Expression(rhs); }
    friend Expression operator*(float lhs, Variable& rhs) { return Expression(lhs) * Expression(rhs); }
    friend Expression operator/(float lhs, Variable& rhs) { return Expression(lhs) / Expression(rhs); }
    inline friend Expression operator+(const Expression& lhs, const Expression& rhs);
    inline friend Expression operator-(const Expression& lhs, const Expression& rhs);
    inline friend Expression operator*(const Expression& lhs, const Expression& rhs);
    inline friend Expression operator/(const Expression& lhs, const Expression& rhs);
    friend Expression operator+(const Expression& lhs, Variable& rhs) { return lhs + Expression(Term(rhs)); }
    friend Expression operator-(const Expression& lhs, Variable& rhs) { return lhs - Expression(Term(rhs)); }
    friend Expression operator*(const Expression& lhs, Variable& rhs) { return lhs * Expression(Term(rhs)); }
    friend Expression operator/(const Expression& lhs, Variable& rhs) { return lhs / Expression(Term(rhs)); }
    friend Expression operator+(Variable& lhs, const Expression& rhs) { return Expression(Term(lhs)) + rhs; }
    friend Expression operator-(Variable& lhs, const Expression& rhs) { return Expression(Term(lhs)) - rhs; }
    friend Expression operator*(Variable& lhs, const Expression& rhs) { return Expression(Term(lhs)) * rhs; }
    friend Expression operator/(Variable& lhs, const Expression& rhs) { return Expression(Term(lhs)) / rhs; }
    friend Expression operator+(const Expression& lhs, float rhs) { return lhs + Expression(Term(rhs)); }
    friend Expression operator-(const Expression& lhs, float rhs) { return lhs - Expression(Term(rhs)); }
    friend Expression operator*(const Expression& lhs, float rhs) { return lhs * Expression(Term(rhs)); }
    friend Expression operator/(const Expression& lhs, float rhs) { return lhs / Expression(Term(rhs)); }
    friend Expression operator+(float lhs, const Expression& rhs) { return Expression(Term(lhs)) + rhs; }
    friend Expression operator-(float lhs, const Expression& rhs) { return Expression(Term(lhs)) - rhs; }
    friend Expression operator*(float lhs, const Expression& rhs) { return Expression(Term(lhs)) * rhs; }
    friend Expression operator/(float lhs, const Expression& rhs) { return Expression(Term(lhs)) / rhs; }

    // functions
    friend Expression sin(const Expression& expr) { return Expression::unitary_function(expr, Term::Sin); }
    friend Expression sin(Variable& var) { return sin(Expression(Term(var))); }
    friend Expression sin(float v) { return sin(Expression(Term(v))); }
    friend Expression cos(const Expression& expr) { return Expression::unitary_function(expr, Term::Cos); }
    friend Expression cos(Variable& var) { return cos(Expression(Term(var))); }
    friend Expression cos(float v) { return cos(Expression(Term(v))); }
    friend Expression sqrt(const Expression& expr) { return Expression::unitary_function(expr, Term::Sqrt); }
    friend Expression sqrt(Variable& var) { return sqrt(Expression(Term(var))); }
    friend Expression sqrt(float v) { return sqrt(Expression(Term(v))); }
    friend Expression log(const Expression& expr) { return Expression::unitary_function(expr, Term::Log); }
    friend Expression log(Variable& var) { return log(Expression(Term(var))); }
    friend Expression log(float v) { return log(Expression(Term(v))); }
    friend Expression ln(const Expression& expr) { return Expression::unitary_function(expr, Term::Ln); }
    friend Expression ln(Variable& var) { return ln(Expression(Term(var))); }
    friend Expression ln(float v) { return ln(Expression(Term(v))); }
    friend Expression exp(const Expression& expr) { return Expression::unitary_function(expr, Term::Exp); }
    friend Expression exp(Variable& var) { return exp(Expression(Term(var))); }
    friend Expression exp(float v) { return exp(Expression(Term(v))); }
    inline friend Expression pow(const Expression& expr1, const Expression& expr2);
    friend Expression pow(const Expression& expr, Variable& var) { return pow(expr, Expression(var)); }
    friend Expression pow(const Expression& expr, float v) { return pow(expr, Expression(v)); }
    friend Expression pow(float v, const Expression& expr) { return pow(Expression(v), expr); }
    friend Expression pow(float v, Variable& var) { return pow(Expression(v), Expression(var)); }
    friend Expression pow(float v1, float v2) { return pow(Expression(v1), Expression(v2)); }
    friend Expression pow(Variable& var, const Expression& expr) { return pow(Expression(var), Expression(expr)); }
    friend Expression pow(Variable& var1, Variable& var2) { return pow(Expression(var1), Expression(var2)); }
    friend Expression pow(Variable& var, float v) { return pow(Expression(var), Expression(v)); }

    void to_dot() const;
    std::string to_string() const;
};

ED::Expression ED::operator*(const ED::Expression &lhs, const ED::Expression &rhs) {
    Expression result = Expression(lhs);
    result *= rhs;
    return result;
}

ED::Expression ED::operator+(const ED::Expression &lhs, const ED::Expression &rhs) {
    Expression result = Expression(lhs);
    result += rhs;
    return result;
}

ED::Expression ED::operator-(const ED::Expression &lhs, const ED::Expression &rhs) {
    Expression result = Expression(lhs);
    result -= rhs;
    return result;
}

ED::Expression ED::operator/(const ED::Expression &lhs, const ED::Expression &rhs) {
    Expression result = Expression(lhs);
    result /= rhs;
    return result;
}

ED::Expression ED::Expression::unitary_function(const ED::Expression &expr, ED::Term::Type type) {
    Term term(type);
    term.right(*new Term(*expr._root));
    return Expression(term);
}

ED::Expression ED::pow(const ED::Expression &expr1, const ED::Expression &expr2) {
    Term term(Term::Pow);
    term.left(*new Term(*expr1._root));
    term.right(*new Term(*expr2._root));
    return Expression(term);
}

#endif //ED_SOLVER_EXPRESSION_H
