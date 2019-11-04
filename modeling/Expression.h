//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_EXPRESSION_H
#define ED_SOLVER_EXPRESSION_H

namespace ED {
    class Variable;
    class Term;
    class Expression;

    Expression operator*(const Expression& lhs, const Expression& rhs);
    Expression operator+(const Expression& lhs, const Expression& rhs);
    Expression operator-(const Expression& lhs, const Expression& rhs);
    Expression operator/(const Expression& lhs, const Expression& rhs);
    Expression operator*(const Term& lhs, const Term& rhs);
    Expression operator+(const Term& lhs, const Term& rhs);
    Expression operator-(const Term& lhs, const Term& rhs);
    Expression operator/(const Term& lhs, const Term& rhs);
    Expression operator*(const Expression& lhs, Variable& rhs);
    Expression operator+(const Expression& lhs, Variable& rhs);
    Expression operator-(const Expression& lhs, Variable& rhs);
    Expression operator/(const Expression& lhs, Variable& rhs);
    Expression operator*(Variable& lhs, const Expression& rhs);
    Expression operator+(Variable& lhs, const Expression& rhs);
    Expression operator/(Variable& lhs, const Expression& rhs);
    Expression operator-(Variable& lhs, const Expression& rhs);
    Term pow(const Expression& lhs, const Expression &rhs);
    Term pow(const Expression& lhs, float rhs);
    Term pow(float lhs, const Expression& rhs);
    Term pow(const Expression& lhs, Variable& rhs);
    Term pow(Variable& lhs, const Expression& rhs);
    Term cos(const Expression& expr);
    Term sin(const Expression& expr);
    Term sqrt(const Expression& expr);
    Term log(const Expression& expr);
    Term ln(const Expression& expr);
    Term exp(const Expression& expr);
    Term cos(Variable& expr);
    Term sin(Variable& expr);
    Term sqrt(Variable& expr);
    Term log(Variable& expr);
    Term ln(Variable& expr);
    Term exp(Variable& expr);
    Term cos(float expr);
    Term sin(float expr);
    Term sqrt(float expr);
    Term log(float expr);
    Term ln(float expr);
    Term exp(float expr);
}

class ED::Term {
public:
    enum Type { Num, Var, Sum, Mul, Cos, Sin, Ln, Log, Exp, Sqrt, Div, Sub, Pow };
private:
    Type _type;
    Variable* _variable = nullptr;
    float _value = 0.0;
    Term* _left = nullptr;
    Term* _right = nullptr;
public:
    Term(float num);
    Term(Variable& var);
    Term(Type type);
    Term(const Term& term);

    Type type() const { return _type; }
    Variable& as_variable();
    float as_numerical();
    void use_variable_as_numerical();
    bool is_hidden_variable();

    void left(Term& l) { _left = &l; }
    void right(Term& r) { _right = &r; }
    bool has_left() const { return _left != nullptr; }
    bool has_right() const { return _right != nullptr; }
    Term& left() const { return *_left; }
    Term& right() const { return *_right; }

    std::string to_string() const;
};

class ED::Expression {
    Term* _root = nullptr;
    void add_operation(const Term& term, Term::Type type);
    static Expression apply_operation(const Expression& lhs, const Expression& rhs, Term::Type type);
    static Expression apply_operation(const Term& lhs, const Term& rhs, Term::Type type);
    static Term apply_function(const Expression& expr, Term::Type type);
public:
    Expression();
    Expression(const Term& term);
    Expression(const Expression& expr);
    void operator+=(const Term& term);
    void operator-=(const Term& term);
    void operator*=(const Term& term);
    void operator/=(const Term& term);

    friend Expression operator*(const Expression& lhs, const Expression& rhs);
    friend Expression operator+(const Expression& lhs, const Expression& rhs);
    friend Expression operator-(const Expression& lhs, const Expression& rhs);
    friend Expression operator/(const Expression& lhs, const Expression& rhs);
    friend Expression operator*(const Term& lhs, const Term& rhs);
    friend Expression operator+(const Term& lhs, const Term& rhs);
    friend Expression operator-(const Term& lhs, const Term& rhs);
    friend Expression operator/(const Term& lhs, const Term& rhs);
    friend Expression operator*(const Expression& lhs, Variable& rhs);
    friend Expression operator+(const Expression& lhs, Variable& rhs);
    friend Expression operator-(const Expression& lhs, Variable& rhs);
    friend Expression operator/(const Expression& lhs, Variable& rhs);
    friend Expression operator*(Variable& lhs, const Expression& rhs);
    friend Expression operator+(Variable& lhs, const Expression& rhs);
    friend Expression operator/(Variable& lhs, const Expression& rhs);
    friend Expression operator-(Variable& lhs, const Expression& rhs);
    friend Term pow(const Expression& lhs, const Expression &rhs);
    friend Term pow(const Expression& lhs, float rhs);
    friend Term pow(float lhs, const Expression& rhs);
    friend Term pow(const Expression& lhs, Variable& rhs);
    friend Term pow(Variable& lhs, const Expression& rhs);

    friend Term cos(const Expression& expr);
    friend Term sin(const Expression& expr);
    friend Term sqrt(const Expression& expr);
    friend Term log(const Expression& expr);
    friend Term ln(const Expression& expr);
    friend Term exp(const Expression& expr);
    friend Term cos(Variable& expr);
    friend Term sin(Variable& expr);
    friend Term sqrt(Variable& expr);
    friend Term log(Variable& expr);
    friend Term ln(Variable& expr);
    friend Term exp(Variable& expr);
    friend Term cos(float expr);
    friend Term sin(float expr);
    friend Term sqrt(float expr);
    friend Term log(float expr);
    friend Term ln(float expr);
    friend Term exp(float expr);

    void to_dot();
};


#endif //ED_SOLVER_EXPRESSION_H
