//
// Created by hlefebvr on 05/11/19.
//

#ifndef ED_SOLVER_CONSTRAINT_H
#define ED_SOLVER_CONSTRAINT_H

#include <string>
#include "Expression.h"
#include "../utils/Exception.h"

namespace L {
    class AbstractConstraint;
    class CoreConstraint;
    class Constraint;
    class ConstConstraint;
    class Environment;
}

class L::AbstractConstraint {
public:
    enum Type {LessOrEqualTo, GreaterOrEqualTo, EqualTo};
    enum Status { Core, Default };

    // getters
    virtual Type type() const = 0;
    virtual const Expression& expression() const = 0;
    virtual Expression& expression() = 0;
    virtual std::string user_defined_name() const = 0;
    virtual Status status() const = 0;

    // setters
    virtual void expression(const Expression& expr) = 0;
    virtual void type(Type) = 0;

    friend std::ostream& operator<<(std::ostream& os, const Constraint& constraint);
    static std::string to_string(Type type);
};

class L::CoreConstraint : public AbstractConstraint {
protected:
    Expression _expr;
    AbstractConstraint::Type _type = LessOrEqualTo;
    std::string _user_defined_name;
public:
    // constructors
    explicit CoreConstraint(std::string user_defined_name);

    // getters
    Expression& expression() override { return _expr; }
    const Expression& expression() const override { return _expr; }
    Type type() const override { return _type; }
    std::string user_defined_name() const override { return _user_defined_name; }
    Status status() const override { return Core; }

    // setters
    void expression(const Expression& expr) override { _expr = expr; }
    void type(Type type) override { _type = type; }
};

class L::Constraint : public AbstractConstraint {
    CoreConstraint& _core;
public:
    // constructors
    Constraint(Environment& env, const std::string& name);
    explicit Constraint(CoreConstraint& core);
    Constraint(const Constraint& rhs) : _core(rhs._core) {}
    Constraint& operator=(const Constraint& rhs) { _core = rhs._core; return *this; }

    // getters
    Type type() const override { return _core.type(); }
    Expression& expression() override { return _core.expression(); }
    const Expression& expression() const override { return _core.expression(); }
    std::string user_defined_name() const override { return _core.user_defined_name(); }
    Status status() const override { return Default; }

    // setters
    void type(Type type) override { _core.type(type); }
    void expression(const Expression& expr) override { _core.expression(expr); }
};

class L::ConstConstraint : public AbstractConstraint {
    CoreConstraint& _core;
    void type(Type type) override {}
    void expression(const Expression& expr) override {  }
    Expression& expression() override { throw Exception("Const constraint"); }
public:
    // constructor
    explicit ConstConstraint(CoreConstraint& core);
    ConstConstraint& operator=(const ConstConstraint& rhs) { _core = rhs._core; return *this; }

    // getters
    Type type() const override { return _core.type(); }
    const Expression& expression() const override { return _core.expression(); }
    std::string user_defined_name() const override { return _core.user_defined_name(); }
    Status status() const override { return Default; }
};


#endif //ED_SOLVER_CONSTRAINT_H
